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
	virtual void UpdateScene(float _deltaTime);
	/**
	 * Render all the gameobject inside the scene
	 */
	void RenderScene();
	/**
	 * Reset the scene to its initial state
	 */
	void ResetScene();

	/**
	 * Instantiate a gameobject into the scene
	 */
	template<typename T>
	std::shared_ptr<T> Instantiate(std::shared_ptr<T> _gameobj);
	/**
	* Instantiate a gameobject into the scene with transform detail
	*/
	template<typename T>
	std::shared_ptr<T> Instantiate(std::shared_ptr<T> _gameobj, glm::vec3 _pos, glm::vec3 _scale, glm::vec3 _rotation);
	/**
	* Remove a gameobject from the scene
	*/
	void DestroyObject(std::shared_ptr<CGameObject> _gameobj);

	/**
	 * Find gameobject that exists in the scene
	 */
	std::shared_ptr<CGameObject> FindGameObject(std::string _name) const;
	std::vector<std::shared_ptr<CGameObject>> FindGameObjectAll(std::string _name) const;
	std::shared_ptr<CGameObject> FindObjectWithTag(std::string _tag) const;
	std::vector<std::shared_ptr<CGameObject>> FindObjectWithTagAll(std::string _tag) const;

	/**
	 * Return the rendering camera of the scene
	 */
	CCamera* GetMainCamera() const;

	/**
	 * Return the gameobject container of the scene
	 */
	std::vector<std::shared_ptr<CGameObject>> GetObjectVec() const;

	/** Configuration */
public:

	/** Name of the scene */
	std::string m_sceneName;

	/** Is scene rendering using scissor test */
	bool m_bScissorTest;

protected:
	
	/** Rendering camera of the scene */
	CCamera* m_mainCamera;

	/** Rendering cubemap of the scene */
	CCubeMap* m_cubeMap;

	/** Gameobject container of the scene */
	std::vector<std::shared_ptr<CGameObject>> m_vGameObj;

private:

};

template<typename T>
std::shared_ptr<T> CScene::Instantiate(std::shared_ptr<T> _gameobj)
{
	// Check if the object is a gameobject
	if (std::shared_ptr<CGameObject> gameobject
		= std::dynamic_pointer_cast<CGameObject>(_gameobj))
	{
		gameobject->SetScene(this);
		m_vGameObj.push_back(gameobject);

		// Return the gameobject with its right type
		return _gameobj;
	}

	// Otherwise return nullptr
	CDebug::Log("The object instantiating is not a gameobject");
	return nullptr;
}

template<typename T>
std::shared_ptr<T> CScene::Instantiate(std::shared_ptr<T> _gameobj,
	glm::vec3 _pos,
	glm::vec3 _rotation = glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f))
{
	// Check if the object is a gameobject
	if (std::shared_ptr<CGameObject> gameobject
		= std::dynamic_pointer_cast<CGameObject>(_gameobj))
	{
		gameobject->SetScene(this);
		gameobject->m_transform.position = _pos;
		gameobject->m_transform.rotation = _rotation;
		gameobject->m_transform.scale = _scale;
		m_vGameObj.push_back(gameobject);

		// Return the gameobject with its right type
		return _gameobj;
	}

	// Otherwise return nullptr
	CDebug::Log("The object instantiating is not a gameobject");
	return nullptr;
}