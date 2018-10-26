
// Derive Include
#include "Engine/Scene.h"

// Forward Declaration
class CCube;
class CTerrainComponent;

class CTestScene : public CScene
{
public:

	virtual void ConfigurateScene() override;
	virtual void BeginPlay() override;
	virtual void UpdateScene() override;


private:

	std::weak_ptr<CCube> m_cubeOBJ;
	std::weak_ptr<CGameObject> m_terrain;

};