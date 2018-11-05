#pragma once

// Inherite Include
#include "Component.h"

// Forward Declare
class CCamera;
class CClothPoint;
class CClothLink;

class CClothComponent : public CComponent
{
public:

	CClothComponent();
	virtual ~CClothComponent();

	virtual	void BeginPlay() override;
	virtual void Update() override;

	void RenderColth(CCamera* _camera);

private:

	/**
	 * Render data setup
	 */
	void SetupMesh();
	void RefreshMesh();
	void CalculateVertexData();
	void CalculateIndiceData();

	void CreateLinks();
	void CreateLink(
		CClothPoint* _point1,
		CClothPoint* _point2,
		float _restDistance);

	void ConstraintPoints();
	void ConstraintDistance(
		CClothPoint* _point1, CClothPoint* _point2, 
		float _restDistance, float _stiffness);

	bool IndexInBound(int _row, int _col);

public:

	// Configuration
	int m_width;
	int m_height;
	float m_restDistance;
	float m_stiffness;
	
	// Points stored
	std::vector<CClothPoint*> m_clothPoints;

	// Links between points stored
	std::vector<CClothLink*> m_clothLinks;

private:

	// Render Data
	GLuint m_vao, m_vbo, m_ebo;
	GLuint m_program;
	GLuint m_indiceCount;
	std::vector<float> m_vertexBuffer;
	std::vector<GLuint> m_indiceBuffer;
};
