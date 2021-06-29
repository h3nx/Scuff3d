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

	const std::string currentDateTime() {
		//https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[80];
		localtime_s(&tstruct, &now);
		strftime(buf, sizeof(buf), "%H%M%S", &tstruct);

		return buf;
	}

}

#endif