
// Inlcude Game Classes
#include "GameClasses.h"

// Engine Include
#include "Engine/Engine.h"

CWindBox::CWindBox()
{
	m_meshRenderer = CreateComponent<CMeshComponent>();
	m_meshRenderer->SetProgram(CAssetMgr::GetInstance()->GetProgramID("BlinnPhongProgram"));
	m_meshRenderer->SetMesh(CAssetMgr::GetInstance()->GetMesh("DefaultCubeMesh"));
	m_meshRenderer->SetTexture(CAssetMgr::GetInstance()->GetTexture("Box"));
}

CWindBox::~CWindBox()
{}

void CWindBox::BeginPlay()
{
	__super::BeginPlay();
	
	m_moveSpd = 30.0f;
}

void CWindBox::Update()
{
	__super::Update();
	
	if (!m_targetCloth.expired())
	{

	}
	else
	{
		CDebug::Log("There is no target set");
	}
}

void CWindBox::AttachTarget(std::shared_ptr<CGameObject> _targetObj)
{
	m_targetCloth = _targetObj;
}
