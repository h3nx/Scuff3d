#pragma once
#include <map>
#include <queue>
#include "WindowsKeyCodes.h"

namespace scuff3d
{
	class Input {
	public:
		Input(const int numKeysToDefine = 256);
		~Input();
		void setHwnd(HWND hwnd);

		void beginFrame();

		//KEYCODES
		void setkeyDown(const int key);
		void setkeyUp(const int key);

		const bool keyDown(const int key);
		const bool keyUp(const int key);
		const bool keyPressed(const int key);


		void setKeybind(const std::string& name, const int keycode);
		void removeKeybind(const std::string& name);


		//ACTIONS
		// returning storage index
		const int addActionDown(const std::string& keybind, std::function<void()> func);
		const int addActionDown(const int keycode, std::function<void()> func);
		const int addActionUp(const std::string& keybind, std::function<void()> func);
		const int addActionUp(const int keycode, std::function<void()> func);

		// use returned storage index for removal
		void removeActionDown(const std::string& keybind, const int index);
		void removeActionDown(const int keycode, const int index);
		void removeActionUp(const std::string& keybind, const int index);
		void removeActionUp(const int keycode, const int index);



		void confineCursor(const bool state);
		void lockCursor(const bool state, const bool savePosition = true);
		void hideCursor(const bool state);

		void updateMousePos(const glm::vec2& mousePos);
		void updateMousePos(const float x, const float y);

		void renderImGuiContent();

	protected:
		HWND m_hwnd;
		class Key {
		public:
			Key() {
				state = false;
				down = false;
				up = false;
			};
			~Key() { }

			std::vector<std::function<void()>> onDownActions;
			std::vector<std::function<void()>> onUpActions;

			bool state;
			bool down;
			bool up;
			
			void resetFrameState() { down = false;  up = false; }
			void triggerDownActions() {
				for (auto& func : onDownActions) {
					if (func)
						func();
				}
			}
			void triggerUpActions() {
				for (auto& func : onUpActions) {
					if(func)
						func();
				}
			}
		};
		std::vector<Key> m_keys;
		std::queue<int> m_keysToReset;
		std::map<std::string, int> m_keybinds;


		//TODO: Tidy this shite
		glm::vec2 m_mousePosition;
		glm::vec2 m_savedMousePos;
		RECT m_windowRect;
		glm::vec2 m_lockPosition;
		glm::vec2 m_mouseDelta;

		bool m_confineCursor;
		bool m_lockCursor;
		bool m_hideCursor;
		bool m_lockedThisFrame;


	private:
		void initKey(const int index);

		void renderKeycodeDropdown(const std::string& title, int&keycode);

	};

}


