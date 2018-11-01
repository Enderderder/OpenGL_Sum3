#pragma once

// Global Include
#include "Utility.h"

// Forawrd Declare
class CScene;

class CSceneMgr
{
#pragma region Singleton
public:
	// Singleton Methods
	static CSceneMgr* GetInstance();
	static void DestroyInstance();

private:
	// Make singleton by set the constructor inside private
	CSceneMgr();
	~CSceneMgr();

	// Make this class non-copyable
	CSceneMgr(const CSceneMgr& _kr) = delete;
	CSceneMgr& operator= (const CSceneMgr& _kr) = delete;

protected:
	// Singleton Object
	static CSceneMgr* s_pSceneMgr;

#pragma endregion Singleton

public:
	
	// Member Functions
	void InitializeScenes();
	void InitializeFirstScene();
	void RenderCurrentScene();
	void UpdateCurrentScene();
	void CreateNewScene(CScene* _scene);
	void LoadScene(std::string _name);
	CScene* GetRunningScene() const;

private:
	// Member Variables

	std::vector<CScene*> m_scenes;

	CScene* m_runningScene;
};