#pragma once

// Inherited Include
#include "Component.h"

// Forward Declare
class CCamera;
class ssAnimatedModel;

class CAnimationComponent : public CComponent
{
public: 

	CAnimationComponent();
	virtual ~CAnimationComponent();

	virtual void BeginPlay() override;
	virtual void Update() override;

	void Render(CCamera* _camera);

	void SetModel(std::string _path, GLuint _program, CCamera* _camera);
	ssAnimatedModel* GetModel() const;

private:

	ssAnimatedModel* m_animModel;


};