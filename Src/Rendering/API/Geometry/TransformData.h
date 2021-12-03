#pragma once
#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>

// Source:	https://github.com/BTH-StoraSpel-DXR/SPLASH
//			https://github.com/BTH-StoraSpel-DXR/SPLASH/blob/master/Sail/src/Sail/graphics/geometry/Transform.cpp
// Credits: See Contributors

namespace scuff3d
{

	class TransformData {
	protected:
		struct FrameData {
			glm::vec3 m_translation;
			glm::vec3 m_rotation;
			glm::quat m_rotationQuat;
			glm::vec3 m_scale;
			glm::vec3 m_forward;
			glm::vec3 m_right;
			glm::vec3 m_up;

		};
	public:
		explicit TransformData(TransformData* parent);
		TransformData(const glm::vec3& translation, TransformData* parent);
		TransformData(const glm::vec3& translation = { 0.0f, 0.0f, 0.0f },
			const glm::vec3& rotation = { 0.0f, 0.0f, 0.0f },
			const glm::vec3& scale = { 1.0f, 1.0f, 1.0f },
			TransformData* parent = nullptr);
		virtual ~TransformData();


		// 
		//		SETTERS
		//

		void translate(const glm::vec3& translation);
		void translate(const float x, const float y, const float z);

		void scale(const float factor);
		void scale(const glm::vec3& scale);

		//In radians
		void rotate(const glm::vec3& rotation);
		//In radians
		void rotate(const float x, const float y, const float z);
		void rotateAroundX(const float radians);
		void rotateAroundY(const float radians);
		void rotateAroundZ(const float radians);


		void setTranslation(const glm::vec3& translation);
		void setTranslation(const float x, const float y, const float z);

		void setRotations(const glm::vec3& rotations);
		void setRotations(const float x, const float y, const float z);
		void setRotations(const glm::quat& rotations);
		void setScale(const float scale);
		void setScale(const float x, const float y, const float z);
		void setScale(const glm::vec3& scale);

		// Forward should always be a normalized vector 
		void setForward(const glm::vec3& forward);

		// 
		//		GETTERS
		//

		const glm::vec3& getTranslation() const;
		const glm::vec3& getRotations() const;
		const glm::quat& getRotationQuat() const;
		const glm::vec3& getScale() const;

		const glm::vec3& getForward();
		const glm::vec3& getUp();
		const glm::vec3& getRight();

		//TODO: optimize with dirty
		const glm::mat4& getMatrix();
		const glm::mat4& getLocalMatrix();


		// Matrix used by collision etc.
		void prepareMatrix();

		const glm::mat4& getMatrixWithUpdate();

		const glm::mat4& getMatrixWithoutUpdate() const;

		// Matrix used to render
		glm::mat4 getRenderMatrix(float alpha = 1.0f);
		const glm::mat4& getRenderMatrixLastFrame() const;

		//Hierarchy Functions

		void setParent(TransformData* parent);
		void removeParent();

		void addChild(TransformData* child);
		void removeChild(TransformData* child);
		void removeChildren();

		virtual const TransformData* getParent() const;
		const std::vector<TransformData*>& getChildren() const;
		const TransformData* getChild(int index) const;

		const bool isChild(TransformData* child) const;



	protected:
		FrameData m_current;
		FrameData m_previous;

		// Used for collision detection
		// At most updated once per tick
		glm::mat4 m_transformMatrix;
		glm::mat4 m_localTransformMatrix;

		// Used for rendering
		// At most updated once per frame
		glm::mat4 m_renderMatrix;
		glm::mat4 m_renderMatrixLastFrame;
		glm::mat4 m_localRenderMatrix;

		bool m_parentUpdated;
		bool m_parentRenderUpdated;
		int m_hasChanged;     // If the data has been changed since the last update
		bool m_matNeedsUpdate; // Will only be false if m_hasChanged == false and a matrix has been created

		TransformData* m_parent;
		std::vector<TransformData*> m_children;
		//TODO: add this
		std::vector<int> m_childrenOrder;


	protected:
		void updateMatrix();

		void updateRenderMatrix(float alpha);

		void updateForward();
		void updateRight();
		void updateUp();

		void treeNeedsUpdating();


		void clampRotation();
		void clampRotation(float& axis);

		// Modifies the elements of matrix directly instead of multiplying with matrices
		void createTransformMatrix(glm::mat4& destination, const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale) const;

	private:
		friend class UpdateBoundingBoxSystem;
		const int getChange(); //Only access this from UpdateBoundingBoxSystem::update()
	};
}