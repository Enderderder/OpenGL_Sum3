
// This Include
#include "CubeMesh.h"

CCubeMesh::CCubeMesh()
{
	GLfloat cubeVertices[] = {
		// Positions			// Normals			// Tex Coords

		// Front Face
		-1.0f,  1.0f,  1.0f,   0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // 0
		 1.0f,  1.0f,  1.0f,   0.0f, 0.0f, 1.0f,	1.0f, 0.0f, // 1
		 1.0f, -1.0f,  1.0f,   0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // 2
		-1.0f, -1.0f,  1.0f,   0.0f, 0.0f, 1.0f,	0.0f, 1.0f, // 3

		// Right Face
		1.0f,  1.0f,  1.0f,   1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // 4
		1.0f,  1.0f, -1.0f,   1.0f, 0.0f, 0.0f,		1.0f, 0.0f, // 5
		1.0f, -1.0f, -1.0f,   1.0f, 0.0f, 0.0f,		1.0f, 1.0f, // 6
		1.0f, -1.0f,  1.0f,   1.0f, 0.0f, 0.0f,		0.0f, 1.0f, // 7

		// Back Face
		 1.0f,  1.0f, -1.0f,   0.0f, 0.0f, -1.0f,	0.0f, 0.0f, // 8
		-1.0f,  1.0f, -1.0f,   0.0f, 0.0f, -1.0f,	1.0f, 0.0f, // 9
		-1.0f, -1.0f, -1.0f,   0.0f, 0.0f, -1.0f,	1.0f, 1.0f, // 10
		 1.0f, -1.0f, -1.0f,   0.0f, 0.0f, -1.0f,	0.0f, 1.0f, // 11

		// Left Face
		-1.0f,  1.0f, -1.0f,  -1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // 12
		-1.0f,  1.0f,  1.0f,  -1.0f, 0.0f, 0.0f,	1.0f, 0.0f, // 13
		-1.0f, -1.0f,  1.0f,  -1.0f, 0.0f, 0.0f,	1.0f, 1.0f, // 14
		-1.0f, -1.0f, -1.0f,  -1.0f, 0.0f, 0.0f,	0.0f, 1.0f, // 15

		// Top Face
		-1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f,	0.0f, 0.0f, // 16
		 1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // 17
		 1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f,	1.0f, 1.0f, // 18
		-1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // 19

		// Bottom Face
		-1.0f, -1.0f,  1.0f,   0.0f, -1.0f, 0.0f,	0.0f, 0.0f, // 20
		 1.0f, -1.0f,  1.0f,   0.0f, -1.0f, 0.0f,	1.0f, 0.0f, // 21
		 1.0f, -1.0f, -1.0f,   0.0f, -1.0f, 0.0f,	1.0f, 1.0f, // 22
		-1.0f, -1.0f, -1.0f,   0.0f, -1.0f, 0.0f,	0.0f, 1.0f, // 23
	};

	GLuint cubeIndices[] = {

		// Front Face	// Left Face
		0, 1, 2,		12, 13, 14,
		0, 2, 3,		12, 14, 15,

		// Right Face	// Top Face
		4, 5, 6,		16, 17, 18,
		4, 6, 7,		16, 18, 19,

		// Back Face	// Bottom Face
		8, 9, 10,		20, 21, 22,
		8, 10, 11,		20, 22, 23,
	};

	// Bind the Vertices and Indices into the VAO
	GLuint VBO, EBO;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	// Bind attribute pointer for shader pipeline
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GL_FLOAT),
		(GLvoid*)0
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GL_FLOAT),
		(GLvoid*)(3 * sizeof(GL_FLOAT))
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),
		(GLvoid*)(6 * sizeof(GLfloat))
	);
	glEnableVertexAttribArray(2);

	// Unbind the VAO after generating
	glBindVertexArray(0);
}

CCubeMesh::~CCubeMesh() {}

void CCubeMesh::RenderMesh()
{
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

