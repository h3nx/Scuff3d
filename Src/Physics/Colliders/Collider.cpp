#include "pch.h"
#include "Collider.h"

namespace Scuff3d {
    namespace Physics {
        Collider::Collider(const glm::vec3 center)
        : m_center(center) { }

        Collider::~Collider() { }

        const glm::vec3& Collider::getCenter() const {
            return m_center;
        }

        void Collider::setCenter(const glm::vec3& center) {
            m_center = center;
        }

    }
}

