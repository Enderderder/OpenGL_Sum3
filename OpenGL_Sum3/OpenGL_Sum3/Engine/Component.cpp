
// Engine Include
#include "Engine.h"

CComponent::CComponent() : m_isActive(true) {}

CComponent::~CComponent() {}

void CComponent::SetOwner(CGameObject* _gameObject)
{
	this->m_owner = _gameObject;
}

CGameObject* CComponent::GetOwner() const
{
	return m_owner;
}

void CComponent::SetActive(bool _b)
{
	m_isActive = _b;
}

bool CComponent::IsActive() const
{
	return m_isActive;
}
