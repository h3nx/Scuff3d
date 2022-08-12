#include "pch.h"
#include "Object.h"
#include "Components/Transform.h"

#include "imgui/imgui.h"
#include "ECS.h"

namespace scuff3d
{
	size_t objectIdCounter = 0;


	

	/*GameObject::GameObject(const std::string& name, GameObject* parent) :
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
	}*/
	
	GameObject::GameObject(ECS* ecs, const std::string& name, const::glm::vec3& translation, const::glm::vec3& rotation, const::glm::vec3& scale, Transform* parent) 
		: m_ecs(ecs)
		, m_name(name)
		, m_id(objectIdCounter++)
		, m_destroying(false)
	{
		addComponent<Transform>(translation, rotation, scale, parent);
	}
	GameObject::GameObject(ECS* ecs, const std::string& name, const::glm::vec3& translation, const::glm::vec3& rotation, const::glm::vec3& scale, GameObject* parent) 
		: GameObject(ecs, name, translation, rotation, scale, parent ? parent->getComponent<Transform>() : nullptr)
	{ }
	/*GameObject::GameObject(ECS* ecs, const std::string& name, const::glm::vec3& translation, const::glm::vec4& quat, const::glm::vec3& scale, Transform* parent) {
	}
	GameObject::GameObject(ECS* ecs, const std::string& name, const::glm::vec3& translation, const::glm::quat& quat, const::glm::vec3& scale, Transform* parent) {
	}*/
	GameObject::GameObject(ECS* ecs, const std::string& name, Transform* parent, const::glm::vec3& translation, const::glm::vec3& rotation, const::glm::vec3& scale)
		: GameObject(ecs, name, translation, rotation, scale, parent)
	{ }
	GameObject::GameObject(ECS * ecs, const std::string & name, GameObject * parent, const::glm::vec3 & translation, const::glm::vec3 & rotation, const::glm::vec3 & scale) 
		: GameObject(ecs, name, translation, rotation, scale, parent)
	{ }
	GameObject::~GameObject() {
		m_ecs->registerObjectRemoval(m_id);
		for (auto& pair : m_components) {
			m_ecs->registerComponentRemoval(pair.first, this);
			safeDelete(pair.second);
		}
	}

	const std::string& GameObject::getName() const {
		return m_name;
	}
	const size_t& GameObject::getID() const {
		return m_id;
	}

	void GameObject::destroy() {
		m_ecs->requestDestroy(this);
	}

	void GameObject::destroyImmediate() {
		m_ecs->requestDestroy(this);
	}

	void GameObject::renderImGui() {
		for (auto& pair : m_components) {
			
			if (ImGui::CollapsingHeader((std::string(typeid(*pair.second).name())+"("+std::to_string(pair.first)+")").c_str())) {
				pair.second->renderImGui();

			}
		}
	}

	void GameObject::registerComponent(const int id) {
		m_ecs->registerComponent(id, this);
	}
	
}