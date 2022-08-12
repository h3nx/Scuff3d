#include "pch.h"
#include "BasicPhysics.h"
#include "Objects/Components.h"
#include "Scuff3dImGui/Widgets/RenderingAPIWidgets.h"
#include "Scuff3dImGui/Widgets/GlmWidgets.h"
#include "Objects/Systems/BasicPhysicsSystem.h"


namespace scuff3d {
	BasicPhysics::BasicPhysics() : Component<BasicPhysics>() {
		m_velocity = {0.0f,0.0f,0.0f};
		m_acceleration = {0.0f,0.0f,0.0f};

		m_rotationalVelocity = {0.0f,0.0f,0.0f};
		m_rotationalAcceleration = { 0.0f,0.0f,0.0f };
	}
	BasicPhysics::~BasicPhysics() {
	}
	void BasicPhysics::update(const float& dt) {
		
	}

	void BasicPhysics::fixedUpdate(const float& dt) {
		GameObject* obj = getGameObject();
		Transform* t = obj->getComponent<Transform>();
		m_velocity += m_acceleration * dt;
		m_rotationalVelocity += m_rotationalAcceleration * dt;
		t->translate(m_velocity * dt);
		t->rotate(m_rotationalVelocity * dt);
	}

	void BasicPhysics::renderImGui() {
		const int line = 85;
		scuff3dImGui::Vec3Var("Velocity", m_velocity, line);
		scuff3dImGui::Vec3Var("Acceleration", m_acceleration, line);
		ImGui::Separator();
		scuff3dImGui::Vec3Var("Angular vel", m_rotationalVelocity, line);
		scuff3dImGui::Vec3Var("Angular Acc", m_rotationalAcceleration, line);

	}

	void BasicPhysics::setAngularVelocity(const glm::vec3& vel) {
		m_rotationalVelocity = vel;
	}

	void BasicPhysics::setAngularAcceleration(const glm::vec3& acc) {
		m_rotationalAcceleration = acc;
	}

	void BasicPhysics::setVelocity(const glm::vec3& vel) {
		m_velocity = vel;
	}

	void BasicPhysics::setAcceleration(const glm::vec3& acc) {
		m_acceleration = acc;
	}

	const glm::vec3& BasicPhysics::getVelocity() const {
		return m_velocity;
	}

	const glm::vec3& BasicPhysics::getAcceleration() const {
		return m_acceleration;
	}

	const glm::vec3& BasicPhysics::getAngularVelocity() const {
		return m_rotationalVelocity;
	}

	const glm::vec3& BasicPhysics::getAngularAcceleration() const {
		return m_rotationalAcceleration;
	}

}