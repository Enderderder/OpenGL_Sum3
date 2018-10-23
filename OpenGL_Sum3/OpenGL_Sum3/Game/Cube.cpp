
// This Include
#include "Cube.h"

// Engine Include
#include "Engine/MeshComponent.h"
#include "Engine/AssetMgr.h"

CCube::CCube() 
{
	m_meshRenderer = CreateComponent<CMeshComponent>();
}

CCube::~CCube() {}

void CCube::BeginPlay()
{
	m_meshRenderer->SetProgram(CAssetMgr::GetInstance()->GetProgramID("BlinnPhongProgram"));
	m_meshRenderer->SetMesh(CAssetMgr::GetInstance()->GetMesh("DefaultCubeMesh"));
	m_meshRenderer->SetTexture(CAssetMgr::GetInstance()->GetTexture("Box"));
}

void CCube::Update()
{

}
