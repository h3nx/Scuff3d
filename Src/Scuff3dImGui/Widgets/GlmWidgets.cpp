#include "pch.h"
#include "GlmWidgets.h"

namespace scuff3dImGui {
	void Mat4(const glm::mat4& matrix) {
		const int decimalPlaces = 2;
		ImGui::BeginGroupPanel("glm mat4");
		ImGui::Text(scuff3d::toString(matrix[0][0], decimalPlaces)); ImGui::SameLine(60);
		ImGui::Text(scuff3d::toString(matrix[0][1], decimalPlaces)); ImGui::SameLine(120);
		ImGui::Text(scuff3d::toString(matrix[0][2], decimalPlaces)); ImGui::SameLine(180);
		ImGui::Text(scuff3d::toString(matrix[0][3], decimalPlaces));

		ImGui::Text(scuff3d::toString(matrix[1][0], decimalPlaces)); ImGui::SameLine(60);
		ImGui::Text(scuff3d::toString(matrix[1][1], decimalPlaces)); ImGui::SameLine(120);
		ImGui::Text(scuff3d::toString(matrix[1][2], decimalPlaces)); ImGui::SameLine(180);
		ImGui::Text(scuff3d::toString(matrix[1][3], decimalPlaces));

		ImGui::Text(scuff3d::toString(matrix[2][0], decimalPlaces)); ImGui::SameLine(60);
		ImGui::Text(scuff3d::toString(matrix[2][1], decimalPlaces)); ImGui::SameLine(120);
		ImGui::Text(scuff3d::toString(matrix[2][2], decimalPlaces)); ImGui::SameLine(180);
		ImGui::Text(scuff3d::toString(matrix[2][3], decimalPlaces));

		ImGui::Text(scuff3d::toString(matrix[3][0], decimalPlaces)); ImGui::SameLine(60);
		ImGui::Text(scuff3d::toString(matrix[3][1], decimalPlaces)); ImGui::SameLine(120);
		ImGui::Text(scuff3d::toString(matrix[3][2], decimalPlaces)); ImGui::SameLine(180);
		ImGui::Text(scuff3d::toString(matrix[3][3], decimalPlaces));
		
		ImGui::EndGroupPanel();
	}
}
