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
	virtual void Update() override;


private:

	std::shared_ptr<CMeshComponent> m_meshRenderer;
};

