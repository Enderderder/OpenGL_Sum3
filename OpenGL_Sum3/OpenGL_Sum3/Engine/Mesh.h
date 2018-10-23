
#pragma once

// Global Include
#include "Utility.h"

class CMesh
{
public:
	CMesh() {};
	~CMesh() {};

	virtual void RenderMesh() = 0;

protected:

	GLuint m_vao;
};

