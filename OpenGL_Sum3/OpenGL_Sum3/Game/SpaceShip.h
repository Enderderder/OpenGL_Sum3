#pragma once

// Derive Include
#include "Engine/GameObject.h"

// Forward Declare
class CSpriteRenderComponent;


class CSpaceShip : public CGameObject
{
public:
	CSpaceShip();
	~CSpaceShip();

	virtual void BeginPlay() override;
	virtual void Update() override;

private:

	CSpriteRenderComponent* m_spriteRenderer;

};

