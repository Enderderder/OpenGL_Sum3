
// This Include
#include "Sprite.h"

// Engine Include
#include "Camera.h"
#include "GameObject.h"
#include "AssetMgr.h"

CSprite::CSprite() {}
CSprite::~CSprite() {}

void CSprite::CreateSprite(const char* _filePath /**  , GLuint _programID */)
{
// 	GLuint program = CAssetMgr::GetInstance()->GetProgramID("DefaultSpriteProgram");
// 	glUseProgram(program);

	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_2D, m_tex);

	unsigned char* image = SOIL_load_image(
		_filePath,		// File Path
		&m_iWidth,		// Output for the image Width
		&m_iHeight,		// Output for the image Height
		0,				// Output for the number of channels
		SOIL_LOAD_RGBA	// Load RGBA values
	);

	glTexImage2D(
		GL_TEXTURE_2D,		// Type of texture
		0,					// Mipmap level, 0 for base
		GL_RGBA,			// Number of color components in texture
		m_iWidth,			// Width of the texture
		m_iHeight,			// Height of the texture
		0,					// This value must be 0
		GL_RGBA,			// Format for the pixel data
		GL_UNSIGNED_BYTE,	// Data type for the pixel data
		image				// Pointer to image data in memory
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D); 
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float halfWidth = (float)(m_iWidth / 2);
	float halfHeight = (float)(m_iHeight / 2);

	GLfloat vertices[] = {
		// Positions				   // Tex Coords
		-halfWidth,  halfHeight, 0.0f, 0.0f, 0.0f,
		 halfWidth,  halfHeight, 0.0f, 1.0f, 0.0f,
		 halfWidth, -halfHeight, 0.0f, 1.0f, 1.0f,
		-halfWidth, -halfHeight, 0.0f, 0.0f, 1.0f,
	}; // vertices to draw onto the screen

	GLuint indices[] = {
		0, 1, 2, // First Triangle
		0, 2, 3  // Second Triangle
	};

	/** Create VBO and EBO variable for binding */
	GLuint VBO, EBO;

	glGenVertexArrays(1, &m_vao); // Create VAO
	glBindVertexArray(m_vao);
	glGenBuffers(1, &VBO); // Create VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &EBO); // Create EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Enable Back Face Culling
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		5 * sizeof(GLfloat),
		(GLvoid*)0
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		5 * sizeof(GLfloat),
		(GLvoid*)(3 * sizeof(GLfloat))
	);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void CSprite::RenderSprite(Transform _transform, CCamera* _camera, GLuint _programID)
{
	glUseProgram(_programID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	glUniform1i(glGetUniformLocation(_programID, "tex"), 0);

	glm::vec3 position = _transform.position;
	glm::vec3 scale = _transform.scale;
	glm::vec3 rotation = _transform.rotation;


	glm::mat4 objTranslate = glm::translate(glm::mat4(), position);
	glm::mat4 objScale = glm::scale(glm::mat4(), scale);
	glm::mat4 objRotation = glm::rotate(glm::mat4(), glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
	objRotation = glm::rotate(objRotation, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
	objRotation = glm::rotate(objRotation, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 model = objTranslate * objRotation * objScale;
	
	glm::mat4 MVP = _camera->GetProj() * _camera->GetView() * model;

	GLint MVPLoc = glGetUniformLocation(_programID, "MVP");
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
