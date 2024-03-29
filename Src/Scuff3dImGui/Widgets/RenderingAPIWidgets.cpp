#include "pch.h"
#include "RenderingAPIWidgets.h"
#include "Rendering/API/Geometry/Mesh.h"

namespace scuff3dImGui {
	void TransformData(scuff3d::TransformData* transform) {
		const int line = 85;
		ImGui::Text("Position");
		glm::vec3 p = transform->getTranslation();
		glm::vec3 r = transform->getRotations();
		glm::vec3 s = transform->getScale();
		float* ptr = nullptr;

		ptr = static_cast<float*>(glm::value_ptr(p));
		ImGui::SameLine(line);
		if (ImGui::DragFloat3("##Position", ptr, 0.1f)) {
			transform->setTranslation(p);
		}

		ImGui::Text("Rotation");
		ptr = static_cast<float*>(glm::value_ptr(r));
		ImGui::SameLine(line);
		if (ImGui::DragFloat3("##Rotation", ptr, 0.1f)) {
			transform->setRotations(r);
		}

		ImGui::Text("Scale");
		ptr = static_cast<float*>(glm::value_ptr(s));
		ImGui::SameLine(line);
		if (ImGui::DragFloat3("##Scale", ptr, 0.1f)) {
			transform->setScale(s);
		}



	};
	void TransformDataEx(scuff3d::TransformData* transform) {
		const int line = 85;
		
		TransformData(transform);

		//glm::vec3()
			
		ImGui::Text("Forward: ");
		ImGui::SameLine(line);
		ImGui::Text(glm::to_string(transform->getForward()));

		ImGui::Text("Up: ");
		ImGui::SameLine(line);
		ImGui::Text(glm::to_string(transform->getUp()));

		ImGui::Text("Right: ");
		ImGui::SameLine(line);
		ImGui::Text(glm::to_string(transform->getRight()));

		ImGui::Text("QuatRot: ");
		ImGui::SameLine(line);
		ImGui::Text(glm::to_string(transform->getRotationQuat()));

	}

	void TransformDataExBorder(scuff3d::TransformData* transform)
	{
		std::stringstream stream;
		stream << "Transform 0x"
			<< std::setfill('0') << std::setw(sizeof(&transform) * 2)
			<< std::hex << &transform;

		ImGui::BeginGroupPanel(stream.str().c_str());
		TransformDataEx(transform);
		ImGui::EndGroupPanel();
	}

	void Mesh_(const scuff3d::Mesh* mesh) {
		ImGui::BeginGroupPanel("Mesh");

		const int count = mesh->getCount();
		const unsigned int* indices = mesh->getIndices();

		const glm::vec3* positions = mesh->getPositions();
		const glm::vec3* normals = mesh->getNormals();
		const glm::vec3* tangents = mesh->getTangents();
		const glm::vec3* bitangents = mesh->getBitangents();
		const glm::vec2* uvs = mesh->getUvs();
		const glm::vec4* colors = mesh->getColors();




		//TODO: render this shit

		if (indices) {

		}
		else {

		}




		ImGui::EndGroupPanel();
	}

}