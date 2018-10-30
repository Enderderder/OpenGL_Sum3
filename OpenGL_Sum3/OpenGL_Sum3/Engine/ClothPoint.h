#pragma once

// GLobal Include
#include "Utility.h"

class CClothPoint
{
public:

	CClothPoint(glm::vec3 _localPosition);
	~CClothPoint();

	void Update(float _deltaTime);
	void ApplyForce(glm::vec3 _force);

private:

	void ApplyGravityForce();

public:

	glm::vec3 m_localPosition;
	glm::vec3 m_oldLocalPosition;
	glm::vec3 m_acceleration;
	float m_damping;
	float m_mass;
	bool m_bMoveable;
	bool m_bActive;

};