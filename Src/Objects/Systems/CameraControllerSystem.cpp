#include "pch.h"
#include "CameraControllerSystem.h"
#include "../Components/CameraController.h"

namespace scuff3d {
	CameraControllerSystem::CameraControllerSystem() {
	}

	CameraControllerSystem::~CameraControllerSystem() {
	}

	void CameraControllerSystem::update(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt) {
		std::map<size_t, GameObject*>& components = objects[CameraController::ID];
		for (auto& pair : components) {
			pair.second->getComponent<CameraController>()->update(dt);
		}




	}

	void CameraControllerSystem::fixedUpdate(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt) {
	}

	void CameraControllerSystem::render(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt) {
	}

	void CameraControllerSystem::debugRender(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt) {
	}

}
