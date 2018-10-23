#pragma once

// Inherited Include
#include "Mesh.h"

class CCubeMesh : public CMesh
{
public:

	CCubeMesh();
	~CCubeMesh();

	virtual void RenderMesh() override;

};