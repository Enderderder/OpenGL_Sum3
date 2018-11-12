
// This Include
#include "Engine.h"

CCPUParticle::CCPUParticle(CCPUParticleComponent* _owner, glm::vec3 _spawnLocation, glm::vec3 _initVelocity)
{
	m_ownerComponent = _owner;
	InitializeData(_spawnLocation, _initVelocity);
}

CCPUParticle::~CCPUParticle()
{
	m_ownerComponent = nullptr;
}

void CCPUParticle::InitializeData(glm::vec3 _spawnLocation, glm::vec3 _initVelocity)
{
	// Safe check
	if (m_ownerComponent == nullptr)
	{
		CDebug::Log("Error - This CPUParticle does not have a parent component.");
		return;
	}

	// Set the init data
	m_shouldDestroy = false;

	this->m_originLocationl = _spawnLocation;

	this->m_totalLifeTime = m_ownerComponent->m_lifeTime;
	m_currentLifeTime = m_totalLifeTime;

	glm::vec3 m_startVelocity =
		m_ownerComponent->GetOwner()->m_transform.GetForward() * m_ownerComponent->m_startVelocity;

}

void CCPUParticle::Update(float _deltaTime)
{
	ApplyGravity(_deltaTime);

	m_worldLocation += m_startVelocity * _deltaTime;

	CountDownLife(_deltaTime);
}

glm::vec3 CCPUParticle::GetLocation() const
{
	return m_worldLocation;
}

void CCPUParticle::CountDownLife(float _deltaTime)
{
	m_currentLifeTime -= _deltaTime;
	if (m_currentLifeTime <= 0.0f)
	{
		m_shouldDestroy = true;
	}
}

void CCPUParticle::ApplyGravity(float _deltaTime)
{
	m_worldLocation += glm::vec3(0.0f, -9.81f, 0.0f) * _deltaTime;
}

