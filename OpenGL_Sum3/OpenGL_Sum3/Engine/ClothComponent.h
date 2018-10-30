#pragma once

// Inherite Include
#include "Component.h"

// Forward Declare
class CCamera;
class CClothPoint;

class CClothComponent : public CComponent
{
public:

	CClothComponent();
	virtual ~CClothComponent();

	virtual	void BeginPlay() override;
	virtual void Update() override;

	void RenderColth(CCamera* _camera);

private:

	bool IndexInBound(int _row, int _col);
	glm::vec3 LocalToWorld(glm::vec3 _localPos, glm::mat4 _mvp);
	void DrawLine(glm::vec3 _pos1, glm::vec3 _pos2);

public:

	int m_width = 10;
	int m_height = 10;

	float m_restDistance = 1.0f;
	
	std::vector<CClothPoint*> m_clothPoints;
};
