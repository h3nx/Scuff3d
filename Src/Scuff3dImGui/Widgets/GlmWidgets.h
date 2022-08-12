#pragma once
#include "imgui/imgui.h"
#include "glm/common.hpp"

namespace scuff3dImGui {
	void Mat4(const glm::mat4& matrix);
	inline void TextVec2(const glm::vec2& vec) {
		ImGui::Text("("+std::to_string(vec.x) + "," + std::to_string(vec.y) + ")");
	}

	inline void Vec3Var(const std::string& name, glm::vec3& vec, const float& offset = 0) {
		ImGui::Text(name.c_str());
		ImGui::SameLine(offset);

		float* ptr = static_cast<float*>(&vec.x);
		ImGui::PushID(ptr);
		if (ImGui::DragFloat3("##vec", ptr, 0.1f)) {
			
		}
		ImGui::PopID();
	}

}