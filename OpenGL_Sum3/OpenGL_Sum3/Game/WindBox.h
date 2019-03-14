#pragma once

// Inherited Incldue
#include "Engine/GameObject.h"

// Forward Declare
class CMeshComponent;

class CWindBox : public CGameObject
{
public:

	CWindBox();
	~CWindBox();

	virtual void BeginPlay() override;
	virtual void Update(float _deltaTime) override;

	void AttachTarget(std::shared_ptr<CGameObject> _targetObj);

private:

	void ProccesMovement();



private:

	CMeshComponent* m_meshRenderer;

	float m_moveSpd;

	std::weak_ptr<CGameObject> m_targetCloth;
};
