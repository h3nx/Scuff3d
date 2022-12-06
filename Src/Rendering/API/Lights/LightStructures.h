#pragma once
#include <glm/glm.hpp>


namespace scuff3d {

	struct BaseLight {
		glm::vec4 color;
	};


	struct PointLight : BaseLight {
		float radius;
		float falloff;
	};

	struct DirectionalLight : BaseLight {
		glm::vec4 direction;
	};











}















