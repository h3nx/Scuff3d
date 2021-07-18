#pragma once
#include <string>
#include "imgui/imgui.h"

namespace ImGui {

	void Text(const std::string& str);
	bool Button(const std::string& str, const ImVec2& size_arg);

	// Source:  https://github.com/ocornut/imgui/issues/1496
	//			https://github.com/ocornut/imgui/issues/1496#issuecomment-655048353
	// Credits: https://github.com/thedmd
	void BeginGroupPanel(const char* name, const ImVec2& size = ImVec2(0.0f, 0.0f));
	void EndGroupPanel();

}