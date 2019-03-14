
// Engine Include
#include "Engine.h"

// Get manager class
static CAssetMgr* p_assetMgr = CAssetMgr::Get();

CMaterial::CMaterial()
{
	m_name = "EmptyMaterial";
}

CMaterial::~CMaterial() {}

void CMaterial::BindProgramData() {}
