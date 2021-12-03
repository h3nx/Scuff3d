#pragma once
#include "Objects/Component.h"
#include "Rendering/API/Geometry/TransformData.h"
#include <vector>
#include <string>

#pragma message("transform")
namespace scuff3d
{
	class Transform : 
		public Component<Transform>, 
		public TransformData
	{
	public:
		explicit Transform(Transform* parent);
		Transform(const glm::vec3& translation, Transform* parent);
		Transform(
			const glm::vec3& translation = { 0.0f, 0.0f, 0.0f },
			const glm::vec3& rotation = { 0.0f, 0.0f, 0.0f },
			const glm::vec3& scale = { 1.0f, 1.0f, 1.0f },
			Transform* parent = nullptr);

		~Transform();
		Transform* getParent();


		void renderImGui();



	private:



	};



}

