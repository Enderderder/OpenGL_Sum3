
// This Include
#include "Debug.h"

// Engine Include
#include "GameObject.h"

void CDebug::Log(std::string _info)
{
	std::cout << "Debug Log: ";
	std::cout << _info << "\n";
}

void CDebug::Log(CGameObject* _gameObject, std::string _info)
{
	std::cout << "(Debug) " << _gameObject->m_name << ": ";
	std::cout << _info << "\n";
}

