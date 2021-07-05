#include "pch.h"
#include "Input.h"

namespace scuff3d
{
	Input::Input(const int numKeysToDefine) {
		for (int i = 0; i < numKeysToDefine; i++) {
			m_keys.emplace_back(Key());
		}
		m_lockedThisFrame = false;
	}

	Input::~Input() {

	}

	void Input::setHwnd(HWND hwnd) {
		m_hwnd = hwnd;
	}

	void Input::beginFrame() {
		while (m_keysToReset.size() > 0) {
			m_keys[m_keysToReset.front()].resetFrameState();
			m_keysToReset.pop();
		}
		m_mouseDelta = glm::zero<glm::vec2>();

		POINT p;
		GetCursorPos(&p);
		//ClientToScreen(m_hwnd, &p);
		ScreenToClient(m_hwnd, &p);
		updateMousePos(p.x,p.y);
		m_lockedThisFrame = false;
	}

	void Input::setkeyDown(const int key) {
		m_keys[key].down = true;
		m_keys[key].state = true;
		m_keys[key].triggerDownActions();
		m_keysToReset.push(key);
	}
	void Input::setkeyUp(const int key) {
		m_keys[key].up = true;
		m_keys[key].state = false;
		m_keys[key].triggerUpActions();
		m_keysToReset.push(key);

	}


	const bool Input::keyDown(const int key) {

		return m_keys[key].down;
	}
	const bool Input::keyUp(const int key) {

		return m_keys[key].up;
	}
	const bool Input::keyPressed(const int key) {

		return m_keys[key].state;
	}


	void Input::setKeybind(const std::string& name, const int keycode) {
		m_keybinds[name] = keycode;
	}

	void Input::removeKeybind(const std::string& name) {
		m_keybinds.erase(name);
	}


	const int Input::addActionDown(const std::string& keybind, std::function<void()> func) {
		return addActionDown(m_keybinds[keybind], func);
	}
	const int Input::addActionDown(const int keycode, std::function<void()> func) {
		m_keys[keycode].onDownActions.push_back(func);
		return m_keys[keycode].onDownActions.size()-1;
	}
	const int Input::addActionUp(const std::string& keybind, std::function<void()> func) {
		return addActionUp(m_keybinds[keybind], func);
	}
	const int Input::addActionUp(const int keycode, std::function<void()> func) {
		m_keys[keycode].onUpActions.push_back(func);
		return m_keys[keycode].onUpActions.size() - 1;
	}

	void Input::removeActionDown(const std::string& keybind, const int index) {
		removeActionDown(m_keybinds[keybind], index);
	}
	void Input::removeActionDown(const int keycode, const int index) {
		m_keys[keycode].onDownActions[index] = std::function<void()>();
	}
	void Input::removeActionUp(const std::string& keybind, const int index) {
		removeActionUp(m_keybinds[keybind], index);
	}
	void Input::removeActionUp(const int keycode, const int index) {
		m_keys[keycode].onUpActions[index] = std::function<void()>();
	}
	void Input::confineCursor(const bool state) {
		// https://docs.microsoft.com/en-us/windows/win32/learnwin32/other-mouse-operations
		if (state) {
			// Get the window client area.
			GetClientRect(m_hwnd, &m_windowRect);
			// Convert the client area to screen coordinates.
			POINT pt = { m_windowRect.left, m_windowRect.top };
			POINT pt2 = { m_windowRect.right, m_windowRect.bottom };
			ClientToScreen(m_hwnd, &pt);
			ClientToScreen(m_hwnd, &pt2);
			SetRect(&m_windowRect, pt.x, pt.y, pt2.x, pt2.y);

			// Confine the cursor.
			ClipCursor(&m_windowRect);
		}
		else {
			ClipCursor(NULL);
		}


	}
	void Input::lockCursor(const bool state, const bool savePosition) {
		m_lockCursor = state;
		m_lockedThisFrame = true;
		if (state) {
			// Get the window client area.
			GetClientRect(m_hwnd, &m_windowRect);
			// Convert the client area to screen coordinates.
			POINT pt = { m_windowRect.left, m_windowRect.top };
			POINT pt2 = { m_windowRect.right, m_windowRect.bottom };
			ClientToScreen(m_hwnd, &pt);
			ClientToScreen(m_hwnd, &pt2);
			SetRect(&m_windowRect, pt.x, pt.y, pt2.x, pt2.y);
			if (savePosition) {

				POINT mousepos = { m_mousePosition.x, m_mousePosition.y };
				ClientToScreen(m_hwnd, &mousepos);

				m_savedMousePos = glm::vec2(mousepos.x, mousepos.y);
				//savePosition 
			}
			m_lockPosition.x = (int)(float((float)m_windowRect.left + (float)m_windowRect.right) * 0.5f);
			m_lockPosition.y = (int)(float((float)m_windowRect.top + (float)m_windowRect.bottom) * 0.5f);
			m_mousePosition = m_lockPosition;
		}
		else {
			if (savePosition) {
				SetCursorPos(m_savedMousePos.x, m_savedMousePos.y);
			}
		}
	}
	void Input::hideCursor(const bool state) {
		ShowCursor(!state);
	}

	void Input::updateMousePos(const glm::vec2& mousePos) {
		if (!m_lockedThisFrame) {
			m_mouseDelta = m_mousePosition - mousePos;

		}
		
		if (m_lockCursor) {
			SetCursorPos(m_lockPosition.x, m_lockPosition.y); // screenspace

			m_mousePosition = m_lockPosition - glm::vec2((float)m_windowRect.left,(float)m_windowRect.top);
		}
		else {
			m_mousePosition = mousePos;
		}


	}

	void Input::updateMousePos(const float x, const float y)
	{
		updateMousePos(glm::vec2(x, y));
	}

	//void Input::setScreenCenter(const glm::vec2& center) {
	//	m_lockPosition = center;
	//}




	void Input::initKey(const int index) {
		m_keys[index] = Input::Key();
	}


	void Input::renderImGuiContent() {
		
		static glm::vec2 temp = glm::vec2(0, 0);
		temp += m_mouseDelta;
		if (ImGui::CollapsingHeader("Mouse")) {
			ImGui::Text("Position("+std::to_string(m_mousePosition.x)+","+ std::to_string(m_mousePosition.y)+")");
			ImGui::Text("Delta("+std::to_string(m_mouseDelta.x)+","+ std::to_string(m_mouseDelta.y)+")");
			ImGui::Text("SavedPos("+std::to_string(m_savedMousePos.x)+","+ std::to_string(m_savedMousePos.y)+")");
			ImGui::Text("LockPos("+std::to_string(m_lockPosition.x)+","+ std::to_string(m_lockPosition.y)+")");
			ImGui::Text("");
			ImGui::Text("Test("+std::to_string(temp.x)+","+std::to_string(temp.y)+")");
		}

		if (ImGui::CollapsingHeader("Keybinds")) {
			for (auto var : m_keybinds)
			{
				Key* k = &m_keys[var.second];
				if (k->state)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0,240,0,255));
				if(k->down)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0,0, 240,255));
				if (k->up)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(240,0,0,255));

				if (ImGui::TreeNode((var.first + " -> " + keycodeName(var.second) +"("+ std::to_string(var.second)+")").c_str())) {


					ImGui::Text("keycode");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(150);

					renderKeycodeDropdown("change KC", m_keybinds[var.first]);
					if (ImGui::Button(("remove keybind##"+var.first).c_str())) {

						removeKeybind(var.first);
						ImGui::TreePop();
						break;
					}

					ImGui::TreePop();
				}

				if (k->state)
					ImGui::PopStyleColor();
				if (k->down)
					ImGui::PopStyleColor();
				if (k->up)
					ImGui::PopStyleColor();
			}
			static char buf[152] = "";
			static int keycode = 0;
			ImGui::SetNextItemWidth(150);
			if (ImGui::InputTextWithHint("##newKeybindName", "", buf, 30)) {

			}
			ImGui::SameLine();
			ImGui::SetNextItemWidth(150);
			renderKeycodeDropdown("newKB", keycode);
			ImGui::SameLine();
			if (ImGui::Button("Add keybind")) {
				setKeybind(buf, keycode);
				strncpy_s(buf, "", 0);
				keycode = 0;
			}

		}
	
	}

	void Input::renderKeycodeDropdown(const std::string& title, int & keycode)
	{
		if (ImGui::BeginCombo(("##keycodeCombo"+title).c_str(), (keycodeName(keycode) + "(" + std::to_string(keycode) + ")").c_str(), ImGuiComboFlags_NoArrowButton)) {

			unsigned int i = 0;
			std::string keyName = "";
			for (int i = 0; i < m_keys.size(); i++) {
				keyName = keycodeName(i);
				if (keyName == "N/A") continue;
				if (ImGui::Selectable((keyName + "(" + std::to_string(i) + ")").c_str(), keycode == i)) {
					keycode = i;
					break;
				}
			}
			ImGui::EndCombo();
		}

	}




}