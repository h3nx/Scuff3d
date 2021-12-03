#include "pch.h"
#include "CameraData.h"

namespace scuff3d {

	CameraData::CameraData() :
		m_fovH(90),
		m_fovV(1.0f),
		m_aspectRatio(1920.0f/1080.0f),
		m_minZ(0.1f),
		m_maxZ(100.0f),
		m_projectionDirty(false)
	{
		calculateViewMatrix({ 0,0,0 }, { 0,0,1 }, { 0,1,0 });
		calculateProjectionMatrix();
		m_fovV = horizontalToVertical(m_fovH);
	}

	CameraData::CameraData(const float& fovH, const float& aspectRatio, const float& minZ, const float& maxZ) : CameraData(){
		m_fovH = fovH;
		m_fovV = horizontalToVertical(fovH);
		m_aspectRatio = aspectRatio;
		m_minZ = minZ;
		m_maxZ = maxZ;
	}

	CameraData::~CameraData() {
	}

	bool CameraData::setFovVertical(const float& fov) {
		constexpr float radToDeg = 180.0f / glm::pi<float>();
		return setFovVerticalRadian(fov / radToDeg);
	}

	bool CameraData::setFovHorizontal(const float& fov) {
		constexpr float radToDeg = 180.0f / glm::pi<float>();
		return setFovHorizontalRadian(fov / radToDeg);
	}

	bool CameraData::setFovVerticalRadian(const float& fov) {
		if (fov <= 0)
			return false;
		m_fovV = fov;
		m_fovH = verticalToHorizontal(m_fovV);
		if (m_fovH < 0)
			m_fovH *= -1.0f;
		m_projectionDirty = true;
		return true;
	}

	bool CameraData::setFovHorizontalRadian(const float& fov) {
		if (fov <= 0)
			return false;
		m_fovH = fov;
		m_fovV = horizontalToVertical(m_fovH);
		if (m_fovV < 0)
			m_fovV *= -1.0f;
		m_projectionDirty = true;
		return true;
	}

	

	bool CameraData::setAspectratio(const float& aspectratio) {
		if (aspectratio == 0)
			return false;
		m_aspectRatio = aspectratio;
		setFovHorizontalRadian(m_fovH);
		m_projectionDirty = true;
		return true;
	}
	bool CameraData::setAspectratio(const glm::vec2& pixels) {
		return setAspectratio(pixels.x / pixels.y);
	}

	bool CameraData::setAspectratio(const float& x, const float& y) {
		return setAspectratio(x / y);
	}
	bool CameraData::setMinZ(const float& minZ) {
		if(minZ <= 0 || minZ > m_maxZ)
			return false;
		m_minZ = minZ;
		m_projectionDirty = true;
		return true;
	}

	bool CameraData::setMaxZ(const float& maxZ) {
		if (maxZ <= 0 || maxZ < m_minZ)
			return false;
		m_maxZ = maxZ;
		m_projectionDirty = true;
		return true;
	}

	const float CameraData::getFovH() const {
		return m_fovH;
	}

	const float CameraData::getFovV() const {
		return m_fovV;
	}

	const float CameraData::getAspectRatio() const {
		return m_aspectRatio;
	}

	const float CameraData::getMinZ() const {
		return m_minZ;
	}

	const float CameraData::getMaxZ() const {
		return m_maxZ;
	}

	const glm::mat4& CameraData::getViewMatrix() {
		return m_viewMatrix;
	}

	const glm::mat4& CameraData::getProjectionMatrix() {
		if (m_projectionDirty)
			calculateProjectionMatrix();
		return m_projectionMatrix;
	}

	const glm::mat4& CameraData::calculateViewMatrix(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up) {
		m_viewMatrix = glm::lookAtRH(position, position + direction, up);
		return m_viewMatrix;
	}

	const glm::mat4& CameraData::calculateProjectionMatrix() {
		m_projectionMatrix = glm::perspectiveRH(m_fovV, m_aspectRatio, m_minZ, m_maxZ);
		return m_projectionMatrix;
	}

	const float CameraData::horizontalToVertical(const float& fov) {
		return 2.0f * atan(tan(fov * 0.5f) / m_aspectRatio);
	}

	const float CameraData::verticalToHorizontal(const float& fov) {
		return 2.0f * atan(tan(fov * 0.5f) * m_aspectRatio);
	}

}