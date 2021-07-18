#pragma once
#include "Object.h"
#pragma message("compiling Component")
namespace scuff3d
{
	class GameObject;
	class ComponentBase {
	public:
		friend class GameObject;
		virtual ~ComponentBase() {};
		static const int createID();
		virtual void init();
		virtual void destroy();
		virtual void preUpdate(const float& dt);
		virtual void update(const float& dt);
		virtual void postUpdate(const float& dt);
		virtual void preFixedUpdate(const float& dt);
		virtual void fixedUpdate(const float& dt);
		virtual void postFixedUpdate(const float& dt);

		const bool isActive() const;
		const void setActive(const bool status);
	protected:
		ComponentBase():m_active(true), m_obj(nullptr) {};
		void setObject(GameObject * obj);
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

	protected:
		Component() {};
	private:



	};




	template<typename ComponentType>
	const int Component<ComponentType>::ID = ComponentBase::createID();

}



