#include "pch.h"
#include "Extensions.h"

void ImGui::Text(const std::string& str)
{
		ImGui::Text(str.c_str());
}

bool ImGui::Button(const std::string& str, const ImVec2& size_arg)
{
	return ImGui::Button(str.c_str(), size_arg);
}
