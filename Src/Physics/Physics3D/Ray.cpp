#include "pch.h"
#include "Ray.h"

namespace Scuff3d {
    namespace Physics {
        Ray::Ray(const glm::vec3& position, const glm::vec3& direction) 
        : m_position(position)
        , m_direction(direction) {
        }
        Ray::~Ray() { }

        const glm::vec3& Ray::getPosition() const {
            return m_position;
        }

        const glm::vec3& Ray::getDirection() const {
            return m_direction;
        }

        void Ray::setPosition(const glm::vec3& position) {
            m_position = position;
        }

        void Ray::setDirection(const glm::vec3& direction) {
            m_direction = direction;
        }

    }
}
