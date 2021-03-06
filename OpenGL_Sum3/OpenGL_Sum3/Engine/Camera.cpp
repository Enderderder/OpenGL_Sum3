
// Engine Include
#include "Engine.h"

// Static variable
static CInput* p_Input = CInput::Get();

CCamera::CCamera()
{
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

void CCamera::Update(float _deltaTime)
{
	if (m_bIsControlling)
	{
		float moveSpeed = 100.0f;
		glm::vec3 resultVec;

		if (p_Input->g_cKeyState[(unsigned char)'w'] == INPUT_HOLD)
		{
			resultVec += this->m_transform.GetForward() * moveSpeed * _deltaTime;
		}
		if (p_Input->g_cKeyState[(unsigned char)'s'] == INPUT_HOLD)
		{
			resultVec -= this->m_transform.GetForward() * moveSpeed * _deltaTime;
		}
		if (p_Input->g_cKeyState[(unsigned char)'a'] == INPUT_HOLD)
		{
			resultVec -= this->m_transform.GetRight() * moveSpeed * _deltaTime;
		}
		if (p_Input->g_cKeyState[(unsigned char)'d'] == INPUT_HOLD)
		{
			resultVec += this->m_transform.GetRight() * moveSpeed * _deltaTime;
		}
		if (p_Input->g_cKeyState[(unsigned char)'r'] == INPUT_HOLD)
		{
			resultVec.y += moveSpeed * _deltaTime;
		}
		if (p_Input->g_cKeyState[(unsigned char)'f'] == INPUT_HOLD)
		{
			resultVec.y -= moveSpeed * _deltaTime;
		}

		if (resultVec != glm::vec3())
		{
			// Add the speed force to the direction
			this->m_transform.position += resultVec;
		}


		// Rotate the camera
		float rotateSpeed = 80.0f;
		glm::vec3 resultRotation = glm::vec3();
		if (p_Input->g_cSpecialKey[GLUT_KEY_UP] == INPUT_HOLD)
		{
			resultRotation.x += rotateSpeed * _deltaTime;
		}
		if (p_Input->g_cSpecialKey[GLUT_KEY_DOWN] == INPUT_HOLD)
		{
			resultRotation.x -= rotateSpeed * _deltaTime;
		}
		if (p_Input->g_cSpecialKey[GLUT_KEY_LEFT] == INPUT_HOLD)
		{
			resultRotation.y -= rotateSpeed * _deltaTime;
		}
		if (p_Input->g_cSpecialKey[GLUT_KEY_RIGHT] == INPUT_HOLD)
		{
			resultRotation.y += rotateSpeed * _deltaTime;
		}

		if (resultRotation != glm::vec3())
		{
			// Add the speed force to the direction
			this->m_transform.rotation += resultRotation;
		}
	}

	// Re-calculate the projection and view matrix
	CalcViewMatrix();
	CalcProjectionMatrix();

	// Recalculate the camera ray direction base on the mouse position
	CalculateCameraRay();
}

glm::mat4 CCamera::GetView() const
{
	return(m_viewMatrix);
}

void CCamera::CalcViewMatrix()
{
	m_viewMatrix = glm::lookAt(
		m_transform.position,
		//{ 0.0f, 0.0f, 0.0f },
		m_transform.position + m_transform.GetForward(),
		//{ 0.0f, 1.0f, 0.0f }
		m_transform.GetUp()
	);
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

void CCamera::CalculateCameraRay()
{
	// Get the mouse ndc coord
	glm::vec2 normalizedScreenPos = p_Input->g_mouseNdcPosition;

	// Screen space to proj space
	glm::vec4 clipCoords = glm::vec4(
		normalizedScreenPos.x, normalizedScreenPos.y, -1.0f, 1.0f);

	// Proj space to eye space
	glm::mat4 invProjMat = glm::inverse(m_projectionMatrix);
	glm::vec4 eyeCoords = invProjMat * clipCoords;
	eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 1.0f);

	// Eye space to world space
	glm::mat4 invViewMat = glm::inverse(m_viewMatrix);
	glm::vec4 rayWorld = invViewMat * eyeCoords;
	m_cameraRay = glm::normalize(glm::vec3(rayWorld));
}
