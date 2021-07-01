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


		void renderImGuiContent();


	protected:
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

		};
		std::vector<Key> m_keys;
		std::queue<int> m_keysToReset;
		std::map<std::string, int> m_keybinds;

	private:
		void initKey(const int index);


		void renderKeycodeDropdown(const std::string& title, int&keycode);

	};

}


