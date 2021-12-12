#include "pch.h"
#include "Window32.h"
#include "Utils/Conversions.hpp"

namespace scuff3d
{
	Window32::Window32(HINSTANCE hInstance, WNDPROC proc, const std::string& name,const glm::vec2& size, const glm::vec2& pos, const bool maximised) {
		m_hInstance = hInstance;
		m_proc = proc;
		m_identifier = this->registerClass(name);
		m_handle = this->createWindow(size, pos, name);
		ShowWindow(m_handle, maximised ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
		UpdateWindow(m_handle);
		m_fullscreen = false;

	}
	Window32::Window32(HINSTANCE hInstance, WNDPROC proc, const std::string& name, const int monitor) {
		m_hInstance = hInstance;
		m_proc = proc;
		m_identifier = this->registerClass(name);

		glm::vec2 pos = { 0,0 };
		glm::vec2 size = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
		m_handle = this->createWindow(size, pos, name, WS_POPUP);
		ShowWindow(m_handle, SW_SHOWNORMAL);
		UpdateWindow(m_handle);

		m_fullscreen = true;
	}
	Window32::~Window32() {
	}

	HWND& Window32::getHandle() {
		return m_handle;
	}

	HINSTANCE& Window32::getHInstance() {
		return m_hInstance;
	}

	const glm::vec2 Window32::getSize() {
		RECT rect = getRect();
		return glm::vec2(rect.right - rect.left, rect.bottom - rect.top);
	}
	const glm::vec2 Window32::getClientSize() {
		RECT rect = getClientRect();
		return glm::vec2(rect.right, rect.bottom);
	}
	const glm::vec2 Window32::getNormalSize() {
		auto& rect = getNormalRect();
		return glm::vec2(rect.right-rect.left, rect.bottom-rect.top);
	}
	const glm::vec2 Window32::getPosition() {
		RECT rect = getRect();
		return glm::vec2(rect.left, rect.top);
	}
	const glm::vec2 Window32::getClientScreenPosition() {
		POINT p;
		p.x = 0;
		p.y = 0;
		ClientToScreen(m_handle, &p);
		return glm::vec2((float)p.x, (float)p.y);
	}
	const glm::vec2 Window32::getNormalPosition() {
		auto& rect = getNormalRect();
		return glm::vec2(rect.left, rect.top);
	}


	const bool Window32::isMaximised() {
		return getPlacement().showCmd == SW_SHOWMAXIMIZED;
	}

	const bool Window32::isMinimised() {
		return getPlacement().showCmd == SW_SHOWMINIMIZED;
	}

	const bool Window32::isFullscreen() {

		return m_fullscreen;
	}

	const RECT Window32::getRect()
	{
		RECT rect;
		GetWindowRect(m_handle, &rect);
		//DEVLOG("Window Rect("+ glm::to_string(glm::vec4(rect.left,rect.top,rect.left,rect.bottom))+")");
		return rect;
	}
	const RECT Window32::getClientRect() {
		RECT rect;
		GetClientRect(m_handle, &rect);
		//DEVLOG("Window Rect("+ glm::to_string(glm::vec4(rect.left,rect.top,rect.left,rect.bottom))+")");
		return rect;
	}
	const RECT Window32::getNormalRect() {
		return getPlacement().rcNormalPosition;
	}


	const std::string& Window32::getTitle() const {
		return m_title;
	}
	void Window32::setWindowTitle(const std::string& title) {
		DEVLOG("Setting window title (" + title + ")");
		SetWindowTextA(m_handle, title.c_str());
		m_title = title;
	}
	
	const WINDOWPLACEMENT Window32::getPlacement() {
		WINDOWPLACEMENT placement;
		placement.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(m_handle, &placement);
		return placement;
	}

	void Window32::setWindowPos(const glm::vec2& pos) { 
		setWindowRect(pos, getSize());
	}
	void Window32::setWindowSize(const glm::vec2& size) {
		setWindowRect(getPosition(), size);

	}
	void Window32::setWindowRect(const glm::vec2& pos, const glm::vec2& size) { 
		MoveWindow(
			m_handle,
			(int)pos.x,
			(int)pos.y,
			(int)size.x,
			(int)size.y,
			false
		);
	}

	void Window32::setWindowed(const glm::vec2& pos, const glm::vec2& size) {
		m_fullscreen = false;
		SetWindowLongPtr(m_handle, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
		setWindowRect(pos, size);
	}

	void Window32::setFullscreen() {
		m_fullscreen = true;
		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);
		SetWindowLongPtr(m_handle, GWL_STYLE, WS_VISIBLE | WS_POPUP);
		SetWindowPos(m_handle, HWND_TOP, 0, 0, w, h, SWP_FRAMECHANGED);
	}

	// EXAMPLE FILTER: 
	// L"All (*.*)\0*.*\0PDF (*.pdf)\0*.pdf\0img (*.png;*.jpeg;*.jpg)\0*.png;*.jpeg;*.jpg\0\0"
	const std::string Window32::getFileName(LPCWSTR filter) {
		OPENFILENAME ofn;       // common dialog box structure
		ZeroMemory(&ofn, sizeof(ofn));
		wchar_t name[260];

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = m_handle;
		ofn.lpstrFile = name;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(name);
		LPCWSTR filterWC = filter;
		ofn.lpstrFilter = filterWC;
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		// Display the Open dialog box. 

		if (GetOpenFileName(&ofn) == TRUE) {
			DEVLOG("file opened("+wCharToString(ofn.lpstrFile)+")");
			return wCharToString(ofn.lpstrFile);
		}
		else {
			DEVLOG("file cancelled");
			exit(0);
		}
		
		return "";
	}



	ATOM Window32::registerClass(const std::string& className) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wide = converter.from_bytes(className);

		return registerClassW(wide);
	}
	ATOM Window32::registerClassW(const std::wstring& className)
	{
		WNDCLASSEXW wcex;

		wcex.cbSize = sizeof(WNDCLASSEXW);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = m_proc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = m_hInstance;
		wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
		wcex.lpszClassName = className.c_str();
		wcex.lpszMenuName = NULL;
		wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		return RegisterClassExW(&wcex);
	}

	HWND Window32::createWindow(const glm::vec2& size, const glm::vec2& pos, const std::string& name, const DWORD& style) {

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wide = converter.from_bytes(name);
		return createWindowW(size, pos, wide, style);

	}
	HWND Window32::createWindowW(const glm::vec2& size, const glm::vec2& pos, const std::wstring& name, const DWORD& style) {
		return CreateWindowExW(
			0,
			name.c_str(),
			name.c_str(),
			style,
			(int)pos.x,
			(int)pos.y,
			(int)size.x,
			(int)size.y,
			nullptr,
			nullptr,
			m_hInstance,
			nullptr
		);
	}


}