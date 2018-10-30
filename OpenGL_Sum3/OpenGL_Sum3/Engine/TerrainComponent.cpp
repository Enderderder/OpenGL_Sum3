
// Engine Include
#include "Engine.h"

CTerrainComponent::CTerrainComponent()
{}

CTerrainComponent::~CTerrainComponent()
{}

void CTerrainComponent::BeginPlay()
{
	__super::BeginPlay();
	
}

void CTerrainComponent::Update()
{
	__super::Update();
	
}

void CTerrainComponent::LoadHeightMap()
{
	// Load the .tga image as a heightmap
	int width, height, channel;
	unsigned char* heightmapImage = SOIL_load_image(
		m_hmInfo.heightmapFilename,
		&width,
		&height,
		&channel,
		SOIL_LOAD_L);

	// Check if the heightmap loaded correctly
	if (heightmapImage == nullptr)
	{
		CDebug::Log("Fail to load the height map, please check file name and file type");
		return;
	}

	// Retrieve the width and height info
	m_hmInfo.width = width;
	m_hmInfo.height = height;

	// Load the height value into the vector
	m_heightMap.resize(width * height, 0);
	for (int i = 0; i < width * height; ++i)
	{
		m_heightMap[i] = heightmapImage[i] * m_hmInfo.heightScale + m_hmInfo.heightOffset;
	}
}

void CTerrainComponent::SmoothHeightMap()
{
	std::vector<float> dest(m_heightMap.size());

	for (unsigned int i = 0; i < m_hmInfo.width; ++i)
	{
		for (unsigned int j = 0; j < m_hmInfo.height; ++j)
		{
			dest[i * m_hmInfo.height + j] = Average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one.
	m_heightMap = dest;
}

void CTerrainComponent::CreateTerrain(HeightMapInfo& _info)
{
	// Pass in the information of the heightmap
	m_hmInfo = _info;

	// Load the height map into a vector that store the height of each point
	LoadHeightMap();

	// Smooth for 10 times to make it smooth af
	for (unsigned int i = 0; i < 10; ++i)
	{
		SmoothHeightMap();
	}

	/************************************************************************/

	// Initialize a vector of position vector with the size
	std::vector<float> vertex;
	float z;
	float x;
	float y;
	for (unsigned int row = 0; row < m_hmInfo.width; ++row)
	{
		for (unsigned int col = 0; col < m_hmInfo.height; ++col)
		{
			z = (float)row * m_hmInfo.cellSpacing;
			x = (float)col * m_hmInfo.cellSpacing;
			y = m_heightMap[(row * m_hmInfo.width) + col];

			// load each data into the vertices
			vertex.push_back(x);
			vertex.push_back(y);
			vertex.push_back(z);
		}
	}

	// Iterate over each quad and compute indices.
	std::vector<GLuint> indices((m_hmInfo.width - 1) * (m_hmInfo.height - 1) * 6);
	int k = 0;
	for (unsigned int row = 0; row < m_hmInfo.width - 1; ++row)
	{
		for (unsigned int col = 0; col < m_hmInfo.height - 1; ++col)
		{
			indices[k] = row * m_hmInfo.height + col;
			indices[k + 1] = row * m_hmInfo.height + col + 1;
			indices[k + 2] = (row + 1) * m_hmInfo.height + col;

			indices[k + 3] = (row + 1) * m_hmInfo.height + col;
			indices[k + 4] = row * m_hmInfo.height + col + 1;
			indices[k + 5] = (row + 1) * m_hmInfo.height + col + 1;

			k += 6; // next quad
		}
	}

	// Bind the program
	m_program = CAssetMgr::GetInstance()->GetProgramID("UnlitProgram");
	
	// Bind the information of the shape in to VAO
	GLuint VBO, EBO;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex.size(), &vertex[0], GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Unbind the VAO after generating
	glBindVertexArray(0);

	// Comput the indice size
	m_indiceCount = indices.size() * sizeof(GLuint) / sizeof(GLuint);
}

void CTerrainComponent::RenderTerrain(CCamera* _camera)
{
	glUseProgram(m_program);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
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
	glDrawElements(GL_TRIANGLES, m_indiceCount, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Unbind
	glBindVertexArray(0);
	glUseProgram(0);
}

float CTerrainComponent::GetHeight(float _x, float _z) const
{
	// Transform from terrain local space to "cell" space.
	float c = (_x) / m_hmInfo.cellSpacing;
	float d = (_z) / m_hmInfo.cellSpacing;

	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = m_heightMap[row * m_hmInfo.height + col];
	float B = m_heightMap[row * m_hmInfo.height + col + 1];
	float C = m_heightMap[(row + 1) * m_hmInfo.height + col];
	float D = m_heightMap[(row + 1) * m_hmInfo.height + col + 1];

	// Where we are relative to the cell.
	float s = c - (float)col;
	float t = d - (float)row;

	// If upper triangle ABC.
	if (s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s * uy + t * vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f - s)*uy + (1.0f - t)*vy;
	}
}

float CTerrainComponent::GetWidth() const
{
	return (m_hmInfo.height - 1) * m_hmInfo.cellSpacing;
}

float CTerrainComponent::GetDepth() const
{
	return (m_hmInfo.width - 1) * m_hmInfo.cellSpacing;
}

bool CTerrainComponent::InBounds(int i, int j)
{
	// True if ij are valid indices; false otherwise.
	return
		i >= 0 && i < (int)m_hmInfo.width &&
		j >= 0 && j < (int)m_hmInfo.height;
}

float CTerrainComponent::Average(int i, int j)
{
	// Function computes the average height of the ij element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float avg = 0.0f;
	float num = 0.0f;

	for (int m = i - 1; m <= i + 1; ++m)
	{
		for (int n = j - 1; n <= j + 1; ++n)
		{
			if (InBounds(m, n))
			{
				avg += m_heightMap[m * m_hmInfo.height + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}
