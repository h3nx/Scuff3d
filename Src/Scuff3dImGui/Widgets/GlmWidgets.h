#pragma once
#include "imgui/imgui.h"
#include "glm/common.hpp"

namespace scuff3dImGui {
	void Mat4(const glm::mat4& matrix);
	inline void TextVec2(const glm::vec2& vec) {
		ImGui::Text("("+std::to_string(vec.x) + "," + std::to_string(vec.y) + ")");
	}

}