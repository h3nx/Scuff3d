#include "pch.h"
#include "ECS.h"
#include "Object.h"

namespace scuff3d {
	GameObject* ECS::createObject(const std::string& name, GameObject* parent, const glm::vec3& position) {
		GameObject* obj = NEW GameObject(name, parent, position);
		
		
		
		
		
		return nullptr;
	}

	GameObject* ECS::find(const size_t& id) {
		return nullptr;
	}
	void ECS::registerComponent(const int componentID, const size_t& objectID) {



	}

	void ECS::registerComponent(const int componentID, GameObject* object) {

	}


}


