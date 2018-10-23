//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name    : Input.cpp
// Description	: 
// Author       : Richard Wulansari & Jacob Dewse
// Mail         : richard.wul7481@mediadesign.school.nz, jacob.dew7364@mediadesign.school.nz
//

// This Include 
#include "Input.h"

// Local Include
#include "Utility.h"

// Static Variable
CInput* CInput::s_pInput = nullptr;

CInput::CInput()
{}

CInput::~CInput()
{}

CInput* CInput::GetInstance()
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
	for (unsigned char i = 0; i < 255; i++)
	{
		g_cKeyState[i] = INPUT_RELEASED;
	}

	glutKeyboardFunc(InitKeyDown);
	glutKeyboardUpFunc(InitKeyUp);
	glutMouseFunc(InitMouse);
}

void CInput::Keyboard_Down(unsigned char key, int x, int y)
{
	if (g_cKeyState[key] != INPUT_HOLD && g_cKeyState[key] != INPUT_FIRST_PRESS)
	{
		g_cKeyState[key] = INPUT_FIRST_PRESS;
		return;
	}
	else g_cKeyState[key] = INPUT_HOLD;
}


void CInput::Keyboard_Up(unsigned char key, int x, int y)
{
	g_cKeyState[key] = INPUT_RELEASED;
}

void CInput::Mouse(int button, int glutState, int x, int y)
{
	if (button < 3)
	{
		g_cMouseState[button] = (glutState == GLUT_DOWN) ? INPUT_HOLD : INPUT_RELEASED;
	}
}

void CInput::InitKeyDown(unsigned char key, int x, int y)
{
		CInput::GetInstance()->Keyboard_Down(key, x, y);
}

void CInput::InitKeyUp(unsigned char key, int x, int y)
{
	CInput::GetInstance()->Keyboard_Up(key, x, y);
}

void CInput::InitMouse(int button, int glutState, int x, int y)
{
	CInput::GetInstance()->Mouse(button, glutState, x, y);
}
