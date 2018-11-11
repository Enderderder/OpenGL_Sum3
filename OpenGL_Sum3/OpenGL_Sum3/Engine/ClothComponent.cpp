
// Engine Include
#include "Engine.h"

CClothComponent::CClothComponent()
{
	m_width = 13;
	m_height = 20;
	m_restDistance = 1.0f;
	m_stiffness = 0.9f;
}

CClothComponent::~CClothComponent()
{
	for (auto link : m_clothLinks)
	{
		delete link;
		link = nullptr;
	}
	m_clothLinks.clear();

	for (auto point : m_clothPoints)
	{
		delete point;
		point = nullptr;
	}
	m_clothPoints.clear();
}

void CClothComponent::BeginPlay()
{
	__super::BeginPlay();

	BuildCloth();

	// Set the middle row to in-active
	for (int i = 0; i < m_width - 3; ++i)
	{
		//m_clothPoints[(6 * m_width) + i]->m_bActive = false;
	}

	// Create the constrain link between points
	CreateLinks();

	// Setup render data for the cloth
	m_program = CAssetMgr::GetInstance()->GetProgramID("UnlitProgram");
	SetupMesh();
}

void CClothComponent::Update()
{
	__super::Update();

	for (int row = 0; row < m_height - 5; ++row)
	{
		for (int col = 5; col < m_width - 5; ++col)
		{
			if (m_clothPoints[row * m_width + col]->m_bActive)
			{
				m_clothPoints[row * m_width + col]->ApplyForce({ 0.0f, 0.0f, 1.0f });
			}
		}
	}

	// Update each point on the cloth as they will have physics
	float deltaTime = CTime::GetInstance()->GetDeltaTime();
	for (const auto& point : m_clothPoints)
	{
		if (point->m_bActive)
		{
			point->Update(deltaTime);
		}	
	}

	for (const auto& link : m_clothLinks)
	{
		if (link->IsActiveLink())
		{
			link->Update();
		}
	}

	// Constraints all the points thats active so they dont loss their shape
	ConstraintPoints();

	// Check the ground collision
	CheckGroundCollision();

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

void CClothComponent::ApplyForceToPoint(int _row, int _col, glm::vec3 _force)
{
	// If the position is outside the cloth bound, return
	if (IndexInBound(_row, _col) == false)
	{
		return;
	}

	// Apply the force to the certain point
	m_clothPoints[_row * m_width + _col]->ApplyForce(_force);
}

void CClothComponent::ReleaseEntireCloth()
{
	for (const auto& point : m_clothPoints)
	{
		if (point->m_bActive)
		{
			point->m_bMoveable = true;
		}
	}
}

void CClothComponent::ResizeCloth(int _width, int _height)
{
	m_width = _width;
	m_height = _height;

	this->RebuildCloth();
}

void CClothComponent::RebuildCloth()
{
	// Clear out all the links
	for (auto& link : m_clothLinks)
	{
		delete link;
		link = nullptr;
	}
	m_clothLinks.clear();
	m_clothLinks.resize(0);

	// Clear out all the points
	for (auto& point : m_clothPoints)
	{
		delete point;
		point = nullptr;
	}
	m_clothPoints.clear();
	m_clothPoints.resize(0);


	// Rebuild the cloth
	BuildCloth();
	CreateLinks();
	RefreshMesh();
}

void CClothComponent::SetGroundObject(std::shared_ptr<CGameObject> _groundObj)
{
	m_groundObj = _groundObj;
}

void CClothComponent::BuildCloth()
{
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

			CClothPoint* clothPoint = new CClothPoint({ x,y,z }, (row * m_width + col));
			m_clothPoints.push_back(clothPoint);

			// Vary the Z value from 0 - 1 as a float number
			z = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
		}
	}

	// Set the top row of the cloth to fix position
	for (int i = 0; i < m_width; i += 4)
	{
		m_clothPoints[i]->m_bMoveable = false;
	}
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

	// Unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
	for (const auto& link : m_clothLinks)
	{
		// Check if the link is active
		if (link->IsActiveLink())
		{
			int pointIndex_1 = link->GetPoint_1()->GetIndex();
			int pointIndex_2 = link->GetPoint_2()->GetIndex();

			m_indiceBuffer.push_back(pointIndex_1);
			m_indiceBuffer.push_back(pointIndex_2);
		}
	}

	// Calculate the indice count
	m_indiceCount = m_indiceBuffer.size();
}

void CClothComponent::CreateLinks()
{
	for (int row = 0; row < m_height; ++row)
	{
		for (int col = 0; col < m_width; ++col)
		{
			// Calculate constrain for the point if it is active
			int selfIndex = row * m_width + col;

			// Creating Horizontal and Verticle link
			// Calculate constrain for the point with the point to the right
			if (IndexInBound(row, col + 1))
			{
				int rightIndex = row * m_width + col + 1;
				CreateLink(m_clothPoints[selfIndex], m_clothPoints[rightIndex], m_restDistance);
			}
			// Calculate constrain for the point with the point below
			if (IndexInBound(row + 1, col))
			{
				int downIndex = (row + 1) * m_width + col;
				CreateLink(m_clothPoints[selfIndex], m_clothPoints[downIndex], m_restDistance);
			}

			// Create Diagonal link
			float diagonalRestDistance = std::sqrtf(std::powf(m_restDistance, 2) * 2);
			// Calculate constrain for the point with the point bottom right
			if (IndexInBound(row + 1, col + 1))
			{
				int bottomRightIndex = (row + 1) * m_width + (col + 1);
				CreateLink(m_clothPoints[selfIndex], m_clothPoints[bottomRightIndex], diagonalRestDistance);
			}
			// Calculate constrain for the point with the point bottom right
			if (IndexInBound(row + 1, col - 1))
			{
				int bottomLeftIndex = (row + 1) * m_width + (col - 1);
				CreateLink(m_clothPoints[selfIndex], m_clothPoints[bottomLeftIndex], diagonalRestDistance);
			}

			// Create Bending link
			float bendingLinkDistance = m_restDistance * 2.0f;
			// Calculate constrain for the point with the point to the rightx2
			if (IndexInBound(row, col + 2))
			{
				int rightIndex = row * m_width + col + 1;
				int rightIndexDouble = row * m_width + col + 2;
				CreateLink(m_clothPoints[selfIndex], m_clothPoints[rightIndexDouble],
					m_clothPoints[rightIndex], bendingLinkDistance);
			}
			// Calculate constrain for the point with the point belowx2
			if (IndexInBound(row + 2, col))
			{
				int downIndex = (row + 1) * m_width + col;
				int downIndexDouble = (row + 2) * m_width + col;
				CreateLink(m_clothPoints[selfIndex], m_clothPoints[downIndexDouble], 
					m_clothPoints[downIndex], bendingLinkDistance);
			}
		}
	}
}

void CClothComponent::CreateLink(CClothPoint* _point1, CClothPoint* _point2, float _restDistance)
{
	CClothLink* newLink = new CClothLink(_point1, _point2, this, _restDistance);

	m_clothLinks.push_back(newLink);
}

void CClothComponent::CreateLink(CClothPoint* _point1, CClothPoint* _point2, CClothPoint* _middlePoint, float _restDistance)
{
	CClothLink* newLink = new CClothLink(_point1, _point2, this, _restDistance, _middlePoint);

	m_clothLinks.push_back(newLink);
}

void CClothComponent::ConstraintPoints()
{
	for (const auto& link : m_clothLinks)
	{
		if (link->IsActiveLink())
		{
			link->ResolveLink();
		}
	}
// 	for (int row = 0; row < m_height; ++row)
// 	{
// 		for (int col = 0; col < m_width; ++col)
// 		{
// 			// Calculate constrain for the point if it is active
// 			int selfIndex = row * m_width + col;
// 			if (m_clothPoints[selfIndex]->m_bActive)
// 			{
// 				// Calculate constrain for the point with the point below if it is active
// 				int downIndex = (row + 1) * m_width + col;
// 				if (row < m_height - 1 && m_clothPoints[downIndex]->m_bActive)
// 				{
// 					ConstraintDistance(
// 						m_clothPoints[selfIndex], m_clothPoints[downIndex],
// 						m_restDistance, m_stiffness);
// 				}
// 
// 				// Calculate constrain for the point with the point below if it is active
// 				int rightIndex = row * m_width + col + 1;
// 				if (col < m_width - 1 && m_clothPoints[rightIndex]->m_bActive)
// 				{
// 					ConstraintDistance(
// 						m_clothPoints[selfIndex], m_clothPoints[rightIndex],
// 						m_restDistance, m_stiffness);
// 				}
// 			}
// 		}
// 	}
}

void CClothComponent::ConstraintDistance(CClothPoint* _point1, CClothPoint* _point2, float _restDistance, float _stiffness)
{
	glm::vec3 delta = _point1->m_localPosition - _point2->m_localPosition;

	float deltaLength = glm::length(delta);

	float difference = (deltaLength - _restDistance) / deltaLength;

	glm::vec3 correctionVec = delta * (1 - m_restDistance / deltaLength);
	glm::vec3 correctionVecHalf = correctionVec * 0.5f;

	float im1 = 1 / _point1->m_mass;
	float im2 = 1 / _point2->m_mass;

	glm::vec3 moveAmount_point1 = delta * (im1 / (im1 + im2)) * _stiffness * difference;
	glm::vec3 moveAmount_point2 = delta * (im2 / (im1 + im2)) * _stiffness * difference;

	//if (_point1->m_bMoveable) _point1->m_localPosition -= correctionVecHalf;
	//if (_point2->m_bMoveable) _point2->m_localPosition += correctionVecHalf;

	if (_point1->m_bMoveable)
	{
		_point1->m_localPosition -= correctionVecHalf;

		if (_point2->m_bMoveable)
		{
			_point2->m_localPosition += correctionVecHalf;
		}
		else
		{
			_point1->m_localPosition -= correctionVecHalf;
		}
	}
	else if (_point2->m_bMoveable)
	{
		_point2->m_localPosition += correctionVec;
	}
}

void CClothComponent::CheckGroundCollision()
{
	// Check if there is a ground to collide with
	if (!m_groundObj.expired())
	{
		glm::vec3 ownerWorldPosition = m_owner->m_transform.position;
		for (auto& point : m_clothPoints)
		{
			if (point->m_bActive)
			{
				// Get the position of the point in world position
				glm::vec3 pointWorldPosition = ownerWorldPosition + point->m_localPosition;

				// Get the y value of the ground
				float groundHeight =
					m_groundObj.lock()->m_transform.position.y
					+ m_groundObj.lock()->m_transform.scale.y;

				// Set the height value to the ground value
				if (pointWorldPosition.y < groundHeight)
				{
					pointWorldPosition.y = groundHeight;
				}

				// Convert the value back to the point's local position
				point->m_localPosition = pointWorldPosition - ownerWorldPosition;
			}
		}
	}
}

bool CClothComponent::IndexInBound(int _row, int _col)
{
	return
		_row >= 0 && _row <= m_height - 1
		&& _col >= 0 && _col <= m_width - 1;
}