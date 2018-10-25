// Global Include
#include "Utility.h"
#include "SceneMgr.h"
#include "Time.h"
#include "AssetMgr.h"
#include "SceneMgr.h"
#include "Input.h"
#include "TextLabel.h"

// GLobal Variables
static CTime* p_Time = CTime::GetInstance();
static CSceneMgr* p_SceneMgr = CSceneMgr::GetInstance();
static CAssetMgr* p_Asset = CAssetMgr::GetInstance();
static CInput* p_Input = CInput::GetInstance();

void InititializeProgram();
void Render();
void Update();

int main(int argc, char **argv)
{
	// Set up randon
	srand((unsigned int)time(NULL));

	// Create the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GL_MULTISAMPLE);
	glutInitWindowPosition(400, 200);

	glutInitWindowSize((int)util::SCR_WIDTH, (int)util::SCR_HEIGHT);
	glutCreateWindow("Velocitas");
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set Clear Screen Color
	glClearColor(0.0, 1.0, 0.0, 1.0); // Make the background color GREEN

	// Initialize OpenGL Library
	glewInit();
	// Initialize the Game
	InititializeProgram();

	//register callbacks
	glutIdleFunc(Update);
	glutDisplayFunc(Render);

	glutCloseFunc([]() {
		p_Input->DestroyInstance();
		p_SceneMgr->DestroyInstance();
		p_Asset->DestroyInstance();
		p_Time->DestroyInstance();
	}); // Clean up the memory when closing the program

	glutMainLoop(); // Must be called last
}

void InititializeProgram()
{
	p_Time->Initialize();
	p_Asset->InitializeAssets();
	p_SceneMgr->InitializeScenes();
	p_Input->InitializeInput();
}

void Render()
{
	p_SceneMgr->RenderCurrentScene();

	glutSwapBuffers();
}

void Update()
{
	p_Time->Update();

	// Update whats currently running
	p_SceneMgr->UpdateCurrentScene();
	p_Input->Update();

	// Goes To Render
	glutPostRedisplay();
}