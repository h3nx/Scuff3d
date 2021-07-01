#include "pch.h"
#include "Input.h"

namespace scuff3d
{
	Input::Input(const int numKeysToDefine) {
		for (int i = 0; i < numKeysToDefine; i++) {
			m_keys.emplace_back(Key());
		}
	}

	Input::~Input() {

	}

	void Input::beginFrame() {
		while (m_keysToReset.size() > 0) {
			m_keys[m_keysToReset.front()].resetFrameState();
			m_keysToReset.pop();
		}
	}

	void Input::setkeyDown(const int key) {
		m_keys[key].down = true;
		m_keys[key].state = true;
	}
	void Input::setkeyUp(const int key) {
		m_keys[key].up = true;
		m_keys[key].state = false;
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

	void Input::renderImGuiContent() {
		

		if (ImGui::CollapsingHeader("Keybinds")) {
			for (auto var : m_keybinds)
			{
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

	void Input::initKey(const int index) {
		m_keys[index] = Input::Key();
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