#include "pch.h"
#include "CameraController.h"


namespace scuff3d {

	CameraController::CameraController(Input* input,
		const std::string& forward,
		const std::string& backward,
		const std::string& right,
		const std::string& left,
		const std::string& up,
		const std::string& down
	) : 
	m_forward(forward),
	m_backward(backward),
	m_right(right),
	m_left(left),
	m_up(up),
	m_down(down)
	{
		m_input = input;
		m_inControl = false;
		m_sens = 0.005;
	}

	CameraController::~CameraController() {
	}

	void CameraController::init() {
		m_transform = getGameObject()->getComponent<Transform>();
	}

	void CameraController::update(const float& dt) {
		
		if (m_input->keybindDown("k1")) {
			setInControl(!m_inControl);
			//DEVLOG("k1");
		}
		if (m_input->keybindDown("Exit"))
			setInControl(false);
		if (!m_inControl) return;
		
		
		glm::vec3 movement = {
			(int)m_input->keybindPressed(m_right) - (int)m_input->keybindPressed(m_left),
			(int)m_input->keybindPressed(m_up) - (int)m_input->keybindPressed(m_down),
			(int)m_input->keybindPressed(m_forward) - (int)m_input->keybindPressed(m_backward),
		};
		

		glm::vec2 rotation = -m_input->getMouseDelta() * m_sens;

		glm::vec3 forward = m_transform->getForward();
		glm::vec3 right = m_transform->getRight();
		glm::vec3 up = m_transform->getUp();


		m_transform->translate((forward * movement.z + right * movement.x + up * movement.y)*dt);
		m_transform->rotate(rotation.y, rotation.x, 0);



	}

	void CameraController::setInControl(const bool state) { 
		m_inControl = state;
		if (m_inControl) {
			m_input->hideCursor(true);
			m_input->lockCursor(true);
			m_input->hideCursor(true);
		}
		else {
			m_input->hideCursor(false);
			m_input->lockCursor(false);
			m_input->hideCursor(false);

		}
	}

	void CameraController::renderImGui() { 
		ImGui::Text("in control");
		ImGui::SameLine();
		ImGui::Checkbox("##dunno", &m_inControl);

		ImGui::Text("addDelta");
		ImGui::SameLine();
		ImGui::rText(to_string(m_accDelta));

	}

}