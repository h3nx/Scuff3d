#pragma once
#include <windows.h>
#include <functional>
#include "Window/Window32.h"
#include "Settings/Settings.h"
#include "Input/Input.h"
#include "Rendering/DX11/RendererDX11.h"
#include "Rendering/DX12/RendererDX12.h"
namespace scuff3d
{
	enum class RenderingAPI {
		DX11,
		DX12
	};

	class Application {
	public:
		Application();
		//use with external window
		Application(HWND hwnd);
		virtual ~Application();
		virtual void exit();
		void stop();

		//bool initWindow(const glm::vec2& size = glm::vec2(1280, 720), const glm::vec2& pos = glm::vec2(0,0));
		bool setWindow(HWND hwnd);
		bool setWindow(Window32* window);
		bool initRenderer(RenderingAPI api);

		virtual void Frame() = 0;

		virtual float beginFrame();

		virtual void preUpdate();
		virtual void update();
		virtual void postUpdate();

		virtual bool preFixedUpdate();
		virtual void fixedUpdate();
		virtual void postFixedUpdate();

		virtual bool preRender();
		virtual void render(std::function<void()> imguiFunc);
		virtual void present();

		virtual void endFrame();

		virtual bool wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		const bool isRunning() const;
		const float getFixedTickTime() const;

	protected:
		HWND m_handle;
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<Window32> m_window;
		std::unique_ptr<Settings> m_basicSettings;
		std::unique_ptr<Input> m_input;
		bool m_running;
		float m_fixedTickTime;

		// timekeeping
		LARGE_INTEGER m_frequency;
		LARGE_INTEGER m_start, m_end;
		float m_timeAcc;
		float m_dt;
		bool m_fixedFrame;



	private:


	};


}