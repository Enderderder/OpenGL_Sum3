
// Engine Include
#include "Engine.h"

CClothComponent::CClothComponent()
{
	m_width = 20;
	m_height = 30;
	m_restDistance = 0.1f;
}

CClothComponent::~CClothComponent()
{

}

void CClothComponent::BeginPlay()
{
	__super::BeginPlay();

	// Calculate the centre point of the cloth
	float halfWidth = (m_width - 1.0f) * m_restDistance * 0.5f;
	float halfHeight = (m_height - 1.0f) * m_restDistance * 0.5f;

	// Map out all the point and create the point for the cloth in local space
	m_clothPoints.resize(0);
	float x, y, z;
	z = 0.0f;
	for (int row = 0; row < m_height; ++row)
	{
		y = halfHeight - row * m_restDistance;
		for (int col = 0; col < m_width; ++col)
		{
			x = col * m_restDistance - halfWidth;

			CClothPoint* clothPoint = new CClothPoint({ x,y,z });
			m_clothPoints.push_back(clothPoint);
		}
	}
	
	// Set the top row of the cloth to fix position
	for (int i = 0; i < m_width; ++i)
	{
		m_clothPoints[i]->m_bMoveable = false;
	}

	// Set the middle row to in-active
	for (int i = 0; i < m_width; ++i)
	{
		m_clothPoints[(6 * m_width) + i]->m_bActive = false;
	}

	// Setup render data for the cloth
	m_program = CAssetMgr::GetInstance()->GetProgramID("UnlitProgram");
	SetupMesh();
}

void CClothComponent::Update()
{
	__super::Update();
	
	// Update each point on the cloth as they will have physics
	float deltaTime = CTime::GetInstance()->GetDeltaTime();
	for (const auto& point : m_clothPoints)
	{
		if (point->m_bActive)
		{
			point->Update(deltaTime);
		}	
	}

	// Refresh the render data every frame
	RefreshMesh();

}

void CClothComponent::RenderColth(CCamera* _camera)
{
	glUseProgram(m_program);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	/** Get the translate scale rotation from the game object transform */
	glm::vec3 objPos = m_owner->m_transform.position;
	glm::vec3 objRotate = m_owner->m_transform.rotation;
	glm::vec3 objScale = m_owner->m_transform.scale;

	/** Calculate the MVP matrix from the game object transform */
	glm::mat4 translate = glm::translate(glm::mat4(), objPos);
	glm::mat4 scale = glm::scale(glm::mat4(), objScale);
	glm::mat4 rotation = glm::mat4();
	rotation = glm::rotate(rotation, glm::radians(objRotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(objRotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(objRotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 model = translate * rotation * scale;
	glm::mat4 mvp = _camera->GetProj() *  _camera->GetView() * model;
	GLint mvpLoc = glGetUniformLocation(m_program, "MVP");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	// Render the shape
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(m_vao);
	glDrawElements(GL_LINES, m_indiceCount, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Unbind
	glBindVertexArray(0);
	glUseProgram(0);
}

void CClothComponent::SetupMesh()
{
	CalculateVertexData();
	CalculateIndiceData();

	// Generate and bind the information
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(
		GL_ARRAY_BUFFER, 
		sizeof(float) * m_vertexBuffer.size(), 
		&m_vertexBuffer[0], 
		GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, 
		sizeof(GLuint) * m_indiceBuffer.size(), 
		&m_indiceBuffer[0], 
		GL_STATIC_DRAW);

	// Set the layout location 0 for the position data of the vertex
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE, 
		3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
}

void CClothComponent::RefreshMesh()
{
	CalculateVertexData();
	CalculateIndiceData();

	// Refresh the binding of datas
	glBindVertexArray(m_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(
		GL_ARRAY_BUFFER, 
		sizeof(float) * m_vertexBuffer.size(), 
		&m_vertexBuffer[0], 
		GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, 
		sizeof(GLuint) * m_indiceBuffer.size(), 
		&m_indiceBuffer[0], 
		GL_STATIC_DRAW);
}

void CClothComponent::CalculateVertexData()
{
	// Reset the vertex buffer
	m_vertexBuffer.clear();
	m_vertexBuffer.resize(0);

	for (const auto& point : m_clothPoints)
	{
		glm::vec3 pointLocalPos = point->m_localPosition;

		m_vertexBuffer.push_back(static_cast<float>(pointLocalPos.x));
		m_vertexBuffer.push_back(static_cast<float>(pointLocalPos.y));
		m_vertexBuffer.push_back(static_cast<float>(pointLocalPos.z));
	}
}

void CClothComponent::CalculateIndiceData()
{
	// Reset the indice buffer
	m_indiceBuffer.clear();
	m_indiceBuffer.resize(0);
	
	// Bind the indice data into the buffer
	for (int row = 0; row < m_height; ++row)
	{
		for (int col = 0; col < m_width; ++col)
		{
			// Link with other points if point is active
			int selfIndex = row * m_width + col;
			if (m_clothPoints[selfIndex]->m_bActive)
			{
				// Link downwards if the point is active
				int downIndex = (row + 1) * m_width + col;
				if (row < m_height - 1 && m_clothPoints[downIndex]->m_bActive)
				{
					m_indiceBuffer.push_back(selfIndex);
					m_indiceBuffer.push_back(downIndex);
				}

				// Link rightwards if the point is active
				int rightIndex = row * m_width + col + 1;
				if (col < m_width - 1 && m_clothPoints[rightIndex]->m_bActive) 
				{
					m_indiceBuffer.push_back(selfIndex);
					m_indiceBuffer.push_back(rightIndex);
				}
			}
		}
	}

	// Calculate the indice count
	m_indiceCount = m_indiceBuffer.size();
}

bool CClothComponent::IndexInBound(int _row, int _col)
{
	return
		_row >= 0 && _row <= m_height - 1
		&& _col >= 0 && _col <= m_width - 1;
}