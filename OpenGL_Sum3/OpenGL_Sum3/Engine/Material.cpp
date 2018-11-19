
// Engine Include
#include "Engine.h"

// Get manager class
static CAssetMgr* p_assetMgr = CAssetMgr::GetInstance();

CMaterial::CMaterial()
{
	m_name = "DefaultShader";
	m_shaderProgram = p_assetMgr->GetProgramID("BlinnPhongProgram");
}

CMaterial::~CMaterial()
{

}

void CMaterial::BindProgramData()
{

}
