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


};
