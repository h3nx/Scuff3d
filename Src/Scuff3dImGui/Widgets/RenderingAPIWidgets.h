#pragma once
#include "Rendering/API/Geometry/TransformData.h"
#include "imgui/imgui.h"

class Mesh;

namespace scuff3dImGui {
	void TransformData(scuff3d::TransformData* transform);
	void TransformDataEx(scuff3d::TransformData* transform);
	void TransformDataExBorder(scuff3d::TransformData* transform);

	//TODO: find better function name
	void Mesh_(const Mesh* mesh);

}