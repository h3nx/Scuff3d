#pragma once
#include <glm/glm.hpp>



namespace Scuff3d {
	namespace Physics {
		class Collider;
		struct ColliderHitInfo {
			const Collider* collider;
			const glm::vec3 position;
			const glm::vec3 normal;
			const glm::vec3 tangent;
			const glm::vec3 bitangent;
		};

		enum {
			Sphere
		};
		class Collider {
		public:
			Collider(const glm::vec3 center = {0,0,0});
			virtual ~Collider();

			const glm::vec3& getCenter() const;

			void setCenter(const glm::vec3& center);

			virtual const ColliderHitInfo rayCast(const glm::vec3& position, const glm::vec3& direction) const = 0;
			virtual const bool rayCastBasic(const glm::vec3& position, const glm::vec3& direction) const = 0;


		protected:

			glm::vec3 m_center;


		};




	}

}




