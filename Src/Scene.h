#pragma once
#include <string>
#include <functional>
#include <map>
#include <queue>
#include "Application.h"
#include "Objects/ECS.h"
//


namespace scuff3d {

	class GameObject;
	class Scene : public scuff3d::ECS
	{
		friend class Application;
	public:
		Scene(const std::string& name = "Scene");
		virtual ~Scene();

		virtual bool init();

		virtual void preUpdate(const float dt);
		virtual void update(const float dt);
		virtual void postUpdate(const float dt);

		virtual void preFixedUpdate(const float dt);
		virtual void fixedUpdate(const float dt);
		virtual void postFixedUpdate(const float dt);

		virtual void preRender();
		virtual void render();
		virtual void postRender();

		


		GameObject* find(const std::string& name);
		GameObject* find(const size_t& id);

		const std::string& getName() const;

		const bool isActive() const;
		void setActive(const bool status);
		void pause();
		void resume();

	protected:
		std::string m_name;
		Application* m_app;
		bool m_active;
		

	private:
		const bool exists(const size_t& id) const;

	};

}