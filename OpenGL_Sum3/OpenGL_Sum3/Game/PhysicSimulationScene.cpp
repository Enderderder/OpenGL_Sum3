
// This Include
#include "PhysicSimulationScene.h"

// Inlcude Game Classes
#include "GameClasses.h"

// Engine Include
#include "Engine/Engine.h"

void CPhysicSimulationScene::ConfigurateScene()
{
	__super::ConfigurateScene();
	
	/** Scene Configuration */
	m_sceneName = "PhysicSimulationScene";
	m_bScissorTest = false;

	/** Create Cubemap Object */
	m_cubeMap = CAssetMgr::GetInstance()->GetCubeMap("DefaultCubeMap");

	/** Create Camera Object */
	m_mainCamera = new CCamera();
	m_mainCamera->m_bIsControlling = true;

	/** Create game objects in the scenes */

	std::shared_ptr<CGameObject> clothObj = Instantiate(std::make_shared<CGameObject>());
	clothObj->m_transform.position = { 0.0f, 0.0f, -10.0f };
	auto* clothComponent = clothObj->CreateComponent<CClothComponent>();
}

void CPhysicSimulationScene::BeginPlay()
{
	__super::BeginPlay();
	
}

void CPhysicSimulationScene::UpdateScene()
{
	__super::UpdateScene();
	

}
