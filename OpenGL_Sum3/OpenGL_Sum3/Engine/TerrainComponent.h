#pragma once

// Inherited Includes
#include "Component.h"

// Forward Declare
class CCamera;

struct SHeightMapInfo
{
	const char* heightmapFilename;
	float heightScale = 1.0f;
	float heightOffset = 0.0f;
	unsigned int width;
	unsigned int height;
	float cellSpacing = 2.0f;
	float smoothness = 0.5f;
};

struct STerrainVertex
{
	glm::vec3 position;
	glm::vec3 normal;
};

class CTerrainComponent : public CComponent
{

public:

	CTerrainComponent();
	virtual ~CTerrainComponent();

	virtual void BeginPlay() override;
	virtual void Update() override;

	/** Create the terrain shape using the loaded height map */
	void CreateTerrain(SHeightMapInfo& _info);

	/** Render function of the terrain */
	void RenderTerrain(CCamera* _camera);

	/** Get the height of the terrain at certain point */
	float GetHeight(float _x, float _z) const;

	/** Get the height of the terrian using grid value */
	float GetGridHeight(int _row, int _col) const;

	float GetWidth() const;

	float GetDepth() const;

	/** Check if the point is inside the height map */
	bool InGridBounds(int i, int j);

private:

	/** Load the height map in using a RAW file */
	void LoadHeightMap();

	/** Calculate the normal value of the terrain */
	void CalculateNormal();

	/** Smooth the height map using average function */
	void SmoothHeightMap();

	/** Compute the average element value of the point */
	float Average(int i, int j);

private:

	SHeightMapInfo m_hmInfo;

	// height of each vertices in the height map
	std::vector<float> m_heightMap;

	// OpenGL rendering data
	std::vector<STerrainVertex> m_terrainVertex;
	GLuint m_vao;
	GLuint m_indiceCount;
	GLuint m_program;

};