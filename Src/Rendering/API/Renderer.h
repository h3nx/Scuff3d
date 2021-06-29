#pragma once
#include <Windows.h>
#include <functional>

class Renderer {
public:
	Renderer(HWND hwnd);
	virtual ~Renderer();

	virtual void init() = 0;

	virtual void beginFrame() = 0;
	virtual void present() = 0;
	virtual void endFrame() = 0;

	virtual void render(std::function<void()> imguiFunc) = 0;

protected:
	HWND m_handle;


};

