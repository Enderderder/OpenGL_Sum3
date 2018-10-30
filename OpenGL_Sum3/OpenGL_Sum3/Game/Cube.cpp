
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
	
	m_meshRenderer->SetProgram(CAssetMgr::GetInstance()->GetProgramID("BlinnPhongProgram"));
	m_meshRenderer->SetMesh(CAssetMgr::GetInstance()->GetMesh("DefaultCubeMesh"));
	m_meshRenderer->SetTexture(CAssetMgr::GetInstance()->GetTexture("Box"));
}

void CCube::Update()
{
	__super::Update();
	
	CInput* input = CInput::GetInstance();

	// Moving functionality of the object
	if (input->g_cKeyState[(unsigned char)'w'] == INPUT_HOLD)
	{
		m_transform.position.z -= m_moveSpd * CTime::GetInstance()->GetDeltaTime();
	}
	if (input->g_cKeyState[(unsigned char)'s'] == INPUT_HOLD)
	{
		m_transform.position.z += m_moveSpd * CTime::GetInstance()->GetDeltaTime();
	}
	if (input->g_cKeyState[(unsigned char)'a'] == INPUT_HOLD)
	{
		m_transform.position.x -= m_moveSpd * CTime::GetInstance()->GetDeltaTime();
	}
	if (input->g_cKeyState[(unsigned char)'d'] == INPUT_HOLD)
	{
		m_transform.position.x += m_moveSpd * CTime::GetInstance()->GetDeltaTime();
	}
}
