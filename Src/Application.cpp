#include "pch.h"
#include "Application.h"
#include "Utils/Development/ConsoleOutput.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace scuff3d
{

	Application::Application() :
		m_running(true),
		m_fixedTickTime(1.0f / 60.0f),
		m_basicSettings(new Settings("Settings/basic.txt")),
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
		m_window.reset(window);



		return false;
	}

	bool Application::initRenderer(RenderingAPI api)
	{
		return false;
	}

	//returns dt
	float Application::beginFrame() {
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
		//DEVLOG("dt:" + std::to_string(dt) +"s");
		if (!m_fixedFrame)
			return m_fixedFrame;



		return m_fixedFrame;
	}

	bool Application::fixedUpdate() {
		if (!m_fixedFrame)
			return m_fixedFrame;



		return m_fixedFrame;
	}

	bool Application::postFixedUpdate() {
		if (!m_fixedFrame)
			return m_fixedFrame;




		return m_fixedFrame;
	}

	void Application::preRender() {
	}

	void Application::render() {
	}

	void Application::postRender() {
	}

	void Application::endFrame() {
	}


	void Application::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		//ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
	}

	const bool Application::isRunning() const {
		return m_running;
	}

	const float Application::getFixedTickTime() const {
		return m_fixedTickTime;
	}

}