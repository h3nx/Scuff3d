#include "pch.h"
#include "Object.h"
#include "Components/Transform.h"

namespace scuff3d
{
	static size_t objectIdCounter = 0;

	GameObject::GameObject(const std::string& name, const glm::vec3& translation, Transform* parent) :
		m_id(objectIdCounter++),
		m_name(name)
	{ 
		addComponent<Transform>(translation, parent);
	}
	GameObject::GameObject(const std::string& name, const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale, Transform* parent) :
		m_id(objectIdCounter++),
		m_name(name)
	{
		addComponent<Transform>(translation, rotation, scale, parent);
	}
	GameObject::~GameObject()
	{ }


	



}