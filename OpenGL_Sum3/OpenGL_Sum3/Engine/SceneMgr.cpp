
// This Include
#include "SceneMgr.h"

// Local Include
#include "Scene.h"
#include "Debug.h"

#include "Game/TestScene.h"

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
	CreateNewScene("Test Scene", new CTestScene());
	
	/** Run the first scene */
	if (!m_scenes.empty())
	{
		auto iter = m_scenes.begin();
		m_runningScene = iter->second;
		m_runningScene->ConfigurateScene();
		m_runningScene->BeginPlay();
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

void CSceneMgr::CreateNewScene(std::string _name, CScene* _scene)
{
	_scene->m_sceneName = _name;
	m_scenes.insert(std::pair<std::string, CScene*>(_name, _scene));
}

void CSceneMgr::LoadScene(std::string _name)
{
	// Reset the current scene
	m_runningScene->ResetScene();

	// Jump to another scene and initialise 
	for (auto iter = m_scenes.begin(); iter != m_scenes.end(); ++iter)
	{
		if (iter->first == _name)
		{
			m_runningScene = iter->second;
			m_runningScene->ConfigurateScene();
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
	for (auto iter = m_scenes.begin(); iter != m_scenes.end(); ++iter)
	{
		delete iter->second;
	}
	m_scenes.clear();
}
