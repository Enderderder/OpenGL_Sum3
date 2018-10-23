#ifndef UTILITY_H
#define UTILITY_H

// OpenGL Include ------------------------------------------------------------------------------
#include <glew.h>
#include <freeglut.h>
#include <SOIL.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <fmod.hpp>
//#include <Box2D/Box2D.h>

// Library Include -----------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <WS2tcpip.h>
#include <strstream>
#include <Windows.h>
#include <cassert>
#include <thread>
#include <sstream>
#include <codecvt>

//#include <vld.h> // Memory Leak Detector

template<typename T>
std::string ToString(const T& _value)
{
	std::strstream theStream;
	theStream << _value << std::ends;
	return (theStream.str());
}

//----------------------------------------------------------------------------------------------


namespace util
{
	// Define the screen size
	static float SCR_WIDTH = 1366;
	static float SCR_HEIGHT = 768;
	static float PIXELUNIT = 1;

	static std::wstring stringToWstring(const std::string& t_str)
	{
		//setup converter
		typedef std::codecvt_utf8<wchar_t> convert_type;
		std::wstring_convert<convert_type, wchar_t> converter;

		//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
		return converter.from_bytes(t_str);
	}

}

// Define Struct -------------------------------------------------------------------------------

struct DebugTimer
{
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

	DebugTimer()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	
	~DebugTimer()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;
		

		float ms = duration.count();
		std::cout << "Debug Timer took " << ms << " ms \n";
	}

};

enum InputState 
{
	INPUT_FIRST_RELEASE,	// First frame of Up state 
	INPUT_RELEASED,			// Default State (Up) 
	INPUT_FIRST_PRESS,		// First frame of Down state 
	INPUT_HOLD,				// Key is held Down 
};

enum InputMouse 
{ 
	MOUSE_LEFT, 
	MOUSE_MIDDLE, 
	MOUSE_RIGHT 
};

//----------------------------------------------------------------------------------------------



#endif // !_UTILITY_H
