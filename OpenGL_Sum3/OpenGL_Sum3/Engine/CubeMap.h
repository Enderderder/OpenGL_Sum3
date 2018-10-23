#pragma once

// Global Include
#include "Utility.h"

// Forward Declare
class CMesh;
class CCamera;

class CCubeMap
{
public:
	CCubeMap() = default;
	CCubeMap(std::vector<std::string> _textureFilePaths);
	~CCubeMap();

	void RenderCubeMap(CCamera* _camera);

	GLuint GetTextureID() const;

private:
	
	GLuint m_vao;
	GLuint m_programID;
	GLuint m_textureID;

};