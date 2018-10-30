
// Engine Include
#include "Engine.h"

CGameObject::CGameObject()
{
	m_scene = nullptr;
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
	for (auto component : m_components)
	{
		if (component->IsActive())
		{
			component->BeginPlay();
		}
	}
}

void CGameObject::Update() 
{
	for (auto component : m_components)
	{
		if (component->IsActive())
		{
			component->Update();
		}
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

void CGameObject::SetScene(CScene* _scene)
{
	m_scene = _scene;
}

CScene* CGameObject::GetScene() const
{
	return m_scene;
}

bool CGameObject::ShouldDestroyed() const
{
	return m_shouldDestroyed;
}