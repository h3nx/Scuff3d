#pragma once
#include "Objects/Component.h"

#include "Rendering/API/Camera/CameraData.h"

namespace scuff3d
{

	class Transform;
	class Camera : public Component<Camera>, public CameraData
	{
	public:
		Camera();
		Camera(const float& fovH, const float& aspectRatio, const float& minZ, const float& maxZ);
		~Camera();

		void init();

		const glm::mat4& getViewMatrix();
		void renderImGui();

	private:
		Transform* m_transform;

	};





}