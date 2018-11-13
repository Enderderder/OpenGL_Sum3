#pragma once

// Inherited Include
#include "Engine/GameObject.h"

// Forward Declare
class CAnimationComponent;
class CTerrainComponent;

enum PLAYER_ANIM_STATE
{
	IDLE,
	RUNNING_FORWARD,
	RUNNING_BACKWARD,
	JUMPING,

};


class CPlayer : public CGameObject
{
public: 

	CPlayer();
	virtual ~CPlayer();

	virtual void BeginPlay() override;
	virtual void Update() override;

	void LinkTerrain(std::shared_ptr<CGameObject> _terrain);

private:

	void StandOnTerrain();
	void SyncTransform();

	void ProcessJump();

	glm::vec3 jumpvelo = {0,0,0};
private:

	CAnimationComponent* m_animComponent;
	PLAYER_ANIM_STATE m_animState;
	
	float m_speed;
	bool m_isJumping;
	float m_jumpHeight;
	bool m_hasReachHeight;

	CTerrainComponent* m_linkedTerrain;

};