#pragma once

// Global Includes
#include "Utility.h"

// Forward Declare
class CScene;
class CGameObject;
class CComponent;
struct Trasform;

// Declare a Transform struct
struct Transform
{
	CGameObject* gameObject;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec3 GetForward() const 
	{
		float radX = glm::radians(rotation.x);
		float radY = glm::radians(rotation.y);

		glm::vec3 forwardVec(
			glm::cos(radX) * glm::sin(radY),
			glm::sin(radX),
			-glm::cos(radX) * glm::cos(radY)
		);

		return glm::normalize(forwardVec);
	}
	glm::vec3 GetUp() const 
	{
		return -glm::cross(this->GetForward(), this->GetRight());
	}
	glm::vec3 GetRight() const 
	{
		//float radX = glm::radians(rotation.x);
		float radY = glm::radians(rotation.y + 90.0f);

		glm::vec3 rightVec(
			glm::sin(radY),
			0.0f,
			-glm::cos(radY)
		);

		return glm::normalize(rightVec);

		//return glm::cross({ 0.0f, -1.0f, 0.0f }, this->GetForward());
	}
};

class CGameObject : public std::enable_shared_from_this<CGameObject>
{
public:
	CGameObject();
	~CGameObject();

	// Properties
	std::string m_tag;
	std::string m_name;
	Transform m_transform;

protected:
	
	bool m_shouldDestroyed;
	bool m_isActive;
	
	CScene* m_scene;

	std::vector<CComponent*> m_components;

public:
	/**
	* Initialize the object
	* Call right after the scene initialize
	*/
	virtual void BeginPlay();
	/*
	 * Call every frame
	 */
	virtual void Update();
	/**
	* Creates a component and push to the vector
	*/
	template<typename T>
	T* CreateComponent();
	/**
	* Try get the component of the gameobject
	*/
	template<typename T>
	T* GetComponent() const;
	/*
	 *Check if the object should be destroyed on thie frame
	 */
	bool ShouldDestroyed() const;
	/*
	 *Destroy current gameobject and set it to inactive 
	 *of the garbage cleaning next frame
	 */
	void DestroyObject();
    /*
	 *Check if the obejct is active which determent it should update or not
	 */
	bool IsActive() const;
	/*
	 *Set active state for a object
	 */
	void SetActive(bool);
	/**
	 * Get and Set the scene pointer for the gameobject
	 */
	void SetScene(CScene* _scene);
	CScene* GetScene() const;

};

template<typename T>
T* CGameObject::CreateComponent()
{
	T* newComponent = new T();
	newComponent->SetOwner(this);

	// Check if the generic type is actually a component based class
	if (dynamic_cast<T*>(newComponent) != nullptr)
	{
		// Push into the componet container for operation
		m_components.push_back(newComponent);
		return newComponent;
	}
	else { return nullptr; } // Returns a null pointer to show its not a valid type
}

template<typename T>
T* CGameObject::GetComponent() const
{
	for (CComponent* iter : m_components)
	{
		T* component = dynamic_cast<T*>(iter);
		if (component != nullptr)
		{
			return component;
		}
	}

	return nullptr;
}