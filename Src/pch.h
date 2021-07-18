// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here

// c++ windows etc

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS 

#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <functional>
#include <memory>
#include <algorithm>


#include <locale> // utf8 ansi stuff
#include <codecvt> // converter string - wstring


//GLM

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"

// ImGui

#include "imgui/imgui.h"
#include "Scuff3dImGui/Extensions.h"

// Scuff3d

#include "Utils/Development/ConsoleOutput.h"
#include "Utils/Console.h"
#include "Utils/Conversions.hpp"
#include "Utils/FileIO.h"
#include "Utils/DataExtensions.h"
#include "Input/WindowsKeyCodes.h"

#include "Rendering/API/Geometry/TransformData.h"



#endif //PCH_H
