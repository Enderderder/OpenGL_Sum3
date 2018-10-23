
// This Include
#include "CubeMap.h"

// Engine Include
#include "AssetMgr.h"
#include "Camera.h"

CCubeMap::CCubeMap(std::vector<std::string> _textureFilePaths)
{
	GLfloat cubeVertices[] = {

		// Right Face
		 1.0f,  1.0f,  1.0f,	// 0
		 1.0f,  1.0f, -1.0f,	// 1
		 1.0f, -1.0f, -1.0f,	// 2
		 1.0f, -1.0f,  1.0f,	// 3

		// Left Face
		-1.0f,  1.0f, -1.0f,	// 4
		-1.0f,  1.0f,  1.0f,	// 5
		-1.0f, -1.0f,  1.0f,	// 6
		-1.0f, -1.0f, -1.0f,	// 7

		// Top Face
		-1.0f,  1.0f, -1.0f,	// 8
		 1.0f,  1.0f, -1.0f,    // 9
		 1.0f,  1.0f,  1.0f,    // 10
		-1.0f,  1.0f,  1.0f,    // 11

		// Bottom Face
		-1.0f, -1.0f,  1.0f,    // 12
		 1.0f, -1.0f,  1.0f,    // 13
		 1.0f, -1.0f, -1.0f,    // 14
		-1.0f, -1.0f, -1.0f,    // 15

		// Back Face
		 1.0f,  1.0f, -1.0f,    // 16
		-1.0f,  1.0f, -1.0f,    // 17
		-1.0f, -1.0f, -1.0f,	// 18
		 1.0f, -1.0f, -1.0f,    // 19

		// Front Face
		-1.0f,  1.0f,  1.0f,	// 20
		 1.0f,  1.0f,  1.0f,	// 21
		 1.0f, -1.0f,  1.0f,	// 22
		-1.0f, -1.0f,  1.0f,	// 23
	};

	GLuint cubeIndices[] = {

		// Right Face	// Bottom Face
		0, 1, 2,		12, 13, 14,
		0, 2, 3,		12, 14, 15,

		// Left Face	// Back Face
		4, 5, 6,		16, 17, 18,
		4, 6, 7,		16, 18, 19,

		// Top Face		// Front Face
		8, 9, 10,		20, 21, 22,
		8, 10, 11,		20, 22, 23,
	};

	// Set the shader program
	m_programID = CAssetMgr::GetInstance()->GetProgramID("CubeMapProgram");

	// Bind each image into the cube map
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

	int width, height;
	unsigned char* image;
	for (GLuint i = 0; i < 6; ++i)
	{
		std::string fullPathName = "Resources/Textures/CubeMaps/";
		fullPathName.append(_textureFilePaths[i]);

		image = SOIL_load_image(fullPathName.c_str(),
			&width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
			width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Free memory and unbind the texture type
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// Bind VBO and EBO and store the VAO
	GLuint VBO, EBO;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
}

CCubeMap::~CCubeMap()
{}

void CCubeMap::RenderCubeMap(CCamera* _camera)
{
	// Binding and settings
	glUseProgram(m_programID);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
	glUniform1i(glGetUniformLocation(m_programID, "cubeSampler"), 0);

	glm::mat4 model = glm::scale(glm::mat4(), glm::vec3(1000.0f, 1000.0f, 1000.0f));
	glm::mat4 MVP = _camera->GetProj() * _camera->GetView() * model;
	glUniformMatrix4fv(glGetUniformLocation(m_programID, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

	// Bind the VAO and draw the cube map
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Clean up after render
	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

GLuint CCubeMap::GetTextureID() const
{
	return m_textureID;
}