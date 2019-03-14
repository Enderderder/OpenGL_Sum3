
// This Include 
#include "InputMgr.h"

// Static Variable
CInput* CInput::s_pInput = nullptr;

CInput::CInput()
{}

CInput::~CInput()
{}

CInput* CInput::Get()
{
	if (s_pInput == nullptr)
	{
		s_pInput = new CInput();
	}
	return s_pInput;
}

void CInput::DestroyInstance()
{
	delete s_pInput;
	s_pInput = nullptr;
}

void CInput::InitializeInput()
{
#pragma region Set all input state to release

	for (auto& key : g_cKeyState)
	{
		key = INPUT_RELEASED;
	}

	for (auto& button : g_cMouseState)
	{
		button = INPUT_RELEASED;
	}

#pragma endregion Set all input state to release

	glutKeyboardFunc(InitKeyDown);
	glutKeyboardUpFunc(InitKeyUp);
	glutSpecialFunc(InitSpecKeyDown);
	glutSpecialUpFunc(InitSpecKeyUp);
	glutMouseFunc(InitMouse);
	glutMotionFunc(InitMouseMotion);
	glutPassiveMotionFunc(InitMouseMotion);
}

void CInput::Update()
{
	for (auto& key : g_cKeyState)
	{
		if (key == INPUT_FIRST_PRESS)
		{
			key = INPUT_HOLD;
		}
	}

	for (auto& button : g_cMouseState)
	{
		if (button == INPUT_FIRST_PRESS)
		{
			button = INPUT_HOLD;
		}
	}

	for (auto& specKey : g_cSpecialKey)
	{
		if (specKey == INPUT_FIRST_PRESS)
		{
			specKey = INPUT_HOLD;
		}
	}
}

void CInput::KeyboardDown(unsigned char key, int x, int y)
{
	if (g_cKeyState[key] != INPUT_HOLD && g_cKeyState[key] != INPUT_FIRST_PRESS)
	{
		g_cKeyState[key] = INPUT_FIRST_PRESS;
	}
	else g_cKeyState[key] = INPUT_HOLD;
}

void CInput::KeyboardUp(unsigned char key, int x, int y)
{
	g_cKeyState[key] = INPUT_RELEASED;
}

void CInput::KeyboardSpecialDown(int key, int x, int y)
{
	if (g_cSpecialKey[key] != INPUT_HOLD && g_cSpecialKey[key] != INPUT_FIRST_PRESS)
	{
		g_cSpecialKey[key] = INPUT_FIRST_PRESS;
	}
	else g_cSpecialKey[key] = INPUT_HOLD;
}

void CInput::KeyboardSpecialUp(int key, int x, int y)
{
	g_cSpecialKey[key] = INPUT_RELEASED;
}

void CInput::MouseButton(int button, int glutState, int x, int y)
{
	if (button < 3)
	{
		if (glutState == GLUT_DOWN)
		{
			if (g_cMouseState[button] != INPUT_HOLD && g_cMouseState[button] != INPUT_FIRST_PRESS)
			{
				g_cMouseState[button] = INPUT_FIRST_PRESS;
			}
			else g_cMouseState[button] = INPUT_HOLD;
		}
		else
		{
			g_cMouseState[button] =  INPUT_RELEASED;
		}
	}
}

void CInput::MouseMotion(int x, int y)
{
	// Convert the OpenGL coord to NDC
	float ndcMouseX = (2.0f * (float)x / util::SCR_WIDTH - 1.0f);
	float ndcMouseY = 1.0f - (2.0f * (float)y) / util::SCR_HEIGHT;
	g_mouseNdcPosition = { ndcMouseX, ndcMouseY };

	// Get the mouse position at OpenGL coord
	glm::vec2 mousePosition = glm::vec2((float)x, (float)y);

	// Convert the mouse origin to centre of the screen
	mousePosition.x -= util::SCR_WIDTH / 2.0f;
	mousePosition.y -= util::SCR_HEIGHT / 2.0f;
}

void CInput::InitKeyDown(unsigned char key, int x, int y)
{
	CInput::Get()->KeyboardDown(key, x, y);
}

void CInput::InitKeyUp(unsigned char key, int x, int y)
{
	CInput::Get()->KeyboardUp(key, x, y);
}

void CInput::InitSpecKeyDown(int key, int x, int y)
{
	CInput::Get()->KeyboardSpecialDown(key, x, y);
}

void CInput::InitSpecKeyUp(int key, int x, int y)
{
	CInput::Get()->KeyboardSpecialUp(key, x, y);
}

void CInput::InitMouse(int button, int glutState, int x, int y)
{
	CInput::Get()->MouseButton(button, glutState, x, y);
}

void CInput::InitMouseMotion(int x, int y)
{
	CInput::Get()->MouseMotion(x, y);
}