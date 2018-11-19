#pragma once

// Global include
#include "Utility.h"

// Forward Declare

class CCPUParticle
{
public:

	CCPUParticle(CCPUParticleComponent* _owner, glm::vec3 _spawnLocation, glm::vec3 _initVelocity);
	virtual ~CCPUParticle();

	void InitializeData(glm::vec3 _spawnLocation, glm::vec3 _initVelocity);
	void Update(float _deltaTime);

	// Get if the particle should be destroyed
	bool ShouldDestroy() const;

	// Setter n Getter of location data
	void SetLocation(glm::vec3 _location);
	void ResetLocation(glm::vec3 _location);
	glm::vec3 GetLocation() const;

	// Setter n Getter of velocity data
	void SetVelocity(glm::vec3 _velocity);
	glm::vec3 GetVelocity() const;

	void ResetLifeTime(float _lifeTime);

private:

	void CountDownLife(float _deltaTime);
	void ApplyGravity(float _deltaTime);


private:

	// Component pointer
	CCPUParticleComponent* m_ownerComponent;

	// Destroy flag
	bool m_shouldDestroy;

	// Data
	glm::vec3 m_startVelocity;
	glm::vec3 m_originLocation;
	glm::vec3 m_worldLocation;
	float m_totalLifeTime;
	float m_currentLifeTime;

};

