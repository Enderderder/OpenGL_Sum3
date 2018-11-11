
// This Include
#include "Engine.h"

CNGPGPUParticleComponent::CNGPGPUParticleComponent()
{
	m_particleCount = 10000;
	m_lifeTime = 1.0f;
	m_particlesPerSecond = 100;
	m_startVelocity = 1.0f;
	m_isLooping = true;
	m_isPlaying = true;
}

CNGPGPUParticleComponent::~CNGPGPUParticleComponent()
{
	for (auto* particle : m_particles)
	{
		delete particle;
		particle = nullptr;
	}
	m_particles.clear();
}

void CNGPGPUParticleComponent::BeginPlay()
{
	__super::BeginPlay();

	GenerateRenderData();
}

void CNGPGPUParticleComponent::Update()
{
	__super::Update();
	
	// Get the delta time for later use
	float deltaTime = CTime::GetInstance()->GetDeltaTime();

	/// Here will be the code that spawn the particle with time 
	///
	///
	/// =======================================================

	if (m_isPlaying)
	{
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

void CNGPGPUParticleComponent::Render(CCamera* _camera)
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
	glUniform1i(glGetUniformLocation(m_program, "Texture"), 0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

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

void CNGPGPUParticleComponent::SetTexture(std::string _texureName)
{
	GLuint texture = CAssetMgr::GetInstance()->GetTexture(_texureName);
	if (texture != NULL)
	{
		m_texture = texture;
	}
}

void CNGPGPUParticleComponent::GenerateRenderData()
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(
		GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vertices.size(), 
		&m_vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE,
		3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CNGPGPUParticleComponent::RefreshRenderData()
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
