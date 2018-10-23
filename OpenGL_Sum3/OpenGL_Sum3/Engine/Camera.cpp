
// This Include
#include "Camera.h"

// Engine Include
#include "Input.h"

// Static variable
static CInput* p_Input = CInput::GetInstance();

CCamera::CCamera()
{
	m_cameraPosition = glm::vec3(0.0f, 100.0f, 500.0f);
	m_cameraFacing = glm::vec3(0.0f, 0.0f, -1.0f);
	m_cameraNormal = glm::vec3(0.0f, 1.0f, 0.0f);
	m_bPerspective = true;
	m_fov = 45.0f;
	m_nearPlane = 0.1f;
	m_farPlane = 3000.0f;
	m_bIsControlling = true;
	m_viewPortWidth = util::SCR_WIDTH;
	m_viewPortHeight = util::SCR_HEIGHT;
}

CCamera::~CCamera()
{}

void CCamera::BeginPlay()
{
	CalcViewMatrix();
	CalcProjectionMatrix();
}

void CCamera::Update()
{
	if (m_bIsControlling)
	{
		glm::vec3 resultVec;

		if (p_Input->g_cKeyState[(unsigned char)'w'] == INPUT_HOLD || 
			p_Input->g_cKeyState[(unsigned char)'w'] == INPUT_FIRST_PRESS)
		{
			resultVec.z -= 2.0f;
		}
		if ((p_Input->g_cKeyState[(unsigned char)'s'] == INPUT_HOLD || 
			p_Input->g_cKeyState[(unsigned char)'s'] == INPUT_FIRST_PRESS))
		{
			resultVec.z += 2.0f;
		}
		if ((p_Input->g_cKeyState[(unsigned char)'a'] == INPUT_HOLD ||
			p_Input->g_cKeyState[(unsigned char)'a'] == INPUT_FIRST_PRESS))
		{
			resultVec.x -= 2.0f;
		}
		if ((p_Input->g_cKeyState[(unsigned char)'d'] == INPUT_HOLD || 
			p_Input->g_cKeyState[(unsigned char)'d'] == INPUT_FIRST_PRESS))
		{
			resultVec.x += 2.0f;
		}
		if ((p_Input->g_cKeyState[(unsigned char)'r'] == INPUT_HOLD ||
			p_Input->g_cKeyState[(unsigned char)'r'] == INPUT_FIRST_PRESS))
		{
			resultVec.y += 2.0f;
		}
		if ((p_Input->g_cKeyState[(unsigned char)'f'] == INPUT_HOLD ||
			p_Input->g_cKeyState[(unsigned char)'f'] == INPUT_FIRST_PRESS))
		{
			resultVec.y -= 2.0f;
		}

		if (resultVec != glm::vec3())
		{
			// Add the speed force to the direction
			this->m_cameraPosition += resultVec;

			//this->m_velocity = glm::normalize(resultVec) * m_movementSpd;
		}
	}


	// Re-calculate the projection and view matrix
	CalcViewMatrix();
	CalcProjectionMatrix();
}

glm::mat4 CCamera::GetView() const
{
	return(m_viewMatrix);
}

void CCamera::CalcViewMatrix()
{
	m_viewMatrix = glm::lookAt(
		m_cameraPosition,
		glm::vec3(0.0, 0.0, 0.0),
		//m_cameraPosition + m_cameraFacing,
		m_cameraNormal);
}

glm::mat4 CCamera::GetProj() const
{
	return m_projectionMatrix;
}

void CCamera::CalcProjectionMatrix()
{
	if (m_bPerspective)
	{
		m_projectionMatrix = glm::perspective(m_fov,
			m_viewPortWidth / m_viewPortHeight, m_nearPlane, m_farPlane);
	} 
	else
	{
		m_projectionMatrix = glm::ortho(-(m_viewPortWidth / 2), (m_viewPortWidth / 2),
			-(m_viewPortHeight / 2), (m_viewPortHeight / 2), m_nearPlane, m_farPlane);
	}
}

glm::vec3 CCamera::GetCameraFacing() const
{
	return m_cameraFacing;
}
void CCamera::SetCameraFacing(glm::vec3 _facing)
{
	m_cameraFacing = _facing;
}

glm::vec3 CCamera::GetCameraNormal() const
{
	return m_cameraNormal;
}
void CCamera::SetCameraNormal(glm::vec3 _normal)
{
	m_cameraNormal = _normal;
}