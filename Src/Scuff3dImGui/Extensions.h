#pragma once
#include <string>
#include "imgui/imgui.h"

namespace ImGui {

	inline void Text(const std::string& str) {
		ImGui::Text(str.c_str());
	}
	inline bool Button(const std::string& str, const ImVec2& size_arg) {
		return ImGui::Button(str.c_str(), size_arg);
	}

	// Source:  https://github.com/ocornut/imgui/issues/1496
	//			https://github.com/ocornut/imgui/issues/1496#issuecomment-655048353
	// Credits: https://github.com/thedmd
	void BeginGroupPanel(const std::string& name, const ImVec2& size = ImVec2(0.0f, 0.0f));
	void BeginGroupPanel(const char* name, const ImVec2& size = ImVec2(0.0f, 0.0f));
	void EndGroupPanel();
	inline void rText(const char* fmt, const float end, ...) {
		float rSide = end == 0.0f ? ImGui::GetContentRegionAvailWidth() : end;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + rSide - ImGui::CalcTextSize(fmt).x
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);

		va_list args;
		va_start(args, end);
		ImGui::TextV(fmt, args);
		va_end(args);
	}
	inline void rText(const std::string& str, const float& end = 0.0f) {
		rText(str.c_str(),end);
	}
	



}