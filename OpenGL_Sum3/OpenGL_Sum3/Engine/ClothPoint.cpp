
// Engine Include
#include "Engine.h"

CClothPoint::CClothPoint(glm::vec3 _localPosition)
{
	m_localPosition = _localPosition;
	m_acceleration = { 0.0f, 0.0f, 0.0f };
	m_oldLocalPosition = _localPosition;
	m_mass = 1.0f;
	m_damping = 0.5f;
	m_bMoveable = false;
}

CClothPoint::~CClothPoint() {}

void CClothPoint::Update(float _deltaTime)
{
	if (m_bMoveable) // If the point is not a fixed point
	{
		// Apply some gravity
		ApplyGravityForce();

		// Update the position of the point with real time tick
		glm::vec3 storeOldPosition = m_localPosition;
		m_localPosition =
			m_localPosition + (m_localPosition - m_oldLocalPosition) * (1.0f - m_damping) + m_acceleration * (std::powf(_deltaTime, 2));
		m_oldLocalPosition = storeOldPosition;
	}

}

void CClothPoint::ApplyForce(glm::vec3 _force)
{
	m_acceleration += _force / m_mass;
}

void CClothPoint::ApplyGravityForce()
{
	this->ApplyForce({ 0.0f, m_mass * 9.81f, 0.0f });
}
