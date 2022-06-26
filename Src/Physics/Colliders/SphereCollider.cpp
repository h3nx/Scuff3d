#include "pch.h"
#include "SphereCollider.h"


namespace Scuff3d {
    namespace Physics {

        SphereCollider::SphereCollider(const glm::vec3 center, const float radius)
            : Collider(center)
            , m_radius(radius) 
        { }

        SphereCollider::~SphereCollider() {
        }

        const float SphereCollider::getRadius() const {
            return m_radius;
        }

        void SphereCollider::setRadius(const float radius) {
            m_radius = radius;
        }

        const ColliderHitInfo SphereCollider::rayCast(const glm::vec3& position, const glm::vec3& direction) const {

            ////solve for tc
            //float L = s->center - r->origin;
            //float tc = dot(L, r->direction);

            //if (tc& lt; 0.0) return false;
            //float d2 = (tc * tc) - (L * L);

            //float radius2 = s->radius * s->radius;
            //if (d2 > radius2) return false;

            ////solve for t1c
            //float t1c = sqrt(radius2 - d2);

            ////solve for intersection points
            //*t1 = tc - t1c;
            //*t2 = tc + t1c;

            //return true;

            /*
            
            
            19 
            20 
            21
            22

                23
                24
                1
                2

            3
            4
            5
            6

                7
                8
                9
                10

            11
            12
            13
            14

                15
                16
                17
                18

            
            
            
            
            
            */



            return {
                this,
                {0,0,0},
                {0,0,0},
                {0,0,0},
                {0,0,0}
            };
        }

        const bool SphereCollider::rayCastBasic(const glm::vec3& position, const glm::vec3& direction) const {
            return false;
        }

    }
}