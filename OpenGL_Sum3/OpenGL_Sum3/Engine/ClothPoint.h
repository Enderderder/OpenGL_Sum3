#pragma once

// GLobal Include
#include "Utility.h"

class CClothPoint
{
public:

	CClothPoint(glm::vec3 _localPosition, int _index);
	~CClothPoint();

	void Update(float _deltaTime);
	void ApplyForce(glm::vec3 _force);

	int GetIndex() const;

private:

	void ApplyGravityForce();

public:

	int m_index;
	glm::vec3 m_localPosition;
	glm::vec3 m_oldLocalPosition;
	glm::vec3 m_acceleration;
	float m_damping;
	float m_mass;
	bool m_bMoveable;
	bool m_bActive;

};