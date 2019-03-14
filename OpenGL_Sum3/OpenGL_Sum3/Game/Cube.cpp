
// Inlcude Game Classes
#include "GameClasses.h"

// Engine Include
#include "Engine/Engine.h"

CCube::CCube() 
{
	m_meshRenderer = CreateComponent<CMeshComponent>();

	m_moveSpd = 100.0f;
}

CCube::~CCube() {}

void CCube::BeginPlay()
{
	__super::BeginPlay();
	
	m_meshRenderer->SetProgram(CAssetMgr::Get()->GetProgramID("BlinnPhongProgram"));
	m_meshRenderer->SetMesh(CAssetMgr::Get()->GetMesh("DefaultCubeMesh"));
	m_meshRenderer->SetTexture(CAssetMgr::Get()->GetTexture("Box"));
}

void CCube::Update(float _deltaTime)
{
	__super::Update(_deltaTime);
	
	CInput* input = CInput::Get();

	// Moving functionality of the object
	if (input->g_cKeyState[(unsigned char)'w'] == INPUT_HOLD)
	{
		m_transform.position.z -= m_moveSpd * _deltaTime;
	}
	if (input->g_cKeyState[(unsigned char)'s'] == INPUT_HOLD)
	{
		m_transform.position.z += m_moveSpd * _deltaTime;
	}
	if (input->g_cKeyState[(unsigned char)'a'] == INPUT_HOLD)
	{
		m_transform.position.x -= m_moveSpd * _deltaTime;
	}
	if (input->g_cKeyState[(unsigned char)'d'] == INPUT_HOLD)
	{
		m_transform.position.x += m_moveSpd * _deltaTime;
	}
}
