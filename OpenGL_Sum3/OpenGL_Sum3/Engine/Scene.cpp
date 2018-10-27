
// Engine Include
#include "Engine.h"

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
			CSpriteRenderComponent* spriteRenderer
				= gameObject->GetComponent<CSpriteRenderComponent>();
			if (spriteRenderer)
			{
				spriteRenderer->Render(m_mainCamera);
				continue;
			}

			CMeshComponent* meshRenderer
				= gameObject->GetComponent<CMeshComponent>();
			if (meshRenderer)
			{
				meshRenderer->RenderMesh(m_mainCamera);
				continue;
			}

			CTerrainComponent* terrainRender
				= gameObject->GetComponent<CTerrainComponent>();
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

std::vector<std::shared_ptr<CGameObject>> CScene::FindGameObjectAll(std::string _name) const
{
	std::vector<std::shared_ptr<CGameObject>> resultVector;

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

std::vector<std::shared_ptr<CGameObject>> CScene::FindObjectWithTagAll(std::string _tag) const
{
	std::vector<std::shared_ptr<CGameObject>> resultVector;

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
