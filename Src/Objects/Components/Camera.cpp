#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Scuff3dImGui/Widgets/GlmWidgets.h"


namespace scuff3d {

	Camera::Camera() : Component<Camera>(), CameraData() 
	{ }

	Camera::Camera(const float& fovH, const float& aspectRatio, const float& minZ, const float& maxZ) :
		Component<Camera>(),
		CameraData(fovH,aspectRatio,minZ,maxZ)
	{ }

	Camera::~Camera() {

	}

	void Camera::init() {
		m_transform = getGameObject()->getComponent<Transform>();
	}

	const glm::mat4& Camera::getViewMatrix() {
		return calculateViewMatrix(m_transform->getTranslation(), m_transform->getForward(), m_transform->getUp());
	}

	void Camera::renderImGui() {
		constexpr float radToDeg = 180.0f / glm::pi<float>();
		float fovh = getFovH() * radToDeg;
		float fovv = getFovV() * radToDeg;
		float aspect = getAspectRatio();
		float min = getMinZ();
		float max = getMaxZ();
		
		if (ImGui::SliderFloat("fovH", &fovh, 0.0f, 180.0f)) {
			setFovHorizontal(fovh);
		}
		if (ImGui::SliderFloat("fovV", &fovv, 0.0f, 180.0f, "%.3f")) {
			setFovVertical(fovv);
		}
		if (ImGui::DragFloat("aspect", &aspect, 0.02f)) {
			setAspectratio(aspect);
		}
		if (ImGui::DragFloat("min", &min, 0.02f)) {
			setMinZ(min);
		}
		if (ImGui::DragFloat("max", &max, 0.02f)) {
			setMaxZ(max);
		}

		ImGui::Text("View Matrix");
		scuff3dImGui::Mat4(getViewMatrix());
		ImGui::Text("Projection Matrix");
		scuff3dImGui::Mat4(getProjectionMatrix());


	}

}







/*
	static CameraData data;
		

		


*/