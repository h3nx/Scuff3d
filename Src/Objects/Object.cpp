#include "pch.h"
#include "Object.h"
#include "Components/Transform.h"

#include "imgui/imgui.h"

namespace scuff3d
{
	size_t objectIdCounter = 0;

	GameObject::GameObject(const std::string& name, GameObject* parent) :
		m_id(objectIdCounter++),
		m_name(name)
	{ 
		addComponent<Transform>(parent == nullptr? nullptr : parent->getComponent<Transform>());
	}

	GameObject::GameObject(const std::string& name, Transform* parent) :
		m_id(objectIdCounter++),
		m_name(name)
	{
		addComponent<Transform>(parent);
	}

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
	GameObject::~GameObject() { 
		for (auto& pair : m_components) {
			if (pair.second)
				delete pair.second;
			pair.second = nullptr;
		}
	}

	const std::string& GameObject::getName() const {
		return m_name;
	}
	const size_t& GameObject::getID() const {
		return m_id;
	}

	void GameObject::renderImGui() {
		for (auto& pair : m_components) {
			
			if (ImGui::CollapsingHeader((std::string(typeid(*pair.second).name())+"("+std::to_string(pair.first)+")").c_str())) {
				pair.second->renderImGui();

			}
		}
	}

}