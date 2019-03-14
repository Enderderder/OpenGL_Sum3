
// Engine Include
#include "Engine.h"

// GLobal Variables
static CTime* p_Time = CTime::Get();
static CSceneMgr* p_SceneMgr = CSceneMgr::Get();
static CAssetMgr* p_Asset = CAssetMgr::Get();
static CInput* p_Input = CInput::Get();

/** FPS Debugg Label */
CTextLabel* fpsLabel;
float elapsedTime;
float frameRate;
/** ================ */

void InititializeProgram();
void Render();
void Update();

int main(int argc, char **argv)
{
	// Set up random
	srand((unsigned int)time(nullptr));

	// Create the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GL_MULTISAMPLE);
	glutInitWindowPosition(400, 200);

	glutInitWindowSize((int)util::SCR_WIDTH, (int)util::SCR_HEIGHT);
	glutCreateWindow("OpenGL_Playground");
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set Clear Screen Color
	glClearColor(0.0, 0.0, 0.0, 1.0); // Make the background color Black

	// Initialize OpenGL Library
	glewInit();
	// Initialize the Game
	InititializeProgram();

	//register callbacks
	glutIdleFunc(Update);
	glutDisplayFunc(Render);

	glutCloseFunc([]() {
		p_SceneMgr->DestroyInstance();
		p_Asset->DestroyInstance();
		p_Input->DestroyInstance();
		p_Time->DestroyInstance();
		delete fpsLabel;
	}); // Clean up the memory when closing the program

	glutMainLoop(); // Must be called last
}

void InititializeProgram()
{
	p_Input->InitializeInput();
	p_Asset->InitializeAssets();
	p_SceneMgr->InitializeScenes();

	fpsLabel = new CTextLabel("Arial", "00", glm::vec2(0.0f, 0.0f));
	fpsLabel->SetScale(0.5f);
}

void Render()
{
	p_SceneMgr->RenderCurrentScene();

	// Render fps label
	fpsLabel->RenderTextLabel();

	glutSwapBuffers();
}

void Update()
{
	if (p_SceneMgr->GetRunningScene() == nullptr)
	{
		p_SceneMgr->InitializeFirstScene();
		p_Time->Initialize(); // Have to initialize the time after the initialize scene
		return;
	}

	p_Time->Update();
	
	// Update whats currently running
	p_SceneMgr->UpdateCurrentScene(p_Time->GetDeltaTime());
	p_Input->Update();

	// FPS part
	frameRate++;
	elapsedTime += p_Time->GetDeltaTime();
	if (elapsedTime >= 1.0f)
	{
		fpsLabel->SetText(std::to_string((int)frameRate));
		elapsedTime = 0.0f;
		frameRate = 0.0f;
	}

	// Goes To Render
	glutPostRedisplay();
}