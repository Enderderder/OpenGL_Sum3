#pragma once

// Inherited Include
#include "Component.h"

// Forward Declare
class CCPUParticle;
class CCamera;

class CCPUParticleComponent : public CComponent
{
public:

	CCPUParticleComponent();
	virtual ~CCPUParticleComponent();

	virtual void BeginPlay() override;
	virtual void Update() override;

	void Render(CCamera* _camera);

	void SetTexture(std::string _texureName);


private:

	void GenerateRenderData();
	void RefreshRenderData();
	void DestroyParticle(CCPUParticle* particle);

public:

	// Data
	unsigned int m_particleCount;
	float m_lifeTime;
	int m_particlesPerSecond;
	glm::vec3 m_startVelocity;
	bool m_isLooping;
	bool m_isPlaying;

protected:

	std::vector<CCPUParticle*> m_particles;

	// Render data
	std::vector<glm::vec3> m_vertices;
	GLuint m_vao, m_vbo, m_texture, m_program;
};
