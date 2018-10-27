
// This Include
#include "TestScene.h"

// Inlcude Game Classes
#include "GameClasses.h"

// Engine Include
#include "Engine/Engine.h"

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
	mainCamera->m_bIsControlling = false;
	this->m_mainCamera = mainCamera;
	
	/** Create game objects in the scenes */

	std::shared_ptr<CGameObject> terrain = std::make_shared<CGameObject>();
	terrain->m_transform.position = glm::vec3(0.0, 0.0, 0.0);
	terrain->m_transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	CTerrainComponent* terrainRenderer = terrain->CreateComponent<CTerrainComponent>();
	m_terrain = terrain;
	HeightMapInfo heightMap;
	heightMap.heightmapFilename = "Resources/HeightMaps/RandomHeightmap.tga";
	heightMap.cellSpacing = 4.0f;
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

void CTestScene::BeginPlay()
{
	__super::BeginPlay();

	m_mainCamera->m_cameraPosition = { 300.0f, 200.0f, 500.f };
}

void CTestScene::UpdateScene()
{
	__super::UpdateScene();
	
	glm::vec3 position = m_cubeOBJ.lock()->m_transform.position;
	CTerrainComponent* terrainRender = m_terrain.lock()->GetComponent<CTerrainComponent>();

	m_cubeOBJ.lock()->m_transform.position.y
		= terrainRender->GetHeight(position.x, position.z) + m_cubeOBJ.lock()->m_transform.scale.y;
}

