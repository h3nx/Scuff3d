#include "pch.h"
#include "Scuff3dImGuiWindow.h"
namespace scuff3d
{

	Scuff3dImGuiWindow::Scuff3dImGuiWindow(const std::string& title, const ImVec2& pos , const ImVec2& size, const bool lockPosition, const bool lockSize) {
		m_title = title;
		m_position = pos;
		m_size = size;
		m_originalSize = size;
		m_lockPosition = lockPosition;
		m_lockSize = lockSize;
		m_flags = 0;
		m_posDirty = true;
	}

	Scuff3dImGuiWindow::Scuff3dImGuiWindow(const std::string& title, const std::function<void()> externalFunction, const ImVec2& pos, const ImVec2& size, const bool lockPosition, const bool lockSize) :
		Scuff3dImGuiWindow(title,pos,size,lockPosition,lockSize)
	{
		m_externalFunction = externalFunction;
	}

	Scuff3dImGuiWindow::Scuff3dImGuiWindow(const std::string& title, const bool lockPosition, const bool lockSize, const ImVec2& pos, const ImVec2& size) :
	Scuff3dImGuiWindow(title, pos,size,lockPosition,lockSize)
	{}

	Scuff3dImGuiWindow::~Scuff3dImGuiWindow() {

	}

	void Scuff3dImGuiWindow::render(const float& dt) {
		render(m_externalFunction, dt);
	}

	void Scuff3dImGuiWindow::render(std::function<void()> content, const float& dt) {
		if (!m_active)
			return;
		if (m_posDirty) {
			ImGui::SetNextWindowPos(m_position);
			m_posDirty = false;
		}
		ImGui::SetNextWindowSize(m_originalSize);
		//ImGui::SetNextWindowSize(ImVec2(0, 0));

		ImGuiWindowFlags flags = m_flags;
		if (m_lockSize) { flags |= ImGuiWindowFlags_NoResize; }
		if (m_lockPosition) { flags |= ImGuiWindowFlags_NoMove; }

		if (ImGui::Begin(m_title.c_str(), &m_active, flags)){
			if (!m_lockPosition) { m_position = ImGui::GetWindowPos(); }
			if (!m_lockSize) { m_size = ImGui::GetWindowSize(); }
			if(content)
				content();
		}
		ImGui::End();
	}


	void Scuff3dImGuiWindow::setWindowTitle(const std::string& title) {
		m_title = title;
	}
	

	void Scuff3dImGuiWindow::setPosition(const ImVec2& pos) {
		m_position = pos;
		m_posDirty = true;
	}
	void Scuff3dImGuiWindow::setPosition(const float& x, const float& y) {
		setPosition(ImVec2(x, y));
	}
	void Scuff3dImGuiWindow::setSize(const ImVec2& size) {
		m_size = size;
		m_originalSize = size;
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

	const glm::vec2 Scuff3dImGuiWindow::getPosition() {
		return ImVec2ToGlmVec2(m_position);
		//return glm::vec2(m_position.x,m_position.y);
	}

	const glm::vec2 Scuff3dImGuiWindow::getSize(const bool originalSize) {
		return originalSize ? 
			ImVec2ToGlmVec2(m_originalSize) :
			ImVec2ToGlmVec2(m_size)
		;
	}

	const std::string& Scuff3dImGuiWindow::getTitle() const
	{
		return m_title;
	}

}