//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name    : Input.h
// Description	: 
// Author       : Richard Wulansari & Jacob Dewse
// Mail         : richard.wul7481@mediadesign.school.nz, jacob.dew7364@mediadesign.school.nz
//

#ifndef INPUT_H
#define INPUT_H

class CInput
{
public:
	// Singletom Functions
	static CInput* GetInstance();
	static void DestroyInstance();

	// Member Functions
	void InitializeInput();

	static void InitKeyDown(unsigned char key, int x, int y);
	static void InitKeyUp(unsigned char key, int x, int y);
	static void InitMouse(int button, int glutState, int x, int y);

private:
	// Make Singleton
	CInput();
	~CInput();

	void Keyboard_Down(unsigned char key, int x, int y);
	void Keyboard_Up(unsigned char key, int x, int y);
	void Mouse(int button, int glutState, int x, int y);

	// Make this Instance Non-copyable
	CInput(const CInput& _kr) = delete;
	CInput& operator= (const CInput& _kr) = delete;

protected:
	static CInput* s_pInput;

private: // Private Member Variables


public: // Public Variables
	unsigned char g_cKeyState[255];
	unsigned char g_cMouseState[3];

};

#endif // !INPUT_H