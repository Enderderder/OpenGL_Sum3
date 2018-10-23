#pragma once

// Global Includes
#include "Utility.h"

// Forward Declare
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
	
	std::vector<std::shared_ptr<CComponent>> m_components;

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
	std::shared_ptr<T> CreateComponent();
	/**
	* Try get the component of the gameobject
	*/
	template<typename T>
	std::shared_ptr<T> GetComponent() const;
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

};

template<typename T>
std::shared_ptr<T> CGameObject::CreateComponent()
{
	std::shared_ptr<T> newComponent
		= std::make_shared<T>();
	newComponent->SetOwner(this);

	// Check if the generic type is actually a component based class
	if (std::dynamic_pointer_cast<T>(newComponent))
	{
		// Push into the componet container for operation
		m_components.push_back(newComponent);
		return newComponent;
	}
	else { return nullptr; } // Returns a null pointer to show its not a valid type
}

template<typename T>
std::shared_ptr<T> CGameObject::GetComponent() const
{
	for (std::shared_ptr<CComponent> iter : m_components)
	{
		std::shared_ptr<T> component = std::dynamic_pointer_cast<T>(iter);
		if (component != nullptr)
		{
			return component;
		}
	}

	return nullptr;
}