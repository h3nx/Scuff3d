#pragma once
#include "Objects/Component.h"

namespace scuff3d
{
	class BasicPhysicsSystem;

	class BasicPhysics :
		public Component<BasicPhysics>
	{
	public:
		friend BasicPhysicsSystem;

		BasicPhysics();
		~BasicPhysics();

		
		void update(const float& dt) override;
		void fixedUpdate(const float& dt) override;
		void renderImGui();


		void setAngularVelocity(const glm::vec3& vel);
		void setAngularAcceleration(const glm::vec3& acc);

		void setVelocity(const glm::vec3& vel);
		void setAcceleration(const glm::vec3& acc);

		const glm::vec3& getVelocity() const;
		const glm::vec3& getAcceleration() const;
		const glm::vec3& getAngularVelocity() const;
		const glm::vec3& getAngularAcceleration() const;



	private:
		float m_mass;

		glm::vec3 m_velocity;
		glm::vec3 m_acceleration;

		glm::vec3 m_rotationalVelocity;
		glm::vec3 m_rotationalAcceleration;




	};

}

