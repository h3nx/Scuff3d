#include "pch.h"
#include "Scene.h"



namespace scuff3d {

	Scene::Scene(const std::string& name) :
		m_name(name)
	{ }

	Scene::~Scene() {
	}

	bool Scene::init()
	{
		return true;
	}

	void Scene::preUpdate(const float dt) {
	}

	void Scene::update(const float dt) {
	}

	void Scene::postUpdate(const float dt) {
	}

	void Scene::preFixedUpdate(const float dt) {
	}

	void Scene::fixedUpdate(const float dt) {
	}

	void Scene::postFixedUpdate(const float dt) {
	}

	void Scene::preRender() {
	}

	void Scene::render() {
	}

	void Scene::postRender() {
	}

	void Scene::addObject(const std::string& name, int object) {
		m_gameObjects.emplace(name, object);
	}

	void Scene::removeObject(const std::string& name, int object) {
		if (m_gameObjects.find(name) != m_gameObjects.end()) {
			m_gameObjects.erase(name);
		}
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
		m_active = false;
	}

}