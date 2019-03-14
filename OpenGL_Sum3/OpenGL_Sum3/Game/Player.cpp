
// Game Classes Include
#include "GameClasses.h"

// Engine Include
#include "Engine/Engine.h"
#include "Engine/ssAnimatedModel.h"

CPlayer::CPlayer()
{
	m_speed = 0.5f;
	m_isJumping = false;
	m_hasReachHeight = false;
	m_jumpHeight = 100.0f;

	m_animComponent = CreateComponent<CAnimationComponent>();
	
}

CPlayer::~CPlayer()
{}

void CPlayer::BeginPlay()
{
	__super::BeginPlay();
	
	GLuint animProgram = CAssetMgr::Get()->GetProgramID("AnimationProgram");
	m_animComponent->SetModel("Resources/Models/theDude.dae", animProgram, m_scene->GetMainCamera());
	m_animComponent->GetModel()->setScale(glm::vec3(0.1f));
	m_animComponent->GetModel()->setCurrentAnimation(0, 30);
	m_animState = IDLE;
}

void CPlayer::Update(float _deltaTime)
{
	__super::Update(_deltaTime);
	
	// Process input
	CInput* inputModule = CInput::Get();
	glm::vec3 resultVec = glm::vec3();
	if (inputModule->g_cKeyState[unsigned char('i')] == INPUT_HOLD)
	{
		resultVec.z += m_speed;
	}
	if (inputModule->g_cKeyState[unsigned char('k')] == INPUT_HOLD)
	{
		resultVec.z -= m_speed;
	}
	if (inputModule->g_cKeyState[unsigned char('j')] == INPUT_HOLD)
	{
		resultVec.x += m_speed;
	}
	if (inputModule->g_cKeyState[unsigned char('l')] == INPUT_HOLD)
	{
		resultVec.x -= m_speed;
	}
	if (inputModule->g_cKeyState[32] == INPUT_FIRST_PRESS && !m_isJumping)
	{
		m_isJumping = true;
		jumpvelo = { 0.0f, 2.0f, 0.0f };
	}

	// Process jump b4 running
	//ProcessJump();
	if (m_isJumping)
	{
		// Set the animation
		if (m_animState != JUMPING)
		{
			m_animComponent->GetModel()->setCurrentAnimation(71, 80);
			m_animState = JUMPING;
		}

		float x = this->m_transform.position.x;
		float z = this->m_transform.position.z;
		float groundHeight = m_linkedTerrain->GetHeight(x, z);

		m_transform.position += jumpvelo;
		jumpvelo.y -= 9.8f * _deltaTime;
		if (m_transform.position.y <= groundHeight)
		{
			m_transform.position.y = groundHeight;
			m_isJumping = false;
			m_animComponent->GetModel()->setCurrentAnimation(0, 30);
			m_animState = IDLE;
		}
	}

	if (resultVec != glm::vec3())
	{
		if (resultVec.z >= 0.0f)
		{
			// Set the animation
			if (m_animState != RUNNING_FORWARD && !m_isJumping)
			{
				m_animComponent->GetModel()->setCurrentAnimation(31, 50);
				m_animState = RUNNING_FORWARD;
			}
		}
		else
		{
			// Set the animation
			if (m_animState != RUNNING_BACKWARD && !m_isJumping)
			{
				m_animComponent->GetModel()->setCurrentAnimation(51, 70);
				m_animState = RUNNING_BACKWARD;
			}
		}

		// Translate the character
		this->m_transform.position += resultVec;
	}
	else
	{
		// If not running, set the animation to idle
		if (m_animState != IDLE && !m_isJumping)
		{
			m_animComponent->GetModel()->setCurrentAnimation(0, 30);
			m_animState = IDLE;
		}
	}

	if (!m_isJumping)
	{
		StandOnTerrain();
	}
	SyncTransform();
}

void CPlayer::LinkTerrain(std::shared_ptr<CGameObject> _terrain)
{
	m_linkedTerrain = _terrain->GetComponent<CTerrainComponent>();
}

void CPlayer::StandOnTerrain()
{
	float x = this->m_transform.position.x;
	float z = this->m_transform.position.z;

	this->m_transform.position.y = m_linkedTerrain->GetHeight(x, z);
}

void CPlayer::SyncTransform()
{
	m_animComponent->GetModel()->setPosition(this->m_transform.position);
	m_animComponent->GetModel()->setRotation(this->m_transform.rotation);
}

void CPlayer::ProcessJump()
{
	// Set the animation jumping if it is not already
	if (m_isJumping && m_animState != JUMPING)
	{
		m_animComponent->GetModel()->setCurrentAnimation(71, 80);
		m_animState = JUMPING;
	}

	float x = this->m_transform.position.x;
	float z = this->m_transform.position.z;
	float groundHeight = m_linkedTerrain->GetHeight(x, z);

	if (this->m_transform.position.y <= (groundHeight + m_jumpHeight) && m_hasReachHeight == false)
	{
		this->m_transform.position.y += 0.1f;
	}
	else if (this->m_transform.position.y >= (groundHeight))
	{
		this->m_transform.position.y -= 0.1f;
	}

	if (this->m_transform.position.y >= (groundHeight + m_jumpHeight))
	{
		m_hasReachHeight = true;
	}

	if (this->m_transform.position.y <= groundHeight)
	{
		m_hasReachHeight = false;
		m_isJumping = false;
	}

}
