#pragma once

// Derive Include
#include "Engine/Scene.h"

class CPhysicSimulationScene : public CScene
{
public:

	virtual void ConfigurateScene() override;
	virtual void BeginPlay() override;
	virtual void UpdateScene() override;

private:

	std::weak_ptr<CGameObject> m_clothObj;

	std::weak_ptr<CGameObject> m_groundObj;
};
