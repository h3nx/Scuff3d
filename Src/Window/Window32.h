#pragma once
#include <windows.h>
#include <glm/vec2.hpp>
#include <string>
#include <map>
#include <functional>
namespace scuff3d
{
	class Window32 {
	public:
		// windowed, (normal, maximised)
		Window32(HINSTANCE hInstance, WNDPROC proc, const std::string& name, const glm::vec2& size, const glm::vec2& pos, const bool maximised = false);
		// borderless fullscreen
		Window32(HINSTANCE hInstance, WNDPROC proc, const std::string& name, const int monitor);
		~Window32();

		HWND& getHandle();
		HINSTANCE& getHInstance();



		const glm::vec2 getSize();
		const glm::vec2 getClientSize();
		const glm::vec2 getPosition();
		const glm::vec2 getNormalPosition();
		const glm::vec2 getNormalSize();
		const bool isMaximised();
		const bool isMinimised();
		const bool isFullscreen();

		const RECT getRect();
		const RECT getClientRect();
		const RECT getNormalRect();
		const std::string& getTitle() const;
		void setWindowTitle(const std::string& title);

		const WINDOWPLACEMENT getPlacement();
		
		void setWindowPos(const glm::vec2& pos);
		void setWindowSize(const glm::vec2& size);
		void setWindowRect(const glm::vec2& pos, const glm::vec2& size);

		void setWindowed(const glm::vec2& pos, const glm::vec2& size);
		void setFullscreen();


		const std::string getFileName(LPCWSTR filter = L"All\0*.*\0\0");


	private:
		ATOM registerClass(const std::string& className);
		ATOM registerClassW(const std::wstring& className);

		HWND createWindow(const glm::vec2& size, const glm::vec2& pos, const std::string& name, const DWORD& style = WS_OVERLAPPEDWINDOW);
		HWND createWindowW(const glm::vec2& size, const glm::vec2& pos, const std::wstring& name, const DWORD& style = WS_OVERLAPPEDWINDOW);

	protected:
		HINSTANCE m_hInstance;
		WNDPROC m_proc;
		HWND m_handle;
		ATOM m_identifier;
		std::string m_title;

		bool m_fullscreen;






	};


}