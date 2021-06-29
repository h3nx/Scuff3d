#pragma once
#include <windows.h>
#include <glm/vec2.hpp>
#include <string>
namespace scuff3d
{
	class Window32 {
	public:
		Window32(HINSTANCE hInstance, WNDPROC proc, const std::string& name = "Grove" ,const glm::vec2& size = glm::vec2(1280, 720), const glm::vec2& pos = glm::vec2(0, 0));
		~Window32();

		HWND& getHandle();

		const glm::vec2 getSize();
		const glm::vec2 getPosition();
		const RECT getRect();

		void setWindowTitle(std::string title);


	private:
		ATOM registerClass(const std::string& className);
		HWND createWindow(const glm::vec2& size, const glm::vec2& pos, const std::string& name);

		ATOM registerClassW(const std::wstring& className);
		HWND createWindowW(const glm::vec2& size, const glm::vec2& pos, const std::wstring& name);


	private:
		HINSTANCE m_hInstance;
		WNDPROC m_proc;
		HWND m_handle;
		ATOM m_identifier;

	};


}