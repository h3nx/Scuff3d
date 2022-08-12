#pragma once
#ifndef EXPORT_H
#define EXPORT_H
#include <string>
#include <fstream>
#include <iostream>
namespace scuff3d
{

	inline bool readFile(const std::string fileName, std::string& s) {
		std::ifstream file;
		file.open(fileName);
		if (!file.is_open())
			return false;
		std::string line;
		while (getline(file, line)) {
			s += line + '\n';
		}
		file.close();
		return true;
	}


	inline bool toFile(std::string fileName, std::string s, int mode) {
		std::ofstream file;
		file.open(fileName, mode);
		if (file.is_open()) {
			file << s;
			file.close();
			return true;
		}
		return false;
	}

	

	

}

#endif