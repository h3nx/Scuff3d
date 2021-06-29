#include "pch.h"
#include "Window32.h"


namespace scuff3d
{
	Window32::Window32(HINSTANCE hInstance, WNDPROC proc, const std::string& className,const glm::vec2& size, const glm::vec2& pos) {
		m_hInstance = hInstance;
		m_proc = proc;
		m_identifier = this->registerClass(className);
		m_handle = this->createWindow(size, pos, className);
		ShowWindow(m_handle, true);
	}

	Window32::~Window32() {
	}

	HWND& Window32::getHandle() {
		return m_handle;
	}

	const glm::vec2 Window32::getSize()
	{
		RECT rect = getRect();
		return glm::vec2(rect.right - rect.left, rect.bottom - rect.top);
	}

	const glm::vec2 Window32::getPosition()
	{
		RECT rect = getRect();
		return glm::vec2(rect.left, rect.top);
	}

	const RECT Window32::getRect()
	{
		RECT rect;
		GetWindowRect(m_handle, &rect);
		//DEVLOG("Window Rect("+ glm::to_string(glm::vec4(rect.left,rect.top,rect.left,rect.bottom))+")");
		return rect;
	}

	void Window32::setWindowTitle(std::string title)
	{
		DEVLOG("Setting window title (" + title + ")");
		SetWindowTextA(m_handle, title.c_str());
	}

	ATOM Window32::registerClass(const std::string& className) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wide = converter.from_bytes(className);

		return registerClassW(wide);
	}

	HWND Window32::createWindow(const glm::vec2& size, const glm::vec2& pos, const std::string& name) {

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wide = converter.from_bytes(name);
		return createWindowW(size, pos, wide);

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

	HWND Window32::createWindowW(const glm::vec2& size, const glm::vec2& pos, const std::wstring& name)
	{
		return CreateWindowExW(
			0,
			name.c_str(),
			name.c_str(),
			WS_OVERLAPPEDWINDOW,
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