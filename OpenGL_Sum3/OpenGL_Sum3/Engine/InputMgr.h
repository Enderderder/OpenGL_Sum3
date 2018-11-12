
#pragma once

// Global Include
#include "Utility.h"


class CInput
{
public:
	// Singletom Functions
	static CInput* GetInstance();
	static void DestroyInstance();

	// Member Functions
	void InitializeInput();
	void Update();

	static void InitKeyDown(unsigned char key, int x, int y);
	static void InitKeyUp(unsigned char key, int x, int y);
	static void InitSpecKeyDown(int key, int x, int y);
	static void InitSpecKeyUp(int key, int x, int y);
	static void InitMouse(int button, int glutState, int x, int y);
	static void InitMouseMotion(int x, int y);

private:
	// Make Singleton
	CInput();
	~CInput();

	// Make this Instance Non-copyable
	CInput(const CInput& _kr) = delete;
	CInput& operator= (const CInput& _kr) = delete;

protected:
	static CInput* s_pInput;

private: // Private Member Variables

	void KeyboardDown(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	void KeyboardSpecialDown(int key, int x, int y);
	void KeyboardSpecialUp(int key, int x, int y);
	void MouseButton(int button, int glutState, int x, int y);
	void MouseMotion(int x, int y);

public: // Public Variables

	InputState g_cKeyState[255];
	InputState g_cSpecialKey[118];
	InputState g_cMouseState[3];
	glm::vec2 g_mousePosition;
	glm::vec2 g_mouseNdcPosition;
};