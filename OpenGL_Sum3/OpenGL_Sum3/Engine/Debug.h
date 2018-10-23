

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
	 * Logging out both the gameObject name and the info onto console
	 */
	static void Log(CGameObject* _gameObject, std::string _info = "");

};