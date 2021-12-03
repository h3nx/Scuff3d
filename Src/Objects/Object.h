#pragma once
#include <map>
#include "glm/glm.hpp"
#include "Utils/Helpers.h"
#pragma message("compiling object")
namespace scuff3d
{
	class ComponentBase;
	class Transform;

	class GameObject
	{
	public:
		friend class Scene;
		GameObject(const std::string& name, GameObject* parent);
		GameObject(const std::string& name, Transform* parent);
		GameObject(const std::string& name, const ::glm::vec3& translation, Transform* parent);
		GameObject(const std::string& name = "GameObject",
			const ::glm::vec3& translation = { 0.0f, 0.0f, 0.0f },
			const ::glm::vec3& rotation = { 0.0f, 0.0f, 0.0f },
			const ::glm::vec3& scale = { 1.0f, 1.0f, 1.0f },
			Transform* parent = nullptr);
		virtual ~GameObject();

		template<typename ComponentType, typename... Targs>
		ComponentType* addComponent(Targs... args);
		template<typename ComponentType>
		ComponentType* getComponent();
		

		friend bool operator == (const GameObject* l, const std::string& r) noexcept {
			return l->m_name == r;
		};
		const std::string& getName() const;
		const size_t& getID() const;

		void renderImGui();

	private:
		const size_t m_id;
		std::string m_name;

		std::map<int, ComponentBase*> m_components;
	};

	template<typename ComponentType, typename... Targs>
	inline ComponentType* GameObject::addComponent(Targs... args) {
		if (m_components.find(ComponentType::ID) == m_components.end()) {
			m_components[ComponentType::ID] = NEW ComponentType(args...);
			ComponentBase* ptr = m_components[ComponentType::ID];
			ptr->setObject(this);
			ptr->init();
		}
		// Return pointer to the component
		return static_cast<ComponentType*>(m_components[ComponentType::ID]);
	}
	template<typename ComponentType>
	inline ComponentType* GameObject::getComponent() {
		if (m_components.find(ComponentType::ID) == m_components.end()) {
			return nullptr;
		}
		// Return pointer to the component
		return static_cast<ComponentType*>(m_components[ComponentType::ID]);
	}
}

