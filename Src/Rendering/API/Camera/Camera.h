#pragma once
#include "Rendering/API/Geometry/TransformData.h"


namespace scuff3d {

	class Camera : public TransformData{
	public:
		Camera();
		virtual ~Camera();


		void lookAt(const glm::vec3& position);



	private:


	};



}