
// Inlcude Game Classes
#include "GameClasses.h"

// Engine Include
#include "Engine/Engine.h"


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
	m_spriteRenderer->SetSprite(CAssetMgr::Get()->GetSprite("Block"));
	m_spriteRenderer->SetProgram(CAssetMgr::Get()->GetProgramID("DefaultSpriteProgram"));


	//this->m_transform.scale = glm::vec3(1000.0f, 1000.0f, 1000.0f);
}

void CSpaceShip::Update(float _deltaTime)
{
	__super::Update(_deltaTime);
	

}
