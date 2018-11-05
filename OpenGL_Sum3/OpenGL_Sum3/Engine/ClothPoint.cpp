
// This Include
#include "ClothPoint.h"

CClothPoint::CClothPoint(glm::vec3 _localPosition, int _index)
{
	m_index = _index;
	m_localPosition = _localPosition;
	m_acceleration = { 0.0f, 0.0f, 0.0f };
	m_oldLocalPosition = _localPosition;
	m_mass = 0.2f;
	m_damping = 0.1f;
	m_bMoveable = true;
	m_bActive = true;
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
		glm::vec3 velocity = (m_localPosition - m_oldLocalPosition) * (1.0f - m_damping) + m_acceleration * _deltaTime;
		m_localPosition += velocity;
		//	m_localPosition + (m_localPosition - m_oldLocalPosition) * (1.0f - m_damping) + m_acceleration * (std::powf(_deltaTime, 1));
		m_oldLocalPosition = storeOldPosition;

		// Reset the acceleration since the 
		m_acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	}

}

void CClothPoint::ApplyForce(glm::vec3 _force)
{
	m_acceleration = _force / m_mass;
}

int CClothPoint::GetIndex() const
{
	return m_index;
}

void CClothPoint::ApplyGravityForce()
{
	this->ApplyForce({ 0.0f, m_mass * -1.81f, 0.0f });
}