#include "pch.h"
#include "Transform.h"


namespace scuff3d
{
	Transform::Transform(const glm::vec3& translation, Transform* parent) : 
		Component<Transform>(), 
		TransformData(translation, parent) 
	{
	}

	Transform::Transform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale, Transform* parent) : 
		Component<Transform>(), 
		TransformData(translation, rotation, scale, parent) 
	{
	}

	Transform::~Transform()
	{ }




}



