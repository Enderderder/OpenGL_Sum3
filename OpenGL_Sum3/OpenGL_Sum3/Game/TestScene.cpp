
// This Include
#include "TestScene.h"

// Inlcude the game classes
#include "GameClasses.h"
#include "Engine/Camera.h"
#include "Engine/AssetMgr.h"
#include "Engine/MeshComponent.h"
#include "Engine/CubeMap.h"
#include "Engine/Terrain.h"
#include "Engine/SpriteRenderComponent.h"

static CAssetMgr* p_Assets = CAssetMgr::GetInstance();

void CTestScene::ConfigurateScene()
{
	__super::ConfigurateScene();

	/** Scene Configuration */
	m_bScissorTest = false;


	/** Create Cubemap Object */
	m_cubeMap = CAssetMgr::GetInstance()->GetCubeMap("DefaultCubeMap");


	/** Create Camera Object */
	CCamera* mainCamera = new CCamera();
	this->m_mainCamera = mainCamera;
	
	/** Create game objects in the scenes */

	std::shared_ptr<CGameObject> terrain = std::make_shared<CGameObject>();
	terrain->m_transform.position = glm::vec3(0.0, 0.0, 0.0);
	terrain->m_transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	std::shared_ptr<CTerrain> terrainRenderer = terrain->CreateComponent<CTerrain>();
	m_terrainRenderer = terrainRenderer;
	HeightMapInfo heightMap;
	heightMap.heightmapFilename = util::stringToWstring("Resources/HeightMaps/HeightMap3.data");
	heightMap.numRows = 256;
	heightMap.numCols = 256;
	heightMap.cellSpacing = 2.0f;
	terrainRenderer->CreateTerrain(heightMap);
	this->m_vGameObj.push_back(terrain);

	std::shared_ptr<CCube> cubeOBJ = std::make_shared<CCube>();
	m_cubeOBJ = cubeOBJ;
	cubeOBJ->m_transform.position = glm::vec3(100.0f, 0.0f, 100.0f);
	cubeOBJ->m_transform.scale = glm::vec3(10.0f, 10.0f, 10.0f);
	this->m_vGameObj.push_back(cubeOBJ);
	cubeOBJ->m_transform.position.y
		= terrainRenderer->GetHeight(cubeOBJ->m_transform.position.x, cubeOBJ->m_transform.position.z);
}

void CTestScene::UpdateScene()
{
	__super::UpdateScene();
	
	glm::vec3 postion = m_cubeOBJ.lock()->m_transform.position;
	m_cubeOBJ.lock()->m_transform.position.y
		= m_terrainRenderer.lock()->GetHeight(postion.x, postion.z);

}

