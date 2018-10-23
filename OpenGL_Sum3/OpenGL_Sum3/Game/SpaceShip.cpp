
// This Include
#include "SpaceShip.h"

// Engine Include
#include "Engine/AssetMgr.h"
#include "Engine/Component.h"
#include "Engine/SpriteRenderComponent.h"
#include <Box2D.h>


CSpaceShip::CSpaceShip()
{
	m_spriteRenderer = CreateComponent<CSpriteRenderComponent>();
}

CSpaceShip::~CSpaceShip()
{}

void CSpaceShip::BeginPlay()
{
	__super::BeginPlay();
	
	/** Set the sprite for the object */
	m_spriteRenderer->SetSprite(CAssetMgr::GetInstance()->GetSprite("Block"));
	m_spriteRenderer->SetProgram(CAssetMgr::GetInstance()->GetProgramID("DefaultSpriteProgram"));


	//this->m_transform.scale = glm::vec3(1000.0f, 1000.0f, 1000.0f);
}

void CSpaceShip::Update()
{
	__super::Update();
	

}
