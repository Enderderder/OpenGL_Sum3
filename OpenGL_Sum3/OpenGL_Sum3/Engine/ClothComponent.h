#pragma once

// Inherite Include
#include "Component.h"

// Forward Declare
class CCamera;
class CClothPoint;
class CClothLink;
class CGameObject;

class CClothComponent : public CComponent
{
public:

	CClothComponent();
	virtual ~CClothComponent();

	virtual	void BeginPlay() override;
	virtual void Update() override;

	void RenderColth(CCamera* _camera);

	/**
	 * Apply certain force to a specific point inside the cloth
	 */
	void ApplyForceToPoint(int _row, int _col, glm::vec3 _force);
	/**
	 * Release all the pin point of the cloth
	 */
	void ReleaseEntireCloth();
	/**
	 * Resize the cloth
	 */
	void ResizeCloth(int _width, int _height);
	/**
	 * Reset the cloth simulation by rebind everything
	 */
	void RebuildCloth();
	/**
	 * Set the ground object for the cloth
	 */
	void SetGroundObject(std::shared_ptr<CGameObject> _groundObj);

private:

	/**
	 * Create the cloth base on width and height
	 */
	void BuildCloth();

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
	void CreateLink(CClothPoint* _point1,
		CClothPoint* _point2,
		CClothPoint* _middlePoint,
		float _restDistance);

	/**
	 * Cloth constraints and physics
	 */
	void ConstraintPoints();
	void ConstraintDistance(
		CClothPoint* _point1, CClothPoint* _point2, 
		float _restDistance, float _stiffness);

	void CheckGroundCollision();

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

	// Ground object for collision
	std::weak_ptr<CGameObject> m_groundObj;

private:

	// Render Data
	GLuint m_vao, m_vbo, m_ebo;
	GLuint m_program;
	GLuint m_indiceCount;
	std::vector<float> m_vertexBuffer;
	std::vector<GLuint> m_indiceBuffer;
};
