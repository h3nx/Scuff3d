#include "pch.h"
#include "Scuff3dImGuiWindow.h"
namespace scuff3d
{

	Scuff3dImGuiWindow::Scuff3dImGuiWindow(const std::string& title, const ImVec2& pos , const ImVec2& size, const bool lockPosition, const bool lockSize) {
		m_title = title;
		m_position = pos;
		m_size = size;
		m_lockPosition = lockPosition;
		m_lockSize = lockSize;
		m_flags = 0;
	}

	Scuff3dImGuiWindow::~Scuff3dImGuiWindow() {

	}

	void Scuff3dImGuiWindow::render(std::function<void()> content) {
		ImGui::SetNextWindowPos(m_position, ImGuiCond_Once);
		ImGui::SetNextWindowSize(m_size, ImGuiCond_Once);

		ImGuiWindowFlags flags = m_flags;
		if (m_lockSize) { flags |= ImGuiWindowFlags_NoResize; }
		if (m_lockPosition) { flags |= ImGuiWindowFlags_NoMove; }

		if (ImGui::Begin(m_title.c_str(), &m_active, flags)){
			if (!m_lockPosition) { m_position = ImGui::GetWindowPos(); }
			if (!m_lockSize) { m_size = ImGui::GetWindowSize(); }
			content();
		}
		ImGui::End();
	}


	void Scuff3dImGuiWindow::setWindowTitle(const std::string& title) {
		m_title = title;
	}
	

	void Scuff3dImGuiWindow::setPosition(const ImVec2& pos) {
		m_position = pos;
	}
	void Scuff3dImGuiWindow::setSize(const ImVec2& size) {
		m_size = size;
	}

	void Scuff3dImGuiWindow::setFlags(const ImGuiWindowFlags& flags) {
		m_flags = flags;
	}


	void Scuff3dImGuiWindow::setActive(const bool status) {
		m_active = status;
	}
	const bool Scuff3dImGuiWindow::isActive()
	{
		return m_active;
	}
	

	void Scuff3dImGuiWindow::toggle() {
		setActive(!m_active);
	}
	void Scuff3dImGuiWindow::show() {
		setActive(false);
	}
	void Scuff3dImGuiWindow::hide() {
		setActive(false);
	}

}