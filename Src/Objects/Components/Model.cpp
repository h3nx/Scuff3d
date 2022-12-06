#include "pch.h"
#include "Model.h"

void scuff3d::Model::render(const float& dt, Renderer* renderer)
{



}

void scuff3d::Model::renderImGui() {



	if (ImGui::CollapsingHeader("Mesh")) {
		m_data->getMesh()->renderImGui();
	}


}
