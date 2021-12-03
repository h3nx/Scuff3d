#pragma once
#include <string>
#include <functional>
#include <map>
#include <queue>
#include "Application.h"
#include "Objects/Components.h"
//


namespace scuff3d {

	class GameObject;
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

		// add usermade object
		GameObject* addObject(GameObject* obj);
		// add empty object with <name> at (0,0,0)
		GameObject* addObject(const std::string& name, GameObject* parent = nullptr);
		void removeObject(const std::string& name);
		void removeObject(const size_t& id);
		void removeObject(GameObject* object);

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
		
		std::map<size_t, GameObject*> m_gameObjects;
		std::queue<size_t> m_addedGameObjects;
		std::queue<size_t> m_deletedGameObjects;

	private:
		const bool exists(const size_t& id) const;

	};

}