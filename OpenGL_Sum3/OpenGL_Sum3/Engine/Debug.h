

// Global Include
#include "Utility.h"

// Forward Declare
class CGameObject;

class CDebug
{

public:

	/**
	* Logging out some text info onto console
	*/
	static void Log(std::string _info);

	/**
	 * Logging out some text info onto console
	 */
	template<typename T>
	static void Log(T _info);
	/**
	 * Logging out both the gameObject name and the info onto console
	 */
	template<typename T>
	static void Log(CGameObject* _gameObject, T _info = "");

};

template<typename T>
void CDebug::Log(T _info)
{
	std::cout << "Debug Log: ";
	std::cout << util::ToString(_info) << "\n";
}

template<typename T>
void CDebug::Log(CGameObject* _gameObject, T _info)
{
	//std::cout << "(Debug) " << _gameObject->m_name << ": ";
	std::cout << util::ToString(_info) << "\n";
}