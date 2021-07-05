#include "pch.h"
#include "Application.h"
#include "Utils/Development/ConsoleOutput.h"
#include <windowsx.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace scuff3d
{

	Application::Application() :
		m_running(true),
		m_fixedTickTime(1.0f / 60.0f),
		m_basicSettings(new Settings("Settings/basic.txt")),
		m_input(new Input()),
		m_timeAcc(0.0f), m_dt(0.0f), m_fixedFrame(false)
	{
		QueryPerformanceFrequency(&m_frequency);
		QueryPerformanceCounter(&m_start);
		QueryPerformanceCounter(&m_end);






	}
	Application::Application(HWND hwnd) : Application::Application()
	{
		m_handle = hwnd;
		DEVLOG("Init");
	}

	Application::~Application() {


	}

	void Application::exit()
	{
		stop();
	}
	void Application::stop() {
		m_running = false;
	}

	bool Application::setWindow(HWND hwnd)
	{
		m_handle = hwnd;
		return true;
	}
	bool Application::setWindow(Window32* window)
	{
		m_handle = window->getHandle();
		m_input->setHwnd(m_handle); 
		m_window.reset(window);



		return false;
	}

	bool Application::initRenderer(RenderingAPI api)
	{
		switch (api) {
		case RenderingAPI::DX11:
			m_renderer.reset(new RendererDX11(m_handle));
			return true;
		case RenderingAPI::DX12:
			m_renderer.reset(new RendererDX12(m_handle));
			return true;
		default:
			return false;
		}

		return false;
	}

	//returns dt
	float Application::beginFrame() {
		m_input->beginFrame();
		m_fixedFrame = false;
		QueryPerformanceCounter(&m_start);
		m_dt = (float)((m_start.QuadPart - m_end.QuadPart) * 1.0 / m_frequency.QuadPart);
		m_timeAcc += m_dt;
		if (m_timeAcc >= m_fixedTickTime) {
			m_timeAcc -= m_fixedTickTime;
			m_fixedFrame = true;
		}
		m_end = m_start;

		//DEVLOG("ApplicationBegin");
		return 0.0f;
	}


	void Application::preUpdate() {
	}

	void Application::update() {
	}

	void Application::postUpdate() {
	}

	bool Application::preFixedUpdate() {
		if (!m_fixedFrame)
			return m_fixedFrame;



		return m_fixedFrame;
	}

	void Application::fixedUpdate() {

	}

	void Application::postFixedUpdate() {

	}

	bool Application::preRender() {
		if (m_renderer == nullptr)
			return false;
		m_renderer->beginFrame();
		return true;
	}

	void Application::render(std::function<void()> imguiFunc) {
		m_renderer->renderImGui(imguiFunc);
	}

	void Application::present() {

		m_renderer->present();
	}

	void Application::endFrame() {
	}


	bool Application::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);

		switch (message) {

		case WM_DESTROY:
			exit();
			break;
		case WM_SYSCOMMAND:
			if (wParam == SC_KEYMENU && (lParam >> 16) <= 0) return 0;
			break;
		case WM_KEYDOWN:
			// Only process first keystroke, skip repeats
			if ((HIWORD(lParam) & KF_REPEAT) == 0) {
				m_input->setkeyDown(wParam);
				DEVLOG("keydown(" + std::to_string(wParam) + ")");
			}
			break;
		case WM_KEYUP: 
			m_input->setkeyUp(wParam);
			DEVLOG("keyup(" + std::to_string(wParam) + ")");
			break;
		case WM_SYSKEYDOWN:
			if ((HIWORD(lParam) & KF_REPEAT) == 0) {
				m_input->setkeyDown(wParam);
				DEVLOG("syskeydown(" + std::to_string(wParam) + ")");
			}
			break;
		case WM_SYSKEYUP:
			m_input->setkeyUp(wParam);
			DEVLOG("syskeyup(" + std::to_string(wParam) + ")");
			break;

		 case WM_MBUTTONDOWN:
			 m_input->setkeyDown(VK_MBUTTON);
			 DEVLOG("mbuttonDown(" + std::to_string(VK_MBUTTON) + ")");
			 break;
		 case WM_RBUTTONDOWN:
			 m_input->setkeyDown(VK_RBUTTON);
			 DEVLOG("mbuttonDown(" + std::to_string(VK_RBUTTON) + ")");
			 break;
		 case WM_LBUTTONDOWN:
			 m_input->setkeyDown(VK_LBUTTON);
			 DEVLOG("mbuttonDown(" + std::to_string(VK_LBUTTON) + ")");
			 break;
		 case WM_XBUTTONDOWN:
			 m_input->setkeyDown(GET_XBUTTON_WPARAM(wParam) == 1 ? VK_XBUTTON1 : VK_XBUTTON2);
			 DEVLOG("mbuttonDown(" + std::to_string(GET_XBUTTON_WPARAM(wParam)==1? VK_XBUTTON1 : VK_XBUTTON2) + ")");
			 break;


		 case WM_MBUTTONUP:
			 m_input->setkeyUp(VK_MBUTTON);
			 DEVLOG("mbuttonUp(" + std::to_string(VK_MBUTTON) + ")");
			 break;
		 case WM_RBUTTONUP:
			 m_input->setkeyUp(VK_RBUTTON);
			 DEVLOG("mbuttonUp(" + std::to_string(VK_RBUTTON) + ")");
			 break;
		 case WM_LBUTTONUP:
			 m_input->setkeyUp(VK_LBUTTON);
			 DEVLOG("mbuttonUp(" + std::to_string(VK_LBUTTON) + ")");
			 break;
		 case WM_XBUTTONUP:
			 m_input->setkeyUp(GET_XBUTTON_WPARAM(wParam) == 1 ? VK_XBUTTON1 : VK_XBUTTON2);
			 DEVLOG("mbuttonUp(" + std::to_string(GET_XBUTTON_WPARAM(wParam) == 1 ? VK_XBUTTON1 : VK_XBUTTON2) + ")");
			 break;



		 case WM_MOUSEMOVE:
			 //DEVLOG(, );
			 //m_input->updateMousePos((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
			 break;



		}


		return 1;
	}

	const bool Application::isRunning() const {
		return m_running;
	}

	const float Application::getFixedTickTime() const {
		return m_fixedTickTime;
	}

}


//
//DEVLOG(std::to_string(MK_LBUTTON))
//DEVLOG(std::to_string(MK_MBUTTON))
//DEVLOG(std::to_string(MK_RBUTTON))
//DEVLOG(std::to_string(MK_XBUTTON1))
//DEVLOG(std::to_string(MK_XBUTTON2))