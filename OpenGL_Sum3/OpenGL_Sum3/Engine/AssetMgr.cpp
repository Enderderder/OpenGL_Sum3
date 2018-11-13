
// Engine Include
#include "Engine.h"

#pragma region Singleton

// Static Variable
CAssetMgr* CAssetMgr::s_pAssetMgr = nullptr;

CAssetMgr* CAssetMgr::GetInstance()
{
	if (s_pAssetMgr == nullptr)
	{
		s_pAssetMgr = new CAssetMgr();
	}

	return s_pAssetMgr;
}

void CAssetMgr::DestroyInstance()
{
	delete s_pAssetMgr;
	s_pAssetMgr = nullptr;
}

CAssetMgr::CAssetMgr() {}
CAssetMgr::~CAssetMgr() {}

#pragma endregion Singleton

void CAssetMgr::InitializeAssets()
{
	/** Initialize Programs */
	CreateProgram("DefaultSpriteProgram", "Engine/Shaders/Sprite.vs", "Engine/Shaders/Sprite.fs");
	CreateProgram("UnlitProgram", "Engine/Shaders/Unlit.vs", "Engine/Shaders/Unlit.fs");
	CreateProgram("BlinnPhongProgram", "Engine/Shaders/BlinnPhong.vs", "Engine/Shaders/BlinnPhong.fs");
	CreateProgram("CubeMapProgram", "Engine/Shaders/CubeMap.vs", "Engine/Shaders/CubeMap.fs");
	CreateProgram("TextProgram", "Engine/Shaders/Text.vs", "Engine/Shaders/Text.fs");
	CreateProgram("GeometryProgram", "Engine/Shaders/Geo.vs", "Engine/Shaders/Geo.fs", "Engine/Shaders/Geo.gs");
	CreateProgram("CPUParticleProgram", "Engine/Shaders/CPUParticle.vs", "Engine/Shaders/CPUParticle.fs", "Engine/Shaders/CPUParticle.gs");
	CreateProgram("AnimationProgram", "Engine/Shaders/Animation.vs", "Engine/Shaders/Animation.fs");

	/** Initialize Meshes */
	CreateMesh("DefaultCubeMesh", new CCubeMesh());

	/** Initialize Textures */
	CreateTexture("Box", "Resources/Textures/Box.png");

	/** Initialize cube maps */
	CreateCubeMap("DefaultCubeMap", std::vector<std::string>{
		"DefaultCubeMap/right.jpg", "DefaultCubeMap/left.jpg", "DefaultCubeMap/top.jpg",
			"DefaultCubeMap/bottom.jpg", "DefaultCubeMap/back.jpg", "DefaultCubeMap/front.jpg"});


	/** Initialize Sprites */
	CreateSprite("Block", "Resources/Sprites/Block.png");
}

CSprite* CAssetMgr::GetSprite(std::string _name) const
{
	for (auto iter = m_spriteMap.begin(); iter != m_spriteMap.end(); ++iter)
	{
		if (iter->first == _name)
		{
			return iter->second;
		}
	}

	CDebug::Log("Unable to grab sprite from name.");
	return nullptr;
}

GLuint CAssetMgr::GetProgramID(std::string _name) const
{
	for (const auto& iter : m_programMap)
	{
		if (iter.first == _name)
		{
			return iter.second;
		}
	}

	CDebug::Log("Unable to grab program ID from name.");
	return NULL;
}

CMesh* CAssetMgr::GetMesh(std::string _name) const
{
	for (const auto& iter : m_meshMap)
	{
		if (iter.first == _name)
		{
			return iter.second;
		}
	}

	CDebug::Log("Unable to grab mesh from name.");
	return nullptr;
}

GLuint CAssetMgr::GetTexture(std::string _name) const
{
	for (const auto& iter : m_textureMap)
	{
		if (iter.first == _name)
		{
			return iter.second;
		}
	}

	CDebug::Log("Unable to grab texture from name.");
	return NULL;
}

CCubeMap* CAssetMgr::GetCubeMap(std::string _name) const
{
	for (const auto& iter : m_cubemapMap)
	{
		if (iter.first == _name)
		{
			return iter.second;
		}
	}

	CDebug::Log("Unable to grab cube map from name.");
	return nullptr;
}

void CAssetMgr::CreateProgram(std::string _name, const char* _vertexPath, const char* _fragmentPath)
{
	GLuint newProgram = ShaderLoader::CreateProgram(_vertexPath, _fragmentPath);

	if (newProgram != NULL)
	{
		m_programMap.insert(std::pair<std::string, GLuint>(_name, newProgram));
		CDebug::Log("Loaded program: " + _name);
	}
	else
	{
		CDebug::Log("Loaded program \"" + _name + "\" failed");
	}
}

void CAssetMgr::CreateProgram(std::string _name, const char* _vertexPath, const char* _fragmentPath, const char* _geometryPath)
{
	GLuint newProgram = ShaderLoader::CreateProgram(_vertexPath, _fragmentPath, _geometryPath);
	
	if (newProgram != NULL)
	{
		m_programMap.insert(std::pair<std::string, GLuint>(_name, newProgram));
		CDebug::Log("Loaded program: " + _name);
	}
	else
	{
		CDebug::Log("Loaded program \"" + _name + "\" failed");
	}
}

void CAssetMgr::CreateSprite(std::string _name, const char* _pathName)
{
	CSprite* newSprite = new CSprite();
	newSprite->CreateSprite(_pathName);

	m_spriteMap.insert(std::pair<std::string, CSprite*>(_name, newSprite));
	CDebug::Log("Loaded sprite: " + _name);
}

void CAssetMgr::CreateMesh(std::string _name, CMesh* _mesh)
{
	m_meshMap.insert(std::pair<std::string, CMesh*>(_name, _mesh));
	CDebug::Log("Loaded mesh: " + _name);
}

void CAssetMgr::CreateTexture(std::string _name, const char* _pathName)
{
	// Try Load the image into a buffer
	int width, height, channel;
	unsigned char* image = SOIL_load_image(
		_pathName,			// File path / Name
		&width,				// Output for the image width
		&height,			// Output for the image height
		&channel,			// Output for number of channels
		SOIL_LOAD_RGBA);	// Load RGBA values only

	if (image != nullptr) // Check if the texture exists
	{
		/** Bind the texture */
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(
			GL_TEXTURE_2D,		// Type of texture
			0,					// Mipmap level, 0 for base
			GL_RGBA,			// Number of color components in texture
			width,				// Width of the texture
			height,				// Height of the texture
			0,					// This value must be 0
			GL_RGBA,			// Format for the pixel data
			GL_UNSIGNED_BYTE,	// Data type for the pixel data
			image);				// Pointer to image data in memory

		glGenerateMipmap(GL_TEXTURE_2D);

		// Unbind and free the data
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		/************************************************************************/

		//Load into the map
		m_textureMap.insert(std::pair<std::string, GLuint>(_name, texture));
		CDebug::Log("Loaded texture: " + _name);
	}
	else
	{
		CDebug::Log("Loaded texture \"" + _name + "\" failed");
	}
}

void CAssetMgr::CreateCubeMap(std::string _name, std::vector<std::string> _pathNames)
{
	// Create the new cube map using the path names
	CCubeMap* newCubeMap = new CCubeMap(_pathNames);

	//Load into the map
	m_cubemapMap.insert(std::pair<std::string, CCubeMap*>(_name, newCubeMap));
	CDebug::Log("Loaded cubemap: " + _name);
}
