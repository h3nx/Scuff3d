#include "pch.h"
#include "Scene.h"
#include <algorithm>
//#include "Objects/Object.h"

namespace scuff3d {

	Scene::Scene(const std::string& name) :
		m_name(name)
	{ }

	Scene::~Scene() {
		for (auto& pair : m_gameObjects) {
			if (pair.second)
				delete pair.second;
			pair.second = nullptr;
		}
	}

	bool Scene::init() {
		return true;
	}

	void Scene::preUpdate(const float dt) {
		for (auto& pair : m_gameObjects) {
			GameObject* object = pair.second;
			//Transform* transform = object->getComponent<Transform>();
			for (auto& compPair : object->m_components) {
				compPair.second->preUpdate(dt);
			}
		}
	}

	void Scene::update(const float dt) {
		for (auto& pair : m_gameObjects) {
			GameObject* object = pair.second;
			//Transform* transform = object->getComponent<Transform>();
			for (auto& compPair : object->m_components) {
				compPair.second->update(dt);
			}
		}
	}

	void Scene::postUpdate(const float dt) {
		for (auto& pair : m_gameObjects) {
			GameObject* object = pair.second;
			//Transform* transform = object->getComponent<Transform>();
			for (auto& compPair : object->m_components) {
				compPair.second->postUpdate(dt);
			}
		}
	}

	void Scene::preFixedUpdate(const float dt) {
		for (auto& pair : m_gameObjects) {
			GameObject* object = pair.second;
			//Transform* transform = object->getComponent<Transform>();
			for (auto& compPair : object->m_components) {
				compPair.second->preFixedUpdate(dt);
			}
		}
	}

	void Scene::fixedUpdate(const float dt) {
		for (auto& pair : m_gameObjects) {
			GameObject* object = pair.second;
			//Transform* transform = object->getComponent<Transform>();
			for (auto& compPair : object->m_components) {
				compPair.second->fixedUpdate(dt);
			}
		}
	}

	void Scene::postFixedUpdate(const float dt) {
		
	}

	void Scene::preRender() {
		
	}

	void Scene::render() {
		for (auto& pair : m_gameObjects) {
			GameObject* object = pair.second;
			
		}
	}

	void Scene::postRender() {

	}


	GameObject* Scene::addObject(GameObject* obj) {
		m_gameObjects[obj->getID()] = obj;
		return obj;
	}
	GameObject* Scene::addObject(const std::string& name, GameObject* parent) {
		return addObject(new GameObject(name));
	}

	GameObject* Scene::createObject(const std::string& name, GameObject* parent, const glm::vec3& position)
	{
		return nullptr;
	}

	void Scene::removeObject(const std::string& name) {
	}
	void Scene::removeObject(const size_t& id) {
	}
	void Scene::removeObject(GameObject* object) {
	}

	

	GameObject* Scene::find(const std::string& name) {
		auto it = ::std::find_if(m_gameObjects.begin(), m_gameObjects.end(), 
			[&name](auto&& pair) { return pair.second == name; });
		if (it != m_gameObjects.end()) 
			return it->second;
		return nullptr;
	}

	GameObject* Scene::find(const size_t& id)
	{
		if (exists(id))
			return m_gameObjects[id];
		return nullptr;
	}

	const std::string& Scene::getName() const {
		return m_name;
	}
	const bool Scene::isActive() const
	{
		return m_active;
	}
	
	void Scene::setActive(const bool status) {
		if (status)
			resume();
		else
			pause();
	}
	void Scene::pause() {
		m_active = false;
	}
	void Scene::resume() {
		m_active = true;
	}
	
	const bool Scene::exists(const size_t& id) const
	{
		return m_gameObjects.find(id) != m_gameObjects.end();
	}

}