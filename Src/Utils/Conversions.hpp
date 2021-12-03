#pragma once
#include <string>
#include <sstream>
#include <filesystem>
#include "glm/glm.hpp"
#include "imgui/imgui.h"

namespace scuff3d
{
	inline std::string toString(const float value, const int decimalPlaces = 2) {
		std::ostringstream out;
		out.precision(decimalPlaces);
		out << std::fixed << value;
		return out.str();
	}


	inline std::wstring stringToWstring(const std::string& string) {
		return std::filesystem::path(string).generic_wstring();
	}

	inline std::string wStringToString(const std::wstring& wstring) {
		return std::filesystem::path(wstring).generic_string();
	}
	inline std::string wCharToString(WCHAR* c) {
		return std::filesystem::path(c).generic_string();
	}

	inline std::string to_string(const glm::vec2& vec, const int decimalPlaces = 2) {
		return "(" + toString(vec.x, decimalPlaces) + ", "+toString(vec.y, decimalPlaces) + ")";
	}
	inline std::string to_string(const glm::vec3& vec, const int decimalPlaces = 2) {
		return "(" + toString(vec.x, decimalPlaces) + ", " + toString(vec.y, decimalPlaces) + ", " + toString(vec.z, decimalPlaces) + ")";
	}
	inline std::string to_string(const glm::vec4& vec, const int decimalPlaces = 2) {
		return "(" + toString(vec.x, decimalPlaces) + ", " + toString(vec.y, decimalPlaces) + ", " + toString(vec.z, decimalPlaces) + ", " + toString(vec.a, decimalPlaces) + ")";
	}



	inline glm::vec2 ImVec2ToGlmVec2(const ImVec2& imvec) {
		return glm::vec2(imvec[0],imvec[1]);
	}
	
}


