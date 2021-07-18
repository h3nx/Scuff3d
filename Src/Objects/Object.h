#pragma once
#include "Component.h"
#include <map>

namespace scuff3d
{
	class ComponentBase;
	class Transform;

	class GameObject
	{
	public:
		
		GameObject(const std::string& name, const glm::vec3& translation, Transform* parent);
		GameObject(const std::string& name = "GameObject",
			const glm::vec3& translation = { 0.0f, 0.0f, 0.0f },
			const glm::vec3& rotation = { 0.0f, 0.0f, 0.0f },
			const glm::vec3& scale = { 1.0f, 1.0f, 1.0f },
			Transform* parent = nullptr);
		virtual ~GameObject();

		template<typename ComponentType, typename... Targs>
		ComponentType* addComponent(Targs... args);
		template<typename ComponentType>
		ComponentType* getComponent();


	private:
		const size_t m_id;
		std::string m_name;

		std::map<int, ComponentBase*> m_components;
	};

	template<typename ComponentType, typename... Targs>
	ComponentType* GameObject::addComponent(Targs... args) {
		if (m_components.find(ComponentType::ID) == m_components.end()) {
			m_components[ComponentType::ID] = new ComponentType(args...);
			ComponentBase* ptr = m_components[ComponentType::ID];
			ptr->setObject(this);
		}
		// Return pointer to the component
		return static_cast<ComponentType*>(m_components[ComponentType::ID]);
	}
	template<typename ComponentType>
	ComponentType* GameObject::getComponent() {
		if (m_components.find(ComponentType::ID) == m_components.end()) {
			return nullptr;
		}
		// Return pointer to the component
		return static_cast<ComponentType*>(m_components[ComponentType::ID]);
	}
}

