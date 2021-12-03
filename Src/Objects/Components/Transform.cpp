#include "pch.h"
#include "Transform.h"
#include "Scuff3dImGui/Widgets/RenderingAPIWidgets.h"
#include "Scuff3dImGui/Widgets/GlmWidgets.h"

namespace scuff3d
{
	Transform::Transform(Transform* parent) : 
		Component<Transform>(), TransformData(parent) 
	{ 
	}

	Transform::Transform(const glm::vec3& translation, Transform* parent) :
		Component<Transform>(), 
		TransformData(translation,parent) 
	{
	}

	Transform::Transform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale, Transform* parent) : 
		Component<Transform>(), 
		TransformData(translation, rotation, scale, parent) 
	{
	}

	Transform::~Transform()
	{ }
	Transform* Transform::getParent()
	{
		return static_cast<Transform*>(m_parent);
	}

	void Transform::renderImGui() {
		const int line = 85;
		ImGui::Text("Parent:");
		ImGui::SameLine(line);
		ImGui::Text(m_parent ? getParent()->getGameObject()->getName():"no parent");
		scuff3dImGui::TransformDataEx(this);

		ImGui::Text("Local Matrix");
		scuff3dImGui::Mat4(getLocalMatrix());
		ImGui::Text("Global Matrix");
		scuff3dImGui::Mat4(getMatrix());

	}





}



