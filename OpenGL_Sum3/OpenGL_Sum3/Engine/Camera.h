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

private:

	/**
	 * Calculate the ray from the mouse coord
	 */
	void CalculateCameraRay();
		
	/** Configuration for the camera */
public: 

	Transform m_transform;

	//glm::vec3 m_cameraPosition;
	//glm::vec3 m_cameraFacing;
	//glm::vec3 m_cameraNormal;

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

	glm::vec3 m_cameraRay;
};