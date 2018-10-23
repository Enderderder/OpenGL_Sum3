#pragma once

// Derive Include
#include "Component.h"

// Global Include
#include "Utility.h"

// Forward Declare
class CCamera;
class CSprite;

class CSpriteRenderComponent : public CComponent
{
public:
	CSpriteRenderComponent();
	~CSpriteRenderComponent();

	virtual void BeginPlay() override;

	void Render(CCamera* camera);

	void SetSprite(CSprite* _sprite);
	void SetProgram(GLuint _programID);
	
private:

	CSprite* m_sprite;
	GLuint m_programID;

};