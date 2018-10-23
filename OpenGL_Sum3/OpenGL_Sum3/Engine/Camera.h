#pragma once

// Global Include
#include "Utility.h"

enum ECAMERATYPE
{
	ORTHOGRAPHIC,
	PERSPECTIVE,
};

class CCamera
{
public:

	CCamera();
	~CCamera();

	void Update();
	void BeginPlay();

	glm::mat4 GetView() const;
	void CalcViewMatrix();
	glm::mat4 GetProj() const;
	void CalcProjectionMatrix();

	glm::vec3 GetCameraFacing() const;
	void SetCameraFacing(glm::vec3 _Facing);

	glm::vec3 GetCameraNormal() const;
	void SetCameraNormal(glm::vec3 _Normal);
	
	/** Configuration for the camera */
public: 

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraFacing;
	glm::vec3 m_cameraNormal;

	float m_viewPortWidth;
	float m_viewPortHeight;

	bool m_bPerspective;
	float m_fov;
	float m_nearPlane;
	float m_farPlane;

	bool m_bIsControlling;

	/** Member variables */
private: 

	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;
};