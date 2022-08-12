#pragma once
#include <map>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "Utils/Helpers.h"
#pragma message("compiling object")
namespace scuff3d
{
	class ComponentBase;
	class Transform;
	class ECS;

	class GameObject
	{
	public:
		friend class Scene;
		friend class ECS;
	private:
		GameObject(ECS* ecs, const std::string& name = "GameObject",
			const ::glm::vec3& translation = { 0.0f, 0.0f, 0.0f },
			const ::glm::vec3& rotation = { 0.0f, 0.0f, 0.0f },
			const ::glm::vec3& scale = { 1.0f, 1.0f, 1.0f },
			Transform* parent = nullptr
		);
		GameObject(ECS* ecs, const std::string& name = "GameObject",
			const ::glm::vec3& translation = { 0.0f, 0.0f, 0.0f },
			const ::glm::vec3& rotation = { 0.0f, 0.0f, 0.0f },
			const ::glm::vec3& scale = { 1.0f, 1.0f, 1.0f },
			GameObject* parent = nullptr
		);
		/*GameObject(ECS* ecs, const std::string& name = "GameObject",
			const ::glm::vec3& translation = { 0.0f, 0.0f, 0.0f },
			const ::glm::vec4& quat = { 0.0f, 0.0f, 0.0f, 1.0f },
			const ::glm::vec3& scale = { 1.0f, 1.0f, 1.0f },
			Transform* parent = nullptr
		); 
		GameObject(ECS* ecs, const std::string& name = "GameObject",
			const ::glm::vec3& translation = { 0.0f, 0.0f, 0.0f },
			const ::glm::quat& quat = { 0.0f, 0.0f, 0.0f, 1.0f },
			const ::glm::vec3& scale = { 1.0f, 1.0f, 1.0f },
			Transform* parent = nullptr
		);*/
		GameObject(ECS* ecs, const std::string& name = "GameObject",
			Transform* parent = nullptr,
			const ::glm::vec3& translation = { 0.0f, 0.0f, 0.0f },
			const ::glm::vec3& rotation = { 0.0f, 0.0f, 0.0f },
			const ::glm::vec3& scale = { 1.0f, 1.0f, 1.0f }
		);
		GameObject(ECS* ecs, const std::string& name = "GameObject",
			GameObject* parent = nullptr,
			const ::glm::vec3& translation = { 0.0f, 0.0f, 0.0f },
			const ::glm::vec3& rotation = { 0.0f, 0.0f, 0.0f },
			const ::glm::vec3& scale = { 1.0f, 1.0f, 1.0f }
		);

	public:

		template<typename ComponentType, typename... Targs>
		ComponentType* addComponent(Targs... args);
		template<typename ComponentType>
		ComponentType* getComponent();
		//template<typename ComponentType>
		//void removeComponent();

		friend bool operator == (const GameObject* l, const std::string& r) noexcept {
			return l->m_name == r;
		};
		const std::string& getName() const;
		const size_t& getID() const;


		void destroy();
		void destroyImmediate();


		void renderImGui();
	protected:
		virtual ~GameObject();

	private:
		const size_t m_id;
		std::string m_name;

		std::map<int, ComponentBase*> m_components;
		ECS* m_ecs;
		bool m_destroying;

		void registerComponent(const int id);
	};

	template<typename ComponentType, typename... Targs>
	inline ComponentType* GameObject::addComponent(Targs... args) {
		if (m_components.find(ComponentType::ID) == m_components.end()) {
			m_components[ComponentType::ID] = NEW ComponentType(args...);
			ComponentBase* ptr = m_components[ComponentType::ID];
			ptr->setObject(this);
			ptr->init();
			registerComponent(ComponentType::ID);
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

