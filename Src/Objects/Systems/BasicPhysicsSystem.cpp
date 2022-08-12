#include "pch.h"
#include "BasicPhysicsSystem.h"
#include "Objects/Components/Physics/Physics3D/BasicPhysics.h"
#include "Objects/Components/Transform.h"


namespace scuff3d {
	BasicPhysicsSystem::BasicPhysicsSystem() {
	}

	BasicPhysicsSystem::~BasicPhysicsSystem() {
	}

	void BasicPhysicsSystem::update(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt) {
	}

	void BasicPhysicsSystem::fixedUpdate(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt) {

		std::map<size_t, GameObject*>& toRender = objects[BasicPhysics::ID];
		
		for (auto& obj : toRender) {
			Transform* t = obj.second->getComponent<Transform>();
			BasicPhysics* p = obj.second->getComponent<BasicPhysics>();
			p->m_velocity += p->m_acceleration * dt;
			p->m_rotationalVelocity += p->m_rotationalAcceleration * dt;
			t->translate(p->m_velocity * dt);
			t->rotate(p->m_rotationalVelocity * dt);

		}
	}

	void BasicPhysicsSystem::render(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt) {
	}

	void BasicPhysicsSystem::debugRender(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt) {
	}

}