#pragma once
#include "Objects/Component.h"
#include <glm/glm.hpp>
namespace scuff3d
{
	class BasicPhysics :
		public Component<BasicPhysics>
	{
	public:
		BasicPhysics();
		~BasicPhysics() {};

		float m_mass;

		glm::vec3 m_velocity;
		glm::vec3 m_acceleration;

		glm::vec3 m_rotationalVelocity;
		glm::vec3 m_rotationalAcceleration;
		
		float m_dt;

		void update(const float& dt) override;
		void fixedUpdate(const float& dt) override;
		void renderImGui();

	private:




	};

}

