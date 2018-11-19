
// This Include
#include "TestScene.h"

// Inlcude Game Classes
#include "GameClasses.h"

// Engine Include
#include "Engine/Engine.h"
#include "Engine/ssAnimatedModel.h"

static CAssetMgr* p_Assets = CAssetMgr::GetInstance();

void CTestScene::ConfigurateScene()
{
	__super::ConfigurateScene();

	/** Scene Configuration */
	m_sceneName = "TestScene";
	m_bScissorTest = false;

	/** Create Cubemap Object */
	m_cubeMap = CAssetMgr::GetInstance()->GetCubeMap("DefaultCubeMap");

	/** Create Camera Object */
	m_mainCamera = new CCamera();
	m_mainCamera->m_bIsControlling = true;
	
	/** Create game objects in the scenes */

	std::shared_ptr<CGameObject> terrain = Instantiate(std::make_shared<CGameObject>());
	terrain->m_transform.scale = { 1.0f, 1.0f, 1.0f };
	m_terrain = terrain;
	CTerrainComponent* terrainRenderer = terrain->CreateComponent<CTerrainComponent>();
	HeightMapInfo heightMap;
	heightMap.heightmapFilename = "Resources/HeightMaps/RandomHeightmap.tga";
	heightMap.cellSpacing = 3.0f;
	heightMap.smoothness = 0.8f;
	terrainRenderer->CreateTerrain(heightMap);

	std::shared_ptr<CGameObject> cpuParticle_Rain = Instantiate(std::make_shared<CGameObject>());
	cpuParticle_Rain->m_transform.position.y = 1000.0f;
	CCPUParticleComponent* particleSystem = cpuParticle_Rain->CreateComponent<CCPUParticleComponent>();
	particleSystem->m_isLooping = true;
	particleSystem->m_lifeTime = 6.0f;
	particleSystem->m_particleCount = 10000.0f;
	particleSystem->m_particlesPerSecond = 2000.0f;
	particleSystem->m_spawnRadius = 400.0f;
	particleSystem->SetTexture("Box");

	std::shared_ptr<CPlayer> animatedPlayer = Instantiate(std::make_shared<CPlayer>());
	animatedPlayer->LinkTerrain(terrain);
}

void CTestScene::BeginPlay()
{
	__super::BeginPlay();

	m_mainCamera->m_transform.position = { 300.0f, 200.0f, 500.f };
}

void CTestScene::UpdateScene()
{
	__super::UpdateScene();
	
	//glm::vec3 position = m_cubeOBJ.lock()->m_transform.position;
	//CTerrainComponent* terrainRender = m_terrain.lock()->GetComponent<CTerrainComponent>();

}
