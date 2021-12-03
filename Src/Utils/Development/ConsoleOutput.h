#pragma once
#include <string>

#include <windows.h>
#include <iostream>
#ifdef _DEBUG

#define INCLUDE_CALLING_LOCATION true







#define DEVLOGCUSTOM(message, color) { \
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE); \
	CONSOLE_SCREEN_BUFFER_INFO csbi; \
	GetConsoleScreenBufferInfo(hstdout, &csbi); \
	SetConsoleTextAttribute(hstdout, color); \
	std::string file(__FILE__);\
	std::string msg(""); \
	if(INCLUDE_CALLING_LOCATION) { \
		msg = \
			\
			file.substr(file.find_last_of("\\")+1,std::string::npos) + \
			"@" + std::to_string(int(__LINE__)) + ": " + \
			\
			message; \
	} else {\
		msg = message; \
	}\
	std::cout << msg << std::endl; \
	SetConsoleTextAttribute(hstdout, csbi.wAttributes); \
}

#define DEVLOG(message)  DEVLOGCUSTOM(message, 0x0F);
#define DEVLOGSUCCESS(message)  DEVLOGCUSTOM(message, 0X0A); 
#define DEVLOGERROR(message)  DEVLOGCUSTOM(message, 0X0C); 
#define DEVLOGWARNING(message)  DEVLOGCUSTOM(message, 0X0E); 

inline void initConsole() {

	AllocConsole();
	FILE* a;
	freopen_s(&a, "CONIN$", "r", stdin);
	freopen_s(&a, "CONOUT$", "w", stdout);
	freopen_s(&a, "CONOUT$", "w", stderr);
}
inline void releaseConsole(bool pause = false) {
	if (pause) {
		system("pause");
	}
	FreeConsole();
}




#else

#define DEVLOGCUSTOM(message, color)
#define DEVLOG(message)
#define DEVLOGSUCCESS(message)
#define DEVLOGERROR(message)
#define DEVLOGWARNING(message)

#endif


