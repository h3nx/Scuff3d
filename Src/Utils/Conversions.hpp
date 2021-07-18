#pragma once
#include <string>
#include <sstream>


namespace scuff3d
{
	inline std::string toString(const float value, const int decimalPlaces = 2) {
		std::ostringstream out;
		out.precision(decimalPlaces);
		out << std::fixed << value;
		return out.str();
	}




	inline std::string wStringToString(const std::wstring& wstring) {
		//MultiByteToWideChar
		return std::string();
	}

}


