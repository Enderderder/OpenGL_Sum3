#pragma once

// Global Include
#include "Utility.h"

// Forward Declare
class CGameObject;

class CComponent : public std::enable_shared_from_this<CComponent>
{
public:
	CComponent();
	virtual ~CComponent();

protected:

	CGameObject* m_owner;
	bool m_isActive;

// Membre Funciton
public:

	/**
	 * Calls when the game starts
	 */
	virtual void BeginPlay() {}
	/**
	 * Calls every tick
	 */
	virtual void Update(float _tick) {}
	/**
	* Getter and setter of the "owner" property of the component
	*/
	void SetOwner(CGameObject* _gameObject);
	CGameObject* GetOwner() const;
	/**
	* Getter and setter of the "active" property of the component
	*/
	void SetActive(bool);
	bool IsActive() const;
};

