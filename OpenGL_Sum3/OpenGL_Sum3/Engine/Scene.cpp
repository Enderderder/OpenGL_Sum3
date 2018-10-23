// This Include
#include "Scene.h"

// Engine Include
#include "GameObject.h"
#include "SpriteRenderComponent.h"
#include "MeshComponent.h"
#include "Camera.h"
#include "CubeMap.h"
#include "Debug.h"
#include "Terrain.h"
//#include "Player.h"
//#include "PowerUps.h"
//#include "AssetMgr.h"
//#include "MeshMgr.h"
//#include "ModelMgr.h"
//#include "SceneMgr.h"
//#include "Input.h"
//#include "Camera.h"
//#include "CAIMgr.h"
//#include "TextLabel.h"

CScene::CScene()
{
	m_mainCamera = nullptr;
	m_cubeMap = nullptr;
}

CScene::~CScene()
{
	std::cout << "Cleaning Scene... \n";
	// Clean up the memory allocated variables inside the class
	// ========================================================
	delete m_mainCamera;
	m_cubeMap = nullptr;

	// Clear the vector to destroy all the shared ptr object
	m_vGameObj.clear();

	// ========================================================
	std::cout << "Cleaning Done... \n";
}

void CScene::ConfigurateScene() { m_vGameObj.resize(0); }

void CScene::BeginPlay()
{
	for (auto obj : m_vGameObj)
	{
		obj->BeginPlay();
	}
}

void CScene::RenderScene()
{
	if (m_bScissorTest)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 100, 1366, 668);
	}

	if (m_cubeMap)
	{ 
		m_cubeMap->RenderCubeMap(m_mainCamera);
	}

	if (!m_vGameObj.empty())
	{
		for (std::shared_ptr<CGameObject> gameObject : m_vGameObj)
		{
			std::shared_ptr<CSpriteRenderComponent> spriteRenderer
				= gameObject->GetComponent<CSpriteRenderComponent>();
			if (spriteRenderer)
			{
				spriteRenderer->Render(m_mainCamera);
				continue;
			}

			std::shared_ptr<CMeshComponent> meshRenderer 
				= gameObject->GetComponent<CMeshComponent>();
			if (meshRenderer)
			{
				meshRenderer->RenderMesh(m_mainCamera);
				continue;
			}

			std::shared_ptr<CTerrain> terrainRender 
				= gameObject->GetComponent<CTerrain>();
			if (terrainRender)
			{
				terrainRender->RenderTerrain(m_mainCamera);
				continue;
			}
		}
	}

	glDisable(GL_SCISSOR_TEST);
}

void CScene::ResetScene()
{
	m_vGameObj.clear();
}

void CScene::UpdateScene()
{
	// Refresh the camera
	m_mainCamera->Update();

	// Delete the object that should be deleted fron last frame
	for (auto obj : m_vGameObj)
	{
		if (obj->ShouldDestroyed()) { DestroyObject(obj); }
	}

	// Get each Object in the Scene and do their own Update Function
	size_t currVecSize = m_vGameObj.size();
	for (size_t index = 0; index < currVecSize; ++index)
	{
		m_vGameObj[index]->Update();
		currVecSize = m_vGameObj.size(); // Revalidate the number of item inside the vector
	}
}

/* Legacy code */
// void CScene::CheckCollision()
// {
// 	size_t currVecSize = m_vGameObj.size();
// 	for (size_t index = 0; index < currVecSize; ++index)
// 	{
// 		if (m_vGameObj[index]->HasCollider()) // Check if object itself has a collider
// 		{
// 			// Get the collistion detail of the object
// 			float selfCollider = m_vGameObj[index]->GetCollisionRad();
// 			glm::vec3 selfPos = m_vGameObj[index]->GetTransform();
// 
// 			// Check with all the rest of the other objects
// 			for (size_t i = index + 1; i < currVecSize; ++i)
// 			{
// 				if (m_vGameObj[i]->HasCollider())
// 				{
// 					// Get the other objects' collision detail
// 					float otherCollider = m_vGameObj[i]->GetCollisionRad();
// 					glm::vec3 otherPos = m_vGameObj[i]->GetTransform();
// 
// 					float distance = glm::distance(selfPos, otherPos);
// 					if (distance <= (selfCollider + otherCollider))
// 					{
// 						m_vGameObj[index]->OnCollision(m_vGameObj[i]);
// 						m_vGameObj[i]->OnCollision(m_vGameObj[index]);
// 					}
// 				}
// 			}
// 		}
// 		
// 
// 
// 		currVecSize = m_vGameObj.size(); // Revalidate the number of item inside the vector
// 	}
// }

void CScene::Instantiate(std::shared_ptr<CGameObject> _gameobj)
{
	m_vGameObj.push_back(_gameobj);
}

void CScene::Instantiate(std::shared_ptr<CGameObject> _gameobj, glm::vec3 _pos)
{
	_gameobj->m_transform.position = _pos;
	m_vGameObj.push_back(_gameobj);
}

void CScene::Instantiate(std::shared_ptr<CGameObject> _gameobj,
	glm::vec3 _pos, 
	glm::vec3 _rotation, 
	glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f))
{
	_gameobj->m_transform.position = _pos;
	_gameobj->m_transform.rotation = _rotation;
	_gameobj->m_transform.scale = _scale;
	m_vGameObj.push_back(_gameobj);
}

void CScene::DestroyObject(std::shared_ptr<CGameObject> _gameobj)
{
	for (auto iter = m_vGameObj.begin(); iter != m_vGameObj.end(); ++iter)
	{
		if ((*iter) == _gameobj)
		{
			m_vGameObj.erase(iter);
			break;
		}
	}
}

std::shared_ptr<CGameObject> CScene::FindGameObject(std::string _name) const
{
	for (auto iter = m_vGameObj.begin(); iter != m_vGameObj.end(); ++iter)
	{
		if ((*iter)->m_name == _name)
		{
			return *iter;
		}
	}

	return nullptr;
}

std::vector<std::weak_ptr<CGameObject>> CScene::FindGameObjectAll(std::string _name) const
{
	std::vector<std::weak_ptr<CGameObject>> resultVector;

	for (auto iter = m_vGameObj.begin(); iter != m_vGameObj.end(); ++iter)
	{
		if ((*iter)->m_name == _name)
		{
			resultVector.push_back(*iter);
		}
	}

	return resultVector;
}

std::shared_ptr<CGameObject> CScene::FindObjectWithTag(std::string _tag) const
{
	for (auto iter = m_vGameObj.begin(); iter != m_vGameObj.end(); ++iter)
	{
		if ((*iter)->m_tag == _tag)
		{
			return *iter;
		}
	}

	return nullptr;
}

std::vector<std::weak_ptr<CGameObject>> CScene::FindObjectWithTagAll(std::string _tag) const
{
	std::vector<std::weak_ptr<CGameObject>> resultVector;

	for (auto iter = m_vGameObj.begin(); iter != m_vGameObj.end(); ++iter)
	{
		if ((*iter)->m_tag == _tag)
		{
			resultVector.push_back(*iter);
		}
	}

	return resultVector;
}

CCamera* CScene::GetMainCamera() const
{
	return m_mainCamera;
}

std::vector<std::shared_ptr<CGameObject>> CScene::GetObjectVec() const
{
	return m_vGameObj;
}
