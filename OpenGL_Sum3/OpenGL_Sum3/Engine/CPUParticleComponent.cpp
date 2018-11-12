
// This Include
#include "Engine.h"

CCPUParticleComponent::CCPUParticleComponent()
{
	m_particleCount = 10000;
	m_lifeTime = 1.0f;
	m_particlesPerSecond = 100;
	m_startVelocity = 1.0f;
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
		if (m_particles.size() < m_particleCount)
		{
			for (int i = 0; i < glm::floor(m_particlesPerSecond * deltaTime); ++i)
			{
				glm::vec3 spawnPosition = glm::vec3();
				spawnPosition.x = rand() % 100 + (-100);
				spawnPosition.z = rand() % 100 + (-100);

				// Spawn the particle
				CCPUParticle* particle = new CCPUParticle(this, spawnPosition, glm::vec3());
				m_particles.push_back(particle);
			}
		}

		// Update the position of the particles with deltaTime
		for (auto* particle : m_particles)
		{
			particle->Update(deltaTime);
		}
	}

	// Refresh the render datat at the end
	// of the update and prepare for the render
	RefreshRenderData();
}

void CCPUParticleComponent::Render(CCamera* _camera)
{
	// Calculate the bilboad
	glm::mat4 viewMat = _camera->GetView();
	glm::vec3 viewVec = glm::normalize(_camera->m_transform.GetForward());

	glm::vec3 quadVec_1, quadVec_2;
	quadVec_1 = glm::cross(viewVec, _camera->m_transform.GetUp());
	quadVec_1 = glm::normalize(quadVec_1);
	quadVec_2 = glm::cross(viewVec, quadVec_1);
	quadVec_2 = glm::normalize(quadVec_2);

	// Start using GPU to render
	glUseProgram(m_program);

	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set variable for the shader
	glUniform3f(glGetUniformLocation(m_program, "quadVec_1"), quadVec_1.x, quadVec_1.y, quadVec_1.z);
	glUniform3f(glGetUniformLocation(m_program, "quadVec_2"), quadVec_2.x, quadVec_2.y, quadVec_2.z);
	glUniformMatrix4fv(glGetUniformLocation(m_program, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(glGetUniformLocation(m_program, "texture"), 0);

	// Bind render data
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(
		GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vertices.size(), 
		&m_vertices[0], GL_DYNAMIC_DRAW);

	glBindVertexArray(m_vao);
	glDrawArrays(GL_POINTS, 0, m_vertices.size());

	// Unbind
	glBindVertexArray(0);
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
// 	glBufferData(
// 		GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vertices.size(), 
// 		&m_vertices[0], GL_STATIC_DRAW);

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
	m_vertices.resize(m_particles.size());
	for (size_t index = 0; index < m_particles.size(); ++index)
	{
		glm::vec3 position = m_particles[index]->GetLocation();
		m_vertices[index] = position;
	}
}
