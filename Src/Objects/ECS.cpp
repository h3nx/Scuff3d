#include "pch.h"
#include "ECS.h"
#include "Object.h"
#include "Components/Transform.h"
#include "System.h"
namespace scuff3d {
	ECS::ECS() {

	}
	ECS::~ECS() {

		safeDeleteMap(m_systems);
		for (auto& pair : m_gameObjects) {
			if (pair.second) {
				delete pair.second;
			}
		}
		m_gameObjects.clear();
		
		
		safeDeleteMap(m_systems);
		
		m_components.clear();
	}
	GameObject* ECS::createObject(const std::string& name, GameObject* parent, const glm::vec3& position) {
		GameObject* obj = NEW GameObject(this, name, parent, position);
		this->m_gameObjects[obj->getID()] = obj;
		
		
		
		
		return obj;
	}

	GameObject* ECS::find(const size_t& id) {
		return nullptr;
	}
	void ECS::addSystem(System* system, const int order) {
		m_systems[system->getID()] = system;
		m_systemOrder[order].push_back(system->getID());
	}
	void ECS::initSystems() {
	}



	void ECS::frameBegin() {
	}
	void ECS::update(const float& dt) {
		for (auto& s : m_systemOrder) {
			std::vector<size_t>& order = s.second;
			for (size_t& n : order) {
				m_systems[n]->update(m_components, dt);
			}
		}
	}
	void ECS::fixedUpdate(const float& dt) {
		for (auto& s : m_systemOrder) {
			std::vector<size_t>& order = s.second;
			for (size_t& n : order) {
				m_systems[n]->fixedUpdate(m_components, dt);
			}
		}
	}
	void ECS::render(const float& dt) {
		for (auto& s : m_systemOrder) {
			std::vector<size_t>& order = s.second;
			for (size_t& n : order) {
				m_systems[n]->render(m_components, dt);
			}
		}
	}
	void ECS::frameEnd() {

	}












	void ECS::registerComponent(const int componentID, const size_t& objectID) {
	}

	void ECS::registerComponent(const int componentID, GameObject* object) {
		m_components[componentID][object->getID()] = object;
	}

	void ECS::registerComponentRemoval(const int componentID, GameObject* object) {
		m_components[componentID][object->getID()] = nullptr;

	}

	void ECS::registerObjectRemoval(const size_t& id) {
		m_gameObjects[id] = nullptr;
	}

	void ECS::requestDestroy(GameObject* obj) {

	}

	void ECS::requestImmediateDestroy(GameObject* obj) {
		delete obj;
	}



}


