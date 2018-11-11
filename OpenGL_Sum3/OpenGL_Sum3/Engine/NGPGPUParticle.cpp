
// This Include
#include "Engine.h"

CNGPGPUParticle::CNGPGPUParticle(CNGPGPUParticleComponent* _owner)
{
	m_ownerComponent = _owner;
	InitializeData();
}

CNGPGPUParticle::~CNGPGPUParticle()
{
	m_ownerComponent = nullptr;
}

void CNGPGPUParticle::InitializeData()
{
	// Safe check
	if (m_ownerComponent == nullptr)
	{
		CDebug::Log("Error - This NGPGPUParticle does not have a parent component.");
		return;
	}

	// Set the init data
	m_shouldDestroy = false;

	this->m_originLocationl = m_ownerComponent->GetOwner()->m_transform.position;
	m_worldLocation = m_originLocationl;

	this->m_totalLifeTime = m_ownerComponent->m_lifeTime;
	m_currentLifeTime = m_totalLifeTime;

	glm::vec3 m_startVelocity =
		m_ownerComponent->GetOwner()->m_transform.GetForward() * m_ownerComponent->m_startVelocity;

}

void CNGPGPUParticle::Update(float _deltaTime)
{
	ApplyGravity(_deltaTime);

	m_worldLocation += m_startVelocity * _deltaTime;

	CountDownLife(_deltaTime);
}

glm::vec3 CNGPGPUParticle::GetLocation() const
{
	return m_worldLocation;
}

void CNGPGPUParticle::CountDownLife(float _deltaTime)
{
	m_currentLifeTime -= _deltaTime;
	if (m_currentLifeTime <= 0.0f)
	{
		m_shouldDestroy = true;
	}
}

void CNGPGPUParticle::ApplyGravity(float _deltaTime)
{
	m_worldLocation += glm::vec3(0.0f, -9.81f, 0.0f) * _deltaTime;
}

