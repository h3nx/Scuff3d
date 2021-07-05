#pragma once
#include "imgui/imgui.h"
namespace scuff3d {
	class Scuff3dImGuiWindow {
	public:
		Scuff3dImGuiWindow(
			const std::string& title, 
			const ImVec2& pos = ImVec2(0.0f,0.0f), 
			const ImVec2& size = ImVec2(640,360),
			const bool lockPosition = false,
			const bool lockSize = false
		);
		~Scuff3dImGuiWindow();


		void render(std::function<void()> content);

		void setWindowTitle(const std::string& title);
		void setPosition(const ImVec2& pos);
		void setSize(const ImVec2& size);
		void setFlags(const ImGuiWindowFlags& flags);

		void setActive(const bool status);
		const bool isActive();
		void toggle();
		void show();
		void hide();

		const glm::vec2 getPosition();


	private:
		
		bool m_lockPosition;
		bool m_lockSize;
		std::string m_title;
		ImVec2 m_position;
		ImVec2 m_size;
		bool m_active;
		ImGuiWindowFlags m_flags;
		bool m_posDirty;


	};
}