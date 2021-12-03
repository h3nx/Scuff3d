#include "pch.h"
#include "BasicPhysics.h"
#include "Objects/Components.h"
namespace scuff3d {
	BasicPhysics::BasicPhysics() : Component<BasicPhysics>() {
		m_velocity = {0.0f,0.0f,0.0f};
		m_acceleration = {0.0f,0.0f,0.0f};

		m_rotationalVelocity = {0.0f,0.0f,0.0f};
		m_rotationalAcceleration = { 0.0f,0.0f,0.0f };
		m_dt = 0.0f;
	}
	void BasicPhysics::update(const float& dt) {
		
	}

	void BasicPhysics::fixedUpdate(const float& dt) {
		GameObject* obj = getGameObject();
		Transform* t = obj->getComponent<Transform>();
		m_dt = dt;
		m_velocity += m_acceleration * dt;
		m_rotationalVelocity += m_rotationalAcceleration * dt;
		t->translate(m_velocity * dt);
		t->rotate(m_rotationalVelocity * dt);
	}

	void BasicPhysics::renderImGui() {
		ImGui::Text(std::to_string(m_dt)+"s");
		ImGui::Text(std::to_string(1.0f/ m_dt)+"fps");


	}

}