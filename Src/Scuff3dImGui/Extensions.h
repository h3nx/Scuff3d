#pragma once
#include <string>
#include "imgui/imgui.h"

namespace ImGui {

	void Text(const std::string& str);
	bool Button(const std::string& str, const ImVec2& size_arg);

}