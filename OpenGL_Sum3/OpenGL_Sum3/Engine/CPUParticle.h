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

	// Getter and setter
	glm::vec3 GetLocation() const;

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
	glm::vec3 m_originLocationl;
	glm::vec3 m_worldLocation;
	float m_totalLifeTime;
	float m_currentLifeTime;

};

