#pragma once
#include <windows.h>
#include <functional>
#include "Window/Scuff3dWindow32.h"
#include "Settings/Settings.h"
#include "Input/Input.h"
#include "Rendering/DX11/RendererDX11.h"
#include "Rendering/DX12/RendererDX12.h"
#include "Scene.h"
#include "Scuff3dImGui/Window/Scuff3dImGuiWindow.h"
#include "ResourceManager/ResourceManager.h"
#include "PerformaceTracker\PerformanceTracker.h"
#include <thread>

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
		bool setWindow(Scuff3dWindow32* window);
		// windowed
		Scuff3dWindow32* createWindow(HINSTANCE hInstance, WNDPROC wndProc, const std::string& name, const glm::vec2& size, const glm::vec2& pos, const bool maximised = false);
		// fullscreen
		Scuff3dWindow32* createWindow(HINSTANCE hInstance, WNDPROC wndProc, const std::string& name);
		
		bool initRenderer(RenderingAPI api);
		Renderer* getRenderer() { return m_renderer.get(); };

		ResourceManager* getResourceManager() { return m_resourceManager.get(); };
		
		Input* getInput();
		Window32* getWindow();

		/*
			Frame Stuff
		*/

		void Frame(std::function<void()> imguiFunc);

		virtual void beginFrame();

		virtual void preUpdate(const float dt);
		virtual void update(const float dt);
		virtual void postUpdate(const float dt);

		virtual void preFixedUpdate(const float dt);
		virtual void fixedUpdate(const float dt);
		virtual void postFixedUpdate(const float dt);

		virtual bool preRender();
		virtual void render(std::function<void()> imguiFunc);
		virtual void present();

		virtual void endFrame();

		virtual bool wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		/*
			Getting stuff
		*/


		const bool isRunning() const;
		const float getFixedTickTime() const;

		/*
			Scene stuff
		*/

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
		Scuff3dWindow32* m_window;
		std::unique_ptr<Settings> m_basicSettings;
		std::unique_ptr<ResourceManager> m_resourceManager;
		std::unique_ptr<PerformanceTracker> m_performanceTracker;
		std::vector<Scene*> m_sceneStack;
		bool m_running;
		float m_fixedTickTime;

		// timekeeping
		LARGE_INTEGER m_frequency;
		LARGE_INTEGER m_start, m_end;
		float m_timeAcc;
		float m_dt;
		bool m_fixedFrame;


#ifdef _EDITOR
		GameObject* m_editorCamera;
#endif

		void resizedWindow(HWND hWnd);
		void resizedWindow(const int x, const int y);

		//IMGUI STUFF


		std::map<std::string, scuff3d::Scuff3dImGuiWindow*> m_debugWindows;

		void renderImGuiDebug_toggle();
		void renderImGuiDebug_testing();
		void renderImGuiDebug_renderer();
		//void renderImGuiDebug_window();
		void renderImGuiDebug_settings();
		void renderImGuiDebug_scenes();
		void renderImGuiDebug_resourceManager();
		void renderImGuiDebug_input();
		void renderImGuiDebug_perf();

		//bool editorWndProc(HWND hWnd, UINT message, WPARAM, LPARAM lParam);


	private:

		std::thread m_resizeMoveUpdateThread;
		bool m_runResizeMoveUpdateThread;
		bool m_inFrame;

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