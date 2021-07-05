#pragma once
#include "Rendering/API/Geometry/Transform.h"


namespace scuff3d {

	class Camera : public Transform{
	public:
		Camera();
		virtual ~Camera();


		void lookAt(const glm::vec3& position);



	private:


	};



}