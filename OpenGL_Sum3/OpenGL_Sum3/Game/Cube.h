#pragma once

// Inherited Include
#include "Engine/GameObject.h"

// Forward Declare
class CMeshComponent;

class CCube : public CGameObject
{
public:
	CCube();
	~CCube();

	virtual void BeginPlay() override;
	virtual void Update(float _deltaTime) override;


private:

	CMeshComponent* m_meshRenderer;

	float m_moveSpd;
};

