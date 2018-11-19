#pragma once

// Global Include
#include "Utility.h"

// Forward Declare
class CSprite;
class CMesh;
class CCubeMap;
class Text;

class CAssetMgr
{

#pragma region Singleton
public:

	static CAssetMgr* GetInstance();
	static void DestroyInstance();

private:
	CAssetMgr();
	~CAssetMgr();

	CAssetMgr(const CAssetMgr& _kr) = delete;
	CAssetMgr& operator= (const CAssetMgr& _kr) = delete;

protected:
	static CAssetMgr* s_pAssetMgr;

#pragma endregion

public:

	void InitializeAssets();

	CSprite* GetSprite(std::string _name) const;
	GLuint GetProgramID(std::string _name) const;
	CMesh* GetMesh(std::string _name) const;
	GLuint GetTexture(std::string _name) const;
	CCubeMap* GetCubeMap(std::string _name) const;
	Text* GetFont(std::string _name) const;

private:

	/**
	 * Creates shader program and bind it into a map
	 */
	void CreateProgram(std::string _name, const char* _vertexPath, const char* _fragmentPath);
	void CreateProgram(std::string _name, const char* _vertexPath, const char* _fragmentPath, const char* _geometryPath);
	/**
	* Creates sprite and bind it into a map
	*/
	void CreateSprite(std::string _name, const char* _pathName);
	/**
	* Creates mesh and bind it into a map
	*/
	void CreateMesh(std::string _name, CMesh* _mesh);
	/**
	* Creates texture and bind it into a map
	*/
	void CreateTexture(std::string _name, const char* _pathName);
	/**
	* Creates cubemap and bind it into a map
	*/
	void CreateCubeMap(std::string _name, std::vector<std::string> _pathNames);
	/**
	* Creates font and bind it into a map
	*/
	void CreateFontType(std::string _name, const char* _pathName);

private:
	/** Map collection of sprites with a string name */
	std::map<std::string, CSprite*> m_spriteMap;

	/** Map collection of meshes */
	std::map<std::string, CMesh*> m_meshMap;

	/** Map collection of programs with a string name */
	std::map<std::string, GLuint> m_programMap;

	/** Map collection of textures with a string name */
	std::map<std::string, GLuint> m_textureMap;

	/** Map collection of cubemap with a string name */
	std::map<std::string, CCubeMap*> m_cubemapMap;

	/** Map collection of cubemap with a string name */
	std::map<std::string, Text*> m_fontMap;
};

