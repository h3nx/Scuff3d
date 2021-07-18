#pragma once
#include <windows.h>
#include <functional>
#include "Window/Window32.h"
#include "Settings/Settings.h"
#include "Input/Input.h"
#include "Rendering/DX11/RendererDX11.h"
#include "Rendering/DX12/RendererDX12.h"
#include "Scene.h"
#include "Scuff3dImGui/Window/Scuff3dImGuiWindow.h"

namespace scuff3d
{

	class Scene;
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

		void Frame(std::function<void()> imguiFunc);

		virtual void beginFrame();

		virtual void preUpdate(const float dt);
		virtual void update(const float dt);
		virtual void postUpdate(const float dt);

		virtual void preFixedUpdate(const float dt);
		virtual void fixedUpdate(const float dt);
		virtual void postFixedUpdate(const float dt);

		virtual void preRender();
		virtual void render(std::function<void()> imguiFunc);
		virtual void present();

		virtual void endFrame();

		virtual bool wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		const bool isRunning() const;
		const float getFixedTickTime() const;

		void loadScene(Scene* scene);
		void loadSceneAsync(Scene* scene);
		void popScene();
		void removeScene(Scene* scene);
		void clearScenes();
		void pauseScene(Scene* scene);
		void resumeScene(Scene* scene);

		void updateOnlyTopScene();
		void updateAllScenes();

	protected:
		HWND m_handle;
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<Window32> m_window;
		std::unique_ptr<Settings> m_basicSettings;
		std::unique_ptr<Input> m_input;
		std::vector<Scene*> m_sceneStack;
		bool m_running;
		float m_fixedTickTime;

		// timekeeping
		LARGE_INTEGER m_frequency;
		LARGE_INTEGER m_start, m_end;
		float m_timeAcc;
		float m_dt;
		bool m_fixedFrame;




		//IMGUI STUFF


		std::unique_ptr<scuff3d::Scuff3dImGuiWindow> m_toggleDebugWindow;
		std::unique_ptr<scuff3d::Scuff3dImGuiWindow> m_rendererDebugWindow;
		std::unique_ptr<scuff3d::Scuff3dImGuiWindow> m_windowDebugWindow;
		std::unique_ptr<scuff3d::Scuff3dImGuiWindow> m_settingsDebugWindow;
		std::unique_ptr<scuff3d::Scuff3dImGuiWindow> m_scenesDebugWindow;

		void renderImGuiDebug_toggle();
		void renderImGuiDebug_scenes();



	private:

		void processSceneChanges();
		enum class SceneChanges {
			ADD,
			POP,
			REMOVE,
			CLEAR,
			RESUME,
			PAUSE
		};
		std::queue < std::pair<SceneChanges, Scene*>> m_sceneChanges;
		bool m_processOnlyTopScene;

	};


}