#pragma once
#include "glm/glm.hpp"

namespace scuff3d {

	class CameraData {
	public:
		CameraData();
		CameraData(const float& fovH, const float& aspectRatio, const float& minZ, const float& maxZ);
		virtual ~CameraData();

		// in degrees
		bool setFovVertical(const float& fov);
		bool setFovHorizontal(const float& fov);
		bool setFovVerticalRadian(const float& fov);
		bool setFovHorizontalRadian(const float& fov);
		bool setAspectratio(const glm::vec2& pixels);
		bool setAspectratio(const float& x, const float& y);
		bool setAspectratio(const float& aspectratio);
		bool setMinZ(const float& minZ);
		bool setMaxZ(const float& maxZ);

		// In radians
		const float getFovH() const;
		const float getFovV() const;
		const float getAspectRatio() const;
		const float getMinZ() const;
		const float getMaxZ() const;

		virtual const glm::mat4& getViewMatrix();
		const glm::mat4& getProjectionMatrix();

	protected:

		const glm::mat4& calculateViewMatrix(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up);
		const glm::mat4& calculateProjectionMatrix();

		float m_fovH;
		float m_fovV;
		// x divided by y
		float m_aspectRatio;
		float m_minZ;
		float m_maxZ;

		bool m_projectionDirty;

		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;


	private:
		const float horizontalToVertical(const float& fov);
		const float verticalToHorizontal(const float& fov);


	};



}