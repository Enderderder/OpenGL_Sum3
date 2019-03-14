#pragma once

// Inherited Include
#include "Component.h"

class CLightComponent : public CComponent
{

public:

	CLightComponent();
	virtual ~CLightComponent();

	virtual void BeginPlay() override;
	virtual void Update() override;
};