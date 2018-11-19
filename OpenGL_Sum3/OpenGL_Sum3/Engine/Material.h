#pragma once

// Global Inlcude
#include "Utility.h"

class CMaterial
{
public:

	CMaterial();
	virtual ~CMaterial();

	virtual void BindProgramData();


public:

	std::string m_name;

protected:

	GLuint m_shaderProgram;

};