#pragma once
#include "Collider.h"



namespace Scuff3d {
	namespace Physics {
		class SphereCollider : public Collider {
		public:
			SphereCollider(const glm::vec3 center = { 0,0,0 }, const float radius = 1.0f);
			~SphereCollider();

			const float getRadius() const;
			void setRadius(const float radius);
			

			virtual const ColliderHitInfo rayCast(const glm::vec3& position, const glm::vec3& direction) const override;
			virtual const bool rayCastBasic(const glm::vec3& position, const glm::vec3& direction) const override;



		private:
			float m_radius;
		};
	}
}




