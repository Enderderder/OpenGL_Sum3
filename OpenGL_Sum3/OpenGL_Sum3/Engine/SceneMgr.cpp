
// Engine Include
#include "Engine.h"

// Build Scene
#include "BuildScene.h"

// Static Variable
CSceneMgr* CSceneMgr::s_pSceneMgr = nullptr;

CSceneMgr* CSceneMgr::GetInstance()
{
	if (s_pSceneMgr == nullptr)
	{
		s_pSceneMgr = new CSceneMgr();
	}

	return s_pSceneMgr;
}

void CSceneMgr::DestroyInstance()
{
	delete s_pSceneMgr;
	s_pSceneMgr = nullptr;
}

void CSceneMgr::InitializeScenes()
{
	/** Create scenes that is going to build in the game */
	CreateNewScene(new CPhysicSimulationScene());
	CreateNewScene(new CTestScene());
	
	/** Run the first scene */
	if (!m_scenes.empty())
	{
		m_runningScene = m_scenes[0];
		m_runningScene->ConfigurateScene();
		m_runningScene->BeginPlay();
	}
	else
	{
		CDebug::Log("There is no scene build");
	}
}

void CSceneMgr::RenderCurrentScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_runningScene->RenderScene();
}

void CSceneMgr::UpdateCurrentScene()
{
	// Only Proccess the current running scene
	m_runningScene->UpdateScene();
}

void CSceneMgr::CreateNewScene(CScene* _scene)
{
	m_scenes.push_back(_scene);
}

void CSceneMgr::LoadScene(std::string _name)
{
	// Reset the current scene
	m_runningScene->ResetScene();

	// Jump to another scene and initialise 
	for (auto scene : m_scenes)
	{
		if (scene->m_sceneName == _name)
		{
			m_runningScene = scene;
			m_runningScene->ConfigurateScene();
			m_runningScene->BeginPlay();
		}
	}
}

CScene* CSceneMgr::GetRunningScene() const
{
	return m_runningScene;
}

CSceneMgr::CSceneMgr()
{}

CSceneMgr::~CSceneMgr()
{
	m_runningScene = nullptr;

	for (auto scene : m_scenes)
	{
		delete scene;
		scene = nullptr;
	}
	m_scenes.clear();
}
