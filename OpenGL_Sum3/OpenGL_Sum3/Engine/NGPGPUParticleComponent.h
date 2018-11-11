#pragma once

// Inherited Include
#include "Component.h"

// Forward Declare
class CNGPGPUParticle;
class CCamera;

class CNGPGPUParticleComponent : public CComponent
{
public:

	CNGPGPUParticleComponent();
	virtual ~CNGPGPUParticleComponent();

	virtual void BeginPlay() override;
	virtual void Update() override;

	void Render(CCamera* _camera);

	void SetTexture(std::string _texureName);


private:

	void GenerateRenderData();
	void RefreshRenderData();

public:

	// Data
	int m_particleCount;
	float m_lifeTime;
	int m_particlesPerSecond;
	float m_startVelocity;
	bool m_isLooping;
	bool m_isPlaying;

protected:

	std::vector<CNGPGPUParticle*> m_particles;

	// Render data
	std::vector<glm::vec3> m_vertices;
	GLuint m_vao, m_vbo, m_texture, m_program;
};
