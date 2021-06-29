#pragma once
#include <string>



namespace scuff3d {


    // Courtesy of https://stackoverflow.com/a/24315631

    inline void replaceAll(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
    }

    inline std::string replaceAllRet(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        std::string s = str;
        while ((start_pos = s.find(from, start_pos)) != std::string::npos) {
            s.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
        return s;
    }


}




