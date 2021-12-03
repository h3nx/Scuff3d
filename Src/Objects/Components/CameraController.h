#pragma once
#include "../Component.h"
#include "Transform.h"
#include "Input/Input.h"

#include "glm/glm.hpp"

namespace scuff3d {

	class CameraController : public Component<CameraController>
	{
	public:
		CameraController(Input* input, 
			const std::string& forward = "Forward", 
			const std::string& backward = "Back", 
			const std::string& right = "Right", 
			const std::string& left = "Left",
			const std::string& up = "Space",
			const std::string& down = "Down"
		);
		~CameraController();

		void init();
		void update(const float& dt);

		void setInControl(const bool state);


		void renderImGui();

	private:
		Transform* m_transform;
		//glm::vec3 m_input;
		Input* m_input;
		bool m_inControl;
		glm::vec2 m_accDelta;
		float m_sens;

		std::string m_forward;
		std::string m_backward;
		std::string m_right;
		std::string m_left;
		std::string m_up;
		std::string m_down;
	};

}