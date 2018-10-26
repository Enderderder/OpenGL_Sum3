
// Engine Include
#include "Engine.h"

CGameObject::CGameObject()
{
	m_shouldDestroyed = false;
	m_isActive = true;
	m_transform.gameObject = this;
}

CGameObject::~CGameObject()
{
	for (auto iter : m_components)
	{
		delete iter;
		iter = nullptr;
	}
	m_components.clear();
}

void CGameObject::BeginPlay() 
{
	for (auto iter : m_components)
	{
		iter->BeginPlay();
	}
}

void CGameObject::Update() 
{
	for (auto iter : m_components)
	{
		
	}
}

void CGameObject::DestroyObject()
{
	this->m_shouldDestroyed = true;
}

bool CGameObject::IsActive() const
{
	return m_isActive;
}

void CGameObject::SetActive(bool _b)
{
	m_isActive = _b;
}

bool CGameObject::ShouldDestroyed() const
{
	return m_shouldDestroyed;
}