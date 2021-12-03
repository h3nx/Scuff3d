#pragma once
#include <crtdbg.h>

void logMemLeaks() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
}



























//DEVLOG("main application init : " + std::to_string((float)((end.QuadPart - start.QuadPart) * 1.0 / frequency.QuadPart)) + "s");

//DEVLOG("int size: " + std::to_string(sizeof(int)));
//DEVLOG("uint size: " + std::to_string(sizeof(unsigned int)));
//DEVLOG("float size: " + std::to_string(sizeof(float)));
//DEVLOG("double size: " + std::to_string(sizeof(double)));
//DEVLOG("long int size: " + std::to_string(sizeof(long long int)));
//DEVLOG("short size: " + std::to_string(sizeof(short)));















