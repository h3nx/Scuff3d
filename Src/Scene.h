#pragma once
#include <string>
#include <functional>
#include <map>
#include "Application.h"


namespace scuff3d {
	
	class Scene
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

		void addObject(const std::string& name, int object);
		void removeObject(const std::string& name, int object);


		const std::string& getName() const;
		const bool isActive() const;
		void setActive(const bool status);
		void pause();
		void resume();

	protected:
		std::string m_name;
		Application* m_app;
		bool m_active;
		//TODO: change to gameobject
		std::map<std::string, int> m_gameObjects;

	};

}