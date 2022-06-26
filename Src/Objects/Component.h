#pragma once
//#include "Object.h"
//#pragma message("compiling Component")
#include "Rendering/API/Renderer.h"


namespace scuff3d
{
	extern int componentID;

	class GameObject;
	class ComponentBase {
	public:
		friend class GameObject;
		virtual ~ComponentBase() {};
		static const int createID() {
			return componentID++;
		};
		virtual void init() { };
		virtual void destroy() { };
		virtual void render(const float& dt, Renderer* renderer) { };
		virtual void preUpdate(const float& dt) { };
		virtual void update(const float& dt) { };
		virtual void postUpdate(const float& dt) { };
		virtual void preFixedUpdate(const float& dt) { };
		virtual void fixedUpdate(const float& dt) { };
		virtual void postFixedUpdate(const float& dt) { };

		const bool isActive() const {
			return m_active;
		};
		const void setActive(const bool status) {
			m_active = status;
		};
		GameObject* getGameObject() const { return m_obj; };

		virtual void renderImGui() {};
	protected:
		ComponentBase():
			m_active(true), 
			m_obj(nullptr) 
		{};
		void setObject(GameObject* obj) {
			m_obj = obj;
		};
	private:
		bool m_active;
		GameObject* m_obj;
	};




	template<typename ComponentType>
	class Component : public ComponentBase
	{
	public:
		~Component() {};
		static const int ID;
		const std::string getName() {
			return typeid(ComponentType).name();
		}
	protected:
		Component() {};
	private:
		 
	};




	template<typename ComponentType>
	const int Component<ComponentType>::ID = ComponentBase::createID();
}



