
// This Include
#include "Engine.h"

CCPUParticleComponent::CCPUParticleComponent()
{
	m_particleCount = 15000;
	m_lifeTime = 0.5f;
	m_particlesPerSecond = 7000;
	m_spawnRadius = 100.0f;
	m_minStartVelocity = glm::vec3();
	m_minStartVelocity = glm::vec3();
	m_isLooping = true;
	m_isPlaying = true;
}

CCPUParticleComponent::~CCPUParticleComponent()
{
	for (auto* particle : m_particles)
	{
		delete particle;
		particle = nullptr;
	}
	m_particles.clear();
}

void CCPUParticleComponent::BeginPlay()
{
	__super::BeginPlay();

	GenerateRenderData();
}

void CCPUParticleComponent::Update()
{
	__super::Update();
	
	// Get the delta time for later use
	float deltaTime = CTime::GetInstance()->GetDeltaTime();

	if (m_isPlaying)
	{
		// Update the position of the particles with deltaTime
		for (auto* particle : m_particles)
		{
			// If the particle die, detroy. Otherwise update
			if (particle->ShouldDestroy() && m_isLooping) 
			{
				RespawnParitcle(particle); 
			}
			else { particle->Update(deltaTime); }
		}

		// Keep spawning particles if the current life particle hasn't reach max
		if (m_particles.size() < m_particleCount)
		{
			for (int i = 0; i < glm::floor(m_particlesPerSecond * deltaTime) + 1; ++i)
			{
				glm::vec3 spawnPosition = m_owner->m_transform.position;
				spawnPosition.x += util::RandomFloat(-m_spawnRadius, m_spawnRadius);
				spawnPosition.z += util::RandomFloat(-m_spawnRadius, m_spawnRadius);

				glm::vec3 initSpeed = glm::vec3();
				initSpeed.y = -util::RandomFloat(1500.0f, 1600.0f);

				// Spawn the particle
				CCPUParticle* particle = new CCPUParticle(this, spawnPosition, initSpeed);
				m_particles.push_back(particle);
			}
		}
	}

	// Refresh the render datat at the end
	// of the update and prepare for the render
	RefreshRenderData();
}

void CCPUParticleComponent::Render(CCamera* _camera)
{
	// GTFO if there is nothing to render
	if (m_vertices.empty())
	{
		return;
	}

	// Calculate the bilboad
	glm::mat4 viewProjMat = _camera->GetProj() * _camera->GetView();
	glm::vec3 viewVec = glm::normalize(_camera->m_transform.GetForward());

	glm::vec3 quadVec_1, quadVec_2;
	quadVec_1 = glm::cross(viewVec, _camera->m_transform.GetUp());
	quadVec_1 = glm::normalize(quadVec_1);
	quadVec_2 = glm::cross(viewVec, quadVec_1);
	quadVec_2 = glm::normalize(quadVec_2);

	// Start using GPU to render
	glUseProgram(m_program);

	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set variable for the shader
	glUniform3f(glGetUniformLocation(m_program, "quadVec_1"), quadVec_1.x, quadVec_1.y, quadVec_1.z);
	glUniform3f(glGetUniformLocation(m_program, "quadVec_2"), quadVec_2.x, quadVec_2.y, quadVec_2.z);
	glUniformMatrix4fv(glGetUniformLocation(m_program, "vp"), 1, GL_FALSE, glm::value_ptr(viewProjMat));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(glGetUniformLocation(m_program, "Texture"), 0);

	// Bind render data
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(
		GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vertices.size(), 
		&m_vertices[0], GL_STATIC_DRAW);

	glDrawArrays(GL_POINTS, 0, m_vertices.size());

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void CCPUParticleComponent::SetTexture(std::string _texureName)
{
	GLuint texture = CAssetMgr::GetInstance()->GetTexture(_texureName);
	if (texture != NULL)
	{
		m_texture = texture;
	}
}

void CCPUParticleComponent::GenerateRenderData()
{
	m_program = CAssetMgr::GetInstance()->GetProgramID("CPUParticleProgram");

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE,
		3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CCPUParticleComponent::RefreshRenderData()
{
	// Clear all the elements inside the vertices and reset the size
	m_vertices.clear();
	m_vertices.resize(0);
	//m_vertices.resize(m_particles.size());
	for (auto& particle : m_particles)
	{
		if (particle->ShouldDestroy() == false)
		{
			glm::vec3 position = particle->GetLocation();
			m_vertices.push_back(position);
		}
	}
}

void CCPUParticleComponent::RespawnParitcle(CCPUParticle* particle)
{
	glm::vec3 spawnPosition = m_owner->m_transform.position;
	spawnPosition.x += util::RandomFloat(-m_spawnRadius, m_spawnRadius);
	spawnPosition.z += util::RandomFloat(-m_spawnRadius, m_spawnRadius);

	glm::vec3 initSpeed = glm::vec3();
	initSpeed.y = -util::RandomFloat(1500.0f, 1600.0f);

	particle->ResetLocation(spawnPosition);
	particle->SetVelocity(initSpeed);
	particle->ResetLifeTime(m_lifeTime);
}

void CCPUParticleComponent::DestroyParticle(CCPUParticle* particle)
{
	for (auto iter = m_particles.begin(); iter != m_particles.end(); ++iter)
	{
		if ((*iter) == particle)
		{
			delete (*iter);
			m_particles.erase(iter);
			break;
		}
	}
}
