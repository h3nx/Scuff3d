#include "pch.h"
#include "Scene.h"
#include <algorithm>
#include "Objects/Systems/RenderSystem.h"


namespace scuff3d {

	Scene::Scene(const std::string& name) :
		m_name(name)
	{ 
	
	}

	Scene::~Scene() {
		
	}

	bool Scene::init() {

		addSystem(NEW RenderSystem(m_app->getRenderer()), 1000);


		return true;
	}

	void Scene::preUpdate(const float dt) {
		
	}

	void Scene::update(const float dt) {
		ECS::update(dt);
	}

	void Scene::postUpdate(const float dt) {
		
	}

	void Scene::preFixedUpdate(const float dt) {
		
	}

	void Scene::fixedUpdate(const float dt) {
		ECS::fixedUpdate(dt);
	}

	void Scene::postFixedUpdate(const float dt) {
		
	}

	void Scene::preRender() {
		
	}

	void Scene::render() {
		
		ECS::render(0);
	}

	void Scene::postRender() {

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