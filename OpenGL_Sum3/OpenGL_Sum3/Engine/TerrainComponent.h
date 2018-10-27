#pragma once

// Inherited Includes
#include "Component.h"

// Forward Declare
class CCamera;

struct HeightMapInfo
{
	const char* heightmapFilename;
	float heightScale = 1.0f;
	float heightOffset = 0.0f;
	unsigned int width;
	unsigned int height;
	float cellSpacing = 2.0f;
};

struct TerrainVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

class CTerrainComponent : public CComponent
{

public:

	CTerrainComponent();
	virtual ~CTerrainComponent();

	virtual void BeginPlay() override;
	virtual void Update(float _tick);

	/**
	 * Load the height map in using a RAW file
	 */
	void LoadHeightMap();

	/**
	* Smooth the height map using average function
	*/
	void SmoothHeightMap();

	/**
	 * Create the terrain shape using the loaded height map
	 */
	void CreateTerrain(HeightMapInfo& _info);

	/**
	 * Render function of the terrain
	 */
	void RenderTerrain(CCamera* _camera);

	/**
	 * Get the height of the terrain at certain point
	 */
	float GetHeight(float _x, float _z) const;

	float GetWidth() const;

	float GetDepth() const;

	/**
	* Check if the point is inside the height map
	*/
	bool InBounds(int i, int j);

private:

	/**
	 * Compute the average element value of the point
	 */
	float Average(int i, int j);

private:

	HeightMapInfo m_hmInfo;

	UINT m_numVertices;
	UINT m_numFaces;

	// height of each vertices in the height map
	std::vector<float> m_heightMap;

	// OpenGL rendering data
	GLuint m_vao;
	GLuint m_indiceCount;
	GLuint m_program;

};