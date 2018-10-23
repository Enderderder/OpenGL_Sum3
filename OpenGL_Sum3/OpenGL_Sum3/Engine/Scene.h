#pragma once

// Global Include
#include "Utility.h"

// Forward Declaration
class CGameObject;
class CCamera;
class CCubeMap;
class CTextLabel;

class CScene
{
public:
	CScene();
	~CScene();

	/**
	* Create all the game object that is build into the scene
	* at the beginning here. This will be running first when
	* the scene start.
	*/
	virtual void ConfigurateScene();
	/**
	* Update all the contained object and logic of the game
	* every frame
	*/
	virtual void BeginPlay();
	/**
	* After ConfigurationScene() run, the scene is ready to be
	* run. This function will run and calls all the BeginPlay()
	* inside each gameobject and all the logic needs to be run
	* for the scene.
	*/
	virtual void UpdateScene();
	/**
	 * Render all the gameobject inside the scene
	 */
	void RenderScene();
	/**
	 * Reset the scene to its initial state
	 */
	void ResetScene();

	void Instantiate(std::shared_ptr<CGameObject> _gameobj);
	void Instantiate(std::shared_ptr<CGameObject> _gameobj, glm::vec3 _pos);
	void Instantiate(std::shared_ptr<CGameObject> _gameobj, glm::vec3 _pos, glm::vec3 _scale, glm::vec3 _rotation);
	void DestroyObject(std::shared_ptr<CGameObject> _gameobj);

	std::shared_ptr<CGameObject> FindGameObject(std::string _name) const;
	std::vector<std::weak_ptr<CGameObject>> FindGameObjectAll(std::string _name) const;
	std::shared_ptr<CGameObject> FindObjectWithTag(std::string _tag) const;
	std::vector<std::weak_ptr<CGameObject>> FindObjectWithTagAll(std::string _tag) const;

	CCamera* GetMainCamera() const;

	std::vector<std::shared_ptr<CGameObject>> GetObjectVec() const;

	/** Configuration */
public:

	std::string m_sceneName;

	bool m_bScissorTest;

protected:
	
	CCamera* m_mainCamera;
	CCubeMap* m_cubeMap;
	std::vector<std::shared_ptr<CGameObject>> m_vGameObj;

private:

};