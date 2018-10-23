
// This Include 
#include "MeshComponent.h"

// Engine Include
#include "Camera.h"
#include "GameObject.h"
#include "Mesh.h"

CMeshComponent::CMeshComponent() {}

CMeshComponent::~CMeshComponent() {}

void CMeshComponent::SetMesh(CMesh* _mesh)
{
	m_mesh = _mesh;
}

void CMeshComponent::SetTexture(GLuint _texture)
{
	m_texture = _texture;
}

void CMeshComponent::SetProgram(GLuint _program)
{
	m_program = _program;
}

void CMeshComponent::RenderMesh(CCamera* _camera)
{
	glUseProgram(m_program);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	/** Get the translate scale rotation from the game object transform */
	glm::vec3 objPos = this->GetOwner()->m_transform.position;
	glm::vec3 objRotate =this->GetOwner()->m_transform.rotation;
	glm::vec3 objScale = this->GetOwner()->m_transform.scale;

	/** Calculate the MVP matrix from the game object transform */
	glm::mat4 translate = glm::translate(glm::mat4(), objPos);
    glm::mat4 scale = glm::scale(glm::mat4(), objScale);
	glm::mat4 rotation = glm::mat4();
	rotation = glm::rotate(rotation, glm::radians(objRotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(objRotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(objRotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 model = translate * rotation * scale;
	glm::mat4 mvp = _camera->GetProj() *  _camera->GetView() * model;
	GLint mvpLoc = glGetUniformLocation(m_program, "MVP");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	/** Other shader variables */
	
	GLint modelLoc = glGetUniformLocation(m_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat3 normalMat = glm::mat3(transpose(inverse(model)));
	GLuint normalMatLoc = glGetUniformLocation(m_program, "normalMat");
	glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, glm::value_ptr(normalMat));

	GLuint camPosLoc = glGetUniformLocation(m_program, "camPos");
	glUniform3fv(camPosLoc, 1, glm::value_ptr(_camera->m_cameraPosition));

	/************************************************************************/

	/** bind the texture for the mesh */
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(glGetUniformLocation(m_program, "tex"), 0);

	/** Render the mesh */
	m_mesh->RenderMesh();
}
