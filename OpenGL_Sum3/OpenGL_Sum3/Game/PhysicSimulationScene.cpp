
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
	m_cubeMap = CAssetMgr::Get()->GetCubeMap("DefaultCubeMap");

	/** Create Camera Object */
	m_mainCamera = new CCamera();
	m_mainCamera->m_transform.position.z += 5.0f;
	m_mainCamera->m_bIsControlling = true;

	/** Create game objects in the scenes */

	// Create the cloth object for the simulation
	m_clothObj = Instantiate(std::make_shared<CGameObject>());
	m_clothObj.lock()->m_transform.position = { 0.0f, 0.0f, -10.0f };
	m_clothObj.lock()->m_transform.rotation = { 0.0f, 45.0f, 0.0f };
	auto* clothComponent = m_clothObj.lock()->CreateComponent<CClothComponent>();

	// Create the ground object for the cloth
	m_groundObj = Instantiate(std::make_shared<CCube>());
	m_groundObj.lock()->m_transform.position.y -= 10.0f;
	m_groundObj.lock()->m_transform.scale = { 100.0f, 1.0f, 100.0f };

	std::shared_ptr<CWindBox> windBoxObj = Instantiate(std::make_shared<CWindBox>());
	windBoxObj->AttachTarget(m_clothObj.lock());
}

void CPhysicSimulationScene::BeginPlay()
{
	__super::BeginPlay();
	
	// Set the ground object of the cloth
	m_clothObj.lock()->GetComponent<CClothComponent>()->SetGroundObject(m_groundObj.lock());

}

void CPhysicSimulationScene::UpdateScene(float _deltaTime)
{
	__super::UpdateScene(_deltaTime);
	
	CInput* g_Input = CInput::Get();

	// All the control for the cloth
	auto* clothComponent = m_clothObj.lock()->GetComponent<CClothComponent>();
	// Width and height of the cloth
	int clothWidth = clothComponent->m_width;
	int clothHeight = clothComponent->m_height;

	if (g_Input->g_cKeyState[(unsigned char)'g'] == INPUT_FIRST_PRESS)
	{
		clothComponent->ReleaseEntireCloth();
	}
	else if (g_Input->g_cKeyState[(unsigned char)'h'] == INPUT_FIRST_PRESS)
	{
		clothComponent->RebuildCloth();
	}
	// Decrease width by 1
	else if (g_Input->g_cKeyState[(unsigned char)'j'] == INPUT_FIRST_PRESS)
	{
		if (clothWidth - 1 >= 2)
		{
			clothComponent->ResizeCloth(clothWidth - 1, clothHeight);
		}
	}
	// Increase width by 1
	else if (g_Input->g_cKeyState[(unsigned char)'l'] == INPUT_FIRST_PRESS)
	{
		clothComponent->ResizeCloth(clothWidth + 1, clothHeight);
	}
	// Decrease height by 1
	else if (g_Input->g_cKeyState[(unsigned char)'k'] == INPUT_FIRST_PRESS)
	{
		if (clothHeight - 1 >= 2)
		{
			clothComponent->ResizeCloth(clothWidth, clothHeight - 1);
		}
	}
	// Increase height by 1
	else if (g_Input->g_cKeyState[(unsigned char)'i'] == INPUT_FIRST_PRESS)
	{
		clothComponent->ResizeCloth(clothWidth, clothHeight + 1);
	}
}
