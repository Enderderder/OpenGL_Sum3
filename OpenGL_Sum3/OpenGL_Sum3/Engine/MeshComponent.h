#pragma once

// Derive Include
#include "Component.h"

// Forward Declare
class CMesh;
class CCamera;

class CMeshComponent : public CComponent
{
public:
	CMeshComponent();
	virtual ~CMeshComponent();

	void SetMesh(CMesh* _mesh);
	void SetTexture(GLuint _texture);
	void SetProgram(GLuint _program);

	void RenderMesh(CCamera* _camera);

private:

	CMesh* m_mesh;
	GLuint m_texture;
	GLuint m_program;

};

