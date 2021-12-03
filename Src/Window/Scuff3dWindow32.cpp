#include "pch.h"
#include "Scuff3dWindow32.h"
#include "imgui\imgui.h"
namespace scuff3d {
	Scuff3dWindow32::Scuff3dWindow32(HINSTANCE hInstance, WNDPROC proc, const std::string& name, const glm::vec2& size, const glm::vec2& pos, const bool maximised) : 
	Window32(hInstance, proc, name, size, pos, maximised),
	Input(256)
	{ 
		setHwnd(m_handle);
	}
	Scuff3dWindow32::Scuff3dWindow32(HINSTANCE hInstance, WNDPROC proc, const std::string& name, const int monitor) :
	Window32(hInstance, proc, name, monitor),
	Input(256)
	{
		setHwnd(m_handle);
	}
	Scuff3dWindow32::~Scuff3dWindow32() {

	}
	void Scuff3dWindow32::renderImGui() { 
		//if (ImGui::BeginChild(std::string("##Window_input_" + m_title).c_str())) {
			Input::renderImGuiContent();
		//}
		//ImGui::EndChild();

		//if (ImGui::BeginChild(std::string("##Window_window_" + m_title).c_str())) {
			const std::vector<glm::ivec2> standardSizes = {
			{1280,720},
			{1600,900},
			{1920,1040},
			{1920,1080},
			{2560,1040},
			{2560,1080},
			{3840,1040},
			{3840,1080},
			};

			glm::vec2 size = getSize();
			glm::vec2 clientSize = getClientSize();

			ImGui::Text("Window Position");
			ImGui::SameLine();
			ImGui::rText(to_string(getPosition()));

			ImGui::Text("Window Size:");
			ImGui::SameLine();
			//ImGui::rText(to_string(size));
			if (ImGui::BeginCombo("##windowSizes", to_string(size).c_str(), ImGuiComboFlags_NoArrowButton)) {
				for (auto& item : standardSizes) {
					if (ImGui::Selectable(scuff3d::to_string(item).c_str(), (glm::ivec2)size == item)) {
						setWindowSize(item);
					}
				}
				ImGui::EndCombo();
			}
			ImGui::Text("Client Size:");
			ImGui::SameLine();
			ImGui::rText(to_string(clientSize));

		//}
		//ImGui::EndChild();

		
			bool fullscreenState = m_fullscreen;
			if (ImGui::Checkbox("", &fullscreenState)) {
				if (m_fullscreen) {
					setWindowed({ 100,50 }, standardSizes.front());
				}
				else {
					setFullscreen();
				}
			}






	}



	bool Scuff3dWindow32::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		if (hWnd != m_handle) return false;

		switch (message) {
		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_ACTIVE) {
				runMap(m_onActivate);
			}
			else if (LOWORD(wParam) == WA_CLICKACTIVE) {
				runMap(m_onActivate);
			}
			else if (LOWORD(wParam) == WA_INACTIVE) {
				runMap(m_onDeactivate);
				//m_input->setAllKeysUp();
			}
			runMap(m_onStatusChange, LOWORD(wParam) > 0);
			break;
		case WM_SIZE:
		case WM_SIZING:
			runMap(m_onClientResize, getClientSize());
			break;



		case WM_KEYDOWN:
			// Only process first keystroke, skip repeats
			if ((HIWORD(lParam) & KF_REPEAT) == 0) {
				setkeyDown((const int)wParam);
				//DEVLOG("keydown(" + std::to_string((const int)wParam) + ")");
			}
			break;
		case WM_KEYUP:
			setkeyUp((const int)wParam);
			//DEVLOG("keyup(" + std::to_string((const int)wParam) + ")");
			break;
		case WM_SYSKEYDOWN:
			if ((HIWORD(lParam) & KF_REPEAT) == 0) {
				setkeyDown((const int)wParam);
				//DEVLOG("syskeydown(" + std::to_string((const int)wParam) + ")");
			}
			break;
		case WM_SYSKEYUP:
			setkeyUp((const int)wParam);
			//DEVLOG("syskeyup(" + std::to_string((const int)wParam) + ")");
			break;

		case WM_MBUTTONDOWN:
			setkeyDown(VK_MBUTTON);
			//DEVLOG("mbuttonDown(" + std::to_string(VK_MBUTTON) + ")");
			break;
		case WM_RBUTTONDOWN:
			setkeyDown(VK_RBUTTON);
			//DEVLOG("mbuttonDown(" + std::to_string(VK_RBUTTON) + ")");
			break;
		case WM_LBUTTONDOWN:
			setkeyDown(VK_LBUTTON);
			//DEVLOG("mbuttonDown(" + std::to_string(VK_LBUTTON) + ")");
			break;
		case WM_XBUTTONDOWN:
			setkeyDown(GET_XBUTTON_WPARAM(wParam) == 1 ? VK_XBUTTON1 : VK_XBUTTON2);
			//DEVLOG("mbuttonDown(" + std::to_string(GET_XBUTTON_WPARAM(wParam)==1? VK_XBUTTON1 : VK_XBUTTON2) + ")");
			break;


		case WM_MBUTTONUP:
			setkeyUp(VK_MBUTTON);
			// DEVLOG("mbuttonUp(" + std::to_string(VK_MBUTTON) + ")");
			break;
		case WM_RBUTTONUP:
			setkeyUp(VK_RBUTTON);
			//DEVLOG("mbuttonUp(" + std::to_string(VK_RBUTTON) + ")");
			break;
		case WM_LBUTTONUP:
			setkeyUp(VK_LBUTTON);
			//DEVLOG("mbuttonUp(" + std::to_string(VK_LBUTTON) + ")");
			break;
		case WM_XBUTTONUP:
			setkeyUp(GET_XBUTTON_WPARAM(wParam) == 1 ? VK_XBUTTON1 : VK_XBUTTON2);
			// DEVLOG("mbuttonUp(" + std::to_string(GET_XBUTTON_WPARAM(wParam) == 1 ? VK_XBUTTON1 : VK_XBUTTON2) + ")");
			break;









		}



		return false;
	}

	void Scuff3dWindow32::newFrame() {
		Input::beginFrame();
	}

	void Scuff3dWindow32::endFrame() { 
		Input::endFrame();
	}

	void Scuff3dWindow32::onActivate(const std::string& identifier, std::function<void()> func) {
		m_onActivate[identifier] = func;
	}
	void Scuff3dWindow32::onDeactivate(const std::string& identifier, std::function<void()> func) {
		m_onDeactivate[identifier] = func;

	}
	void Scuff3dWindow32::onStatusChange(const std::string& identifier, std::function<void(const bool state)> func) {
		m_onStatusChange[identifier] = func;

	}

	void Scuff3dWindow32::onResizeStart(const std::string& identifier, std::function<void()> func) {
		m_onResizeStart[identifier] = func;

	}
	void Scuff3dWindow32::onResizeEnd(const std::string& identifier, std::function<void()> func) {
		m_onResizeEnd[identifier] = func;

	}
	void Scuff3dWindow32::onClientResize(const std::string& identifier, std::function<void(const glm::vec2& size)> func) {
		m_onClientResize[identifier] = func;

	}

	void Scuff3dWindow32::onMoveStart(const std::string& identifier, std::function<void()> func) {
		m_onMoveStart[identifier] = func;

	}
	void Scuff3dWindow32::onMoveEnd(const std::string& identifier, std::function<void()> func) {
		m_onMoveEnd[identifier] = func;

	}
	void Scuff3dWindow32::onMove(const std::string& identifier, std::function<void(const glm::vec2& pos)> func) {
		m_onMove[identifier] = func;

	}
















}








/*








*/