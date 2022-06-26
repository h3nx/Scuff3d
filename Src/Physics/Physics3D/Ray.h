#pragma once
#include <glm/glm.hpp>


namespace Scuff3d {
    namespace Physics {
        class Ray {
            Ray(const glm::vec3& position = {0,0,0}, const glm::vec3& direction = {0,0,0});
            virtual ~Ray();

            const glm::vec3& getPosition() const;
            const glm::vec3& getDirection() const;

            void setPosition(const glm::vec3& position);
            void setDirection(const glm::vec3& direction);

        private:

            glm::vec3 m_position;
            glm::vec3 m_direction;
        };
    }
}
