#pragma once
#include "Objects/Component.h"
#include "Rendering/API/Geometry/TransformData.h"
#include <vector>
#include <string>

namespace scuff3d
{
	class Transform : 
		public Component<Transform>, 
		public TransformData
	{
	public:
		explicit Transform(Transform * parent) : Component<Transform>(), TransformData() {};
		Transform(const glm::vec3& translation, Transform* parent);
		Transform(
			const glm::vec3& translation = { 0.0f, 0.0f, 0.0f },
			const glm::vec3& rotation = { 0.0f, 0.0f, 0.0f },
			const glm::vec3& scale = { 1.0f, 1.0f, 1.0f },
			Transform* parent = nullptr);

		~Transform();
		


		//const Transform* getParent() const;
		//void setParent(const Transform* parent);

		//void detachChildren();
		//void removeChild(const Transform* child);

		//const std::vector<Transform*>& getChildren() const;
		//const Transform* getChild(int index) const;
		//const Transform* getChild(size_t id) const;
		//const Transform* getChild(const std::string& name) const;

	private:

		Transform* m_parent;


	};



}

