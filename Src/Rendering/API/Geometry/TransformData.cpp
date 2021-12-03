#include "pch.h"
#include "TransformData.h"
#include <glm/gtx/quaternion.hpp>
namespace scuff3d
{

	const glm::vec3 forward_origin(0.0f, 0.0f, -1.0f);
	const glm::vec3 right_origin(1.0f, 0.0f, 0.0f);
	const glm::vec3 up_origin(0.0f, 1.0f, 0.0f);

	/*TransformData::TransformData()
		: TransformData::TransformData({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f })
	{}*/


	TransformData::TransformData(TransformData* parent) : TransformData()
	{
		m_parent = parent;
	}

	TransformData::TransformData(const glm::vec3& translation, TransformData* parent) : TransformData(translation)
	{
		m_parent = parent;
	}

	TransformData::TransformData(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale, TransformData* parent)
	{
		m_current.m_translation = translation;
		m_current.m_rotation = rotation;
		m_current.m_rotationQuat = rotation;
		m_current.m_scale = scale;
		m_current.m_forward = forward_origin;
		m_current.m_right = right_origin;
		m_current.m_up = up_origin;

		m_previous.m_translation = translation;
		m_previous.m_rotation = rotation;
		m_previous.m_rotationQuat = rotation;
		m_previous.m_scale = scale;
		m_previous.m_forward = forward_origin;
		m_previous.m_right = right_origin;
		m_previous.m_up = up_origin;

		m_matNeedsUpdate = true;
		m_hasChanged = 2;

		m_parent = parent;


	}

	TransformData::~TransformData() {
	}
	

	void TransformData::translate(const glm::vec3& move) {
		m_current.m_translation += move;
		m_matNeedsUpdate = true;
		m_hasChanged |= 1;
		treeNeedsUpdating();
	}
	void TransformData::translate(const float x, const float y, const float z) {
		m_current.m_translation += glm::vec3(x, y, z);
		m_matNeedsUpdate = true;
		m_hasChanged |= 1;
		treeNeedsUpdating();
	}

	void TransformData::scale(const float factor) {
		m_current.m_scale *= factor;
		m_matNeedsUpdate = true;
		m_hasChanged |= 2;
		treeNeedsUpdating();
	}
	void TransformData::scale(const glm::vec3& scale) {
		m_current.m_scale *= scale;
		m_matNeedsUpdate = true;
		m_hasChanged |= 2;
		treeNeedsUpdating();
	}

	void TransformData::rotate(const glm::vec3& rotation) {
		m_current.m_rotation += rotation;
		clampRotation();
		m_current.m_rotationQuat = glm::quat(m_current.m_rotation);
		m_matNeedsUpdate = true;
		m_hasChanged |= 2;
		treeNeedsUpdating();
	}
	void TransformData::rotate(const float x, const float y, const float z) {
		m_current.m_rotation += glm::vec3(x, y, z);
		clampRotation();
		m_current.m_rotationQuat = glm::quat(m_current.m_rotation);
		m_matNeedsUpdate = true; // TODO: Check this
		treeNeedsUpdating();
	}

	void TransformData::rotateAroundX(const float radians) {
		m_current.m_rotation.x += radians;
		clampRotation(m_current.m_rotation.x);
		m_current.m_rotationQuat = glm::quat(m_current.m_rotation);
		m_matNeedsUpdate = true;
		m_hasChanged |= 2;
		treeNeedsUpdating();
	}
	void TransformData::rotateAroundY(const float radians) {
		m_current.m_rotation.y += radians;
		clampRotation(m_current.m_rotation.y);
		m_current.m_rotationQuat = glm::quat(m_current.m_rotation);
		m_matNeedsUpdate = true;
		m_hasChanged |= 2;
		treeNeedsUpdating();
	}
	void TransformData::rotateAroundZ(const float radians) {
		m_current.m_rotation.z += radians;
		clampRotation(m_current.m_rotation.z);
		m_current.m_rotationQuat = glm::quat(m_current.m_rotation);
		m_matNeedsUpdate = true;
		m_hasChanged |= 2;
		treeNeedsUpdating();
	}

	void TransformData::setTranslation(const glm::vec3& translation) {
		m_current.m_translation = translation;
		m_matNeedsUpdate = true;
		m_hasChanged |= 1;
		treeNeedsUpdating();
	}
	void TransformData::setTranslation(const float x, const float y, const float z) {
		m_current.m_translation = glm::vec3(x, y, z);
		m_matNeedsUpdate = true;
		m_hasChanged |= 1;
		treeNeedsUpdating();
	}

	void TransformData::setRotations(const glm::vec3& rotations) {
		m_current.m_rotation = rotations;
		clampRotation();
		m_current.m_rotationQuat = glm::quat(m_current.m_rotation);
		m_matNeedsUpdate = true;
		m_hasChanged |= 2;
		treeNeedsUpdating();
	}
	void TransformData::setRotations(const float x, const float y, const float z) {
		m_current.m_rotation = glm::vec3(x, y, z);
		clampRotation();
		m_current.m_rotationQuat = glm::quat(m_current.m_rotation);
		m_matNeedsUpdate = true;
		m_hasChanged |= 2;
		treeNeedsUpdating();
	}
	void TransformData::setRotations(const glm::quat& rotations) {
		m_current.m_rotationQuat = rotations;
		m_current.m_rotation = glm::eulerAngles(rotations);
		clampRotation();
		m_matNeedsUpdate = true;
		m_hasChanged |= 2;
		treeNeedsUpdating();
	}

	void TransformData::setScale(const float scale) {
		m_current.m_scale = glm::vec3(scale, scale, scale);
		m_matNeedsUpdate = true;
		m_hasChanged |= 2;
		treeNeedsUpdating();
	}
	void TransformData::setScale(const float x, const float y, const float z) {
		m_current.m_scale = glm::vec3(x, y, z);
		m_matNeedsUpdate = true;
		m_hasChanged |= 2;
		treeNeedsUpdating();
	}
	void TransformData::setScale(const glm::vec3& scale) {
		m_current.m_scale = scale;
		m_matNeedsUpdate = true;
		m_hasChanged |= 2;
		treeNeedsUpdating();
	}

	void TransformData::setForward(const glm::vec3& forward) {
		m_current.m_forward = glm::vec3(forward.x, forward.y, forward.z);
		m_current.m_rotationQuat = glm::rotation(glm::vec3(0.f, 0.f, -1.f), m_current.m_forward);
		m_matNeedsUpdate = true;
		m_hasChanged |= 2;
	}

	
	// Returns the local translation
	const glm::vec3& TransformData::getTranslation() const {
		return m_current.m_translation;
	}

	const glm::vec3& TransformData::getRotations() const {
		return m_current.m_rotation;
	}
	const glm::quat& TransformData::getRotationQuat() const
	{
		return m_current.m_rotationQuat;
	}
	const glm::vec3& TransformData::getScale() const {
		return m_current.m_scale;
	}

	const glm::vec3& TransformData::getForward() {
		updateForward();
		return m_current.m_forward;
	}
	const glm::vec3& TransformData::getUp() {
		updateUp();
		return m_current.m_up;
	}
	const glm::vec3& TransformData::getRight() {
		updateRight();
		return m_current.m_right;
	}

	const glm::mat4& TransformData::getMatrix() {
		if (m_parent)
			m_transformMatrix = m_parent->getMatrix() * getLocalMatrix();
		else
			m_transformMatrix = m_localTransformMatrix = getLocalMatrix();
		return m_transformMatrix;
	}

	const glm::mat4& TransformData::getLocalMatrix() {
		if (m_matNeedsUpdate) {
			createTransformMatrix(m_localTransformMatrix, m_current.m_translation, m_current.m_rotation, m_current.m_scale);
			m_matNeedsUpdate = false;
		}
		return m_localTransformMatrix;
	}


	void TransformData::prepareMatrix() {
		/*if (m_matNeedsUpdate) {
			updateLocalMatrix();
			m_matNeedsUpdate = false;
		}
		if (m_parentUpdated || !m_parent) {
			updateMatrix();
			m_parentUpdated = false;
		}*/
	}

	const glm::mat4& TransformData::getMatrixWithUpdate() {
		prepareMatrix();
		return m_transformMatrix;
	}


	const glm::mat4& TransformData::getMatrixWithoutUpdate() const {
		return m_transformMatrix;
	}

	glm::mat4 TransformData::getRenderMatrix(float alpha) {
		// If data hasn't changed use the CPU side transformMatrix
		/*if (!m_hasChanged && !m_parentRenderUpdated) {
			m_renderMatrix = getMatrixWithUpdate();
		}
		else {*/
			// if the data has changed between updates then the matrix will be interpolated every frame
			//updateLocalRenderMatrix(alpha);

			//updateRenderMatrix(alpha); // if it has then interpolate
			//if (m_parentRenderUpdated || !m_parent) {
			//	updateRenderMatrix(alpha);
			//	m_parentRenderUpdated = false;
			//}
		//}

		return m_renderMatrix;
	}

	const glm::mat4& TransformData::getRenderMatrixLastFrame() const {
		return m_renderMatrixLastFrame;
	}

	//void TransformData::updateLocalRenderMatrix(float alpha) {
		//// Linear interpolation between the two most recent snapshots
		//glm::vec3 trans = (alpha * m_current.m_translation) + ((1.0f - alpha) * m_data.m_previous.m_translation);
		//glm::quat rot = (alpha * m_current.m_rotationQuat) + ((1.0f - alpha) * m_data.m_previous.m_rotationQuat);
		//glm::vec3 scale = (alpha * m_current.m_scale) + (1.0f - alpha) * m_data.m_previous.m_scale;

		//createTransformMatrix(m_localRenderMatrix, trans, rot, scale);
	//}

	void TransformData::updateRenderMatrix(float alpha) {
		/*if (m_parent) {
			m_renderMatrix = m_parent->getRenderMatrix(alpha) * m_localRenderMatrix;
		}
		else {
			m_renderMatrix = m_localRenderMatrix;
		}*/
	}

	void TransformData::updateForward() {
		m_current.m_forward = m_current.m_rotationQuat * glm::vec3(0.f, 0.f, -1.f);
	}


	void TransformData::updateUp() {
		m_current.m_up = m_current.m_rotationQuat * glm::vec3(0.f, 1.f, 0.f);
	}
	void TransformData::updateRight() {
		m_current.m_right = m_current.m_rotationQuat * glm::vec3(1.f, 0.f, 0.f);
	}

	//void TransformData::updateLocalMatrix() {
	//	createTransformMatrix(m_localTransformMatrix, m_current.m_translation, m_current.m_rotationQuat, m_current.m_scale);
	//}

	void TransformData::updateMatrix() {
		/*if (m_parent) {
			m_transformMatrix = m_parent->getMatrixWithUpdate() * m_localTransformMatrix;
		}
		else {
			m_transformMatrix = m_localTransformMatrix;
		}*/
	}

	void TransformData::treeNeedsUpdating() {
		/*m_parentUpdated = true;
		m_parentRenderUpdated = true;
		if (m_parent) {
			m_hasChanged = m_parent->getChange() | m_hasChanged;
		}
		for (TransformData* child : m_children) {
			child->treeNeedsUpdating();
		}*/
	}

	
	void TransformData::setParent(TransformData* parent) {
		removeParent();
		parent->addChild(this);
	}
	void TransformData::removeParent() {
		m_parent->removeChild(this);
	}

	void TransformData::addChild(TransformData* child) {
		if (isChild(child))
			return;
		m_children.push_back(child);
		child->m_parent = this;
	}
	void TransformData::removeChild(TransformData* child) {
		for (int i = 0; i < m_children.size(); i++) {
			if (m_children[i] == child) {
				child->m_parent = nullptr;
				m_children[i] = m_children.back();
				m_children.pop_back();
				break;
			}
		}
	}
	void TransformData::removeChildren() {
		for (auto child : m_children) {
			child->removeParent();
		}
		m_children.clear();
	}
	
	const TransformData* TransformData::getParent() const {
		return m_parent;
	}
	const std::vector<TransformData*>& TransformData::getChildren() const {
		return m_children;
	}
	const TransformData* TransformData::getChild(int index) const {
		return m_children[index];
	}

	const bool TransformData::isChild(TransformData* child) const {
		for (TransformData* t : m_children) {
			if (t == child) {
				true;
			}
		}
		return false;
	}


		
	void TransformData::clampRotation() {
		clampRotation(m_current.m_rotation.x);
		clampRotation(m_current.m_rotation.y);
		clampRotation(m_current.m_rotation.z);
	}
	void TransformData::clampRotation(float& axis) {
		const float twoPi = glm::two_pi<float>();
		while (axis < 0) {
			axis += twoPi;
		}
		while (axis >= twoPi) {
			axis -= twoPi;
		}
	}

	void TransformData::createTransformMatrix(glm::mat4& destination, const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale) const {
		glm::mat4 prev = glm::mat4(destination);

		destination = glm::identity<glm::mat4>();
		destination = glm::translate(destination, translation);
		destination *= glm::toMat4(rotation);
		destination = glm::scale(destination, scale);
	}

	const int TransformData::getChange() {
		return m_hasChanged;
	}
}