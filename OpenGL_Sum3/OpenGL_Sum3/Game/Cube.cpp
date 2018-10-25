
// This Include
#include "Cube.h"

// Engine Include
#include "Engine/MeshComponent.h"
#include "Engine/AssetMgr.h"
#include "Engine/Input.h"
#include "Engine/Time.h"

CCube::CCube() 
{
	m_meshRenderer = CreateComponent<CMeshComponent>();
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

	if (input->g_cKeyState[(unsigned char)'w'] == INPUT_HOLD)
	{
		m_transform.position.z -= 0.5f;
	}
	if (input->g_cKeyState[(unsigned char)'s'] == INPUT_HOLD)
	{
		m_transform.position.z += 0.5f;
	}
	if (input->g_cKeyState[(unsigned char)'a'] == INPUT_HOLD)
	{
		m_transform.position.x -= 0.5f;
	}
	if (input->g_cKeyState[(unsigned char)'d'] == INPUT_HOLD)
	{
		m_transform.position.x += 0.5f;
	}
}
