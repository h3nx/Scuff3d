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
		m_handle(NULL),
		m_fixedTickTime(1.0f / 60.0f),
		m_basicSettings(new Settings("Settings/basic.txt")),
		m_input(new Input()),
		m_timeAcc(0.0f), m_dt(0.0f), m_fixedFrame(false),
		m_toggleDebugWindow(new Scuff3dImGuiWindow("Debug Toggling", ImVec2(0, 0))),
		m_rendererDebugWindow(new Scuff3dImGuiWindow("Renderer", ImVec2(150, 0))),
		m_windowDebugWindow(new Scuff3dImGuiWindow("Window", ImVec2(250, 0))),
		m_settingsDebugWindow(new Scuff3dImGuiWindow("Settings", ImVec2(350, 0))),
		m_scenesDebugWindow(new Scuff3dImGuiWindow("Scenes", ImVec2(450, 0)))
	{
		QueryPerformanceFrequency(&m_frequency);
		QueryPerformanceCounter(&m_start);
		QueryPerformanceCounter(&m_end);

		m_rendererDebugWindow->setActive(false);
		m_windowDebugWindow->setActive(false);
		m_settingsDebugWindow->setActive(false);
		m_scenesDebugWindow->setActive(false);




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

	void Application::Frame(std::function<void()> imguiFunc) {
		processSceneChanges();
		beginFrame();

		preUpdate(m_dt);
		update(m_dt);
		postUpdate(m_dt);

		if (m_fixedFrame) {
			preFixedUpdate(m_fixedTickTime);
			fixedUpdate(m_fixedTickTime);
			postFixedUpdate(m_fixedTickTime);
		}

		if (m_renderer != nullptr) {
			preRender();
			render(imguiFunc);
			present();
		}

		endFrame();


	}

	//returns dt
	void Application::beginFrame() {
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
	}

	void Application::preUpdate(const float dt) {
		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->preUpdate(dt);
			if (m_processOnlyTopScene)
				break;
		}
	}
	void Application::update(const float dt) {
		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->update(dt);
			if (m_processOnlyTopScene)
				break;
		}
	}
	void Application::postUpdate(const float dt) {
		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->postUpdate(dt);
			if (m_processOnlyTopScene)
				break;
		}
	}

	void Application::preFixedUpdate(const float dt) {
		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->preFixedUpdate(dt);
			if (m_processOnlyTopScene)
				break;
		}
	}
	void Application::fixedUpdate(const float dt) {
		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->fixedUpdate(dt);
			if (m_processOnlyTopScene)
				break;
		}
	}
	void Application::postFixedUpdate(const float dt) {
		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->postFixedUpdate(dt);
			if (m_processOnlyTopScene)
				break;
		}
	}

	void Application::preRender() {
		m_renderer->beginFrame();

		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->preRender();
			if (m_processOnlyTopScene)
				break;
		}
	}
	void Application::render(std::function<void()> imguiFunc) {
		m_renderer->renderImGui([&]() {
			imguiFunc;
			m_toggleDebugWindow->render([&]() {renderImGuiDebug_toggle(); });
			m_rendererDebugWindow->render([&]() {});
			m_windowDebugWindow->render([&]() {});
			m_settingsDebugWindow->render([&]() {});
			m_scenesDebugWindow->render([&]() { renderImGuiDebug_scenes(); });

			});

		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->render();
			if (m_processOnlyTopScene)
				break;
		}
	}
	void Application::present() {

		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->postRender();
			if (m_processOnlyTopScene)
				break;
		}

		m_renderer->present();
	}

	void Application::endFrame() {
	}

	void Application::loadScene(Scene* scene) {
		m_sceneChanges.emplace(SceneChanges::ADD, scene);
	}
	void Application::loadSceneAsync(Scene* scene)
	{
	}
	void Application::popScene() {
		m_sceneChanges.emplace(SceneChanges::POP, nullptr);
	}
	void Application::removeScene(Scene* scene) {
		m_sceneChanges.emplace(SceneChanges::REMOVE, scene);
	}
	void Application::clearScenes() {
		m_sceneChanges.emplace(SceneChanges::CLEAR, nullptr);
	}

	void Application::pauseScene(Scene* scene) {
	}
	void Application::resumeScene(Scene* scene) {
	}
	void Application::updateOnlyTopScene() {
		m_processOnlyTopScene = true;
	}
	void Application::updateAllScenes() {
		m_processOnlyTopScene = false;
	}

	void Application::renderImGuiDebug_toggle() {

		bool status = false;
		std::vector<scuff3d::Scuff3dImGuiWindow*> windows = {
			m_rendererDebugWindow.get(),
			m_windowDebugWindow.get(),
			m_settingsDebugWindow.get(),
			m_scenesDebugWindow.get(),
		};

		for (auto window : windows) {
			status = window->isActive();
			if (ImGui::Checkbox(window->getTitle().c_str(), &status)) {
				window->setActive(status);
			}
		}
	}

	void Application::renderImGuiDebug_scenes() {
		ImGui::BeginGroupPanel("SceneStack");

		static int selectedIndex = -1;
		int i = 0;
		/*if (ImGui::CollapsingHeader("tsto")) {
			ImGui::Text("test");
			ImGui::Text("test");
			ImGui::Text("test");
			ImGui::Text("test");
			ImGui::Text("test");
			ImGui::Text("test");
			ImGui::Text("test");
			ImGui::Text("test");
			ImGui::Text("test");
			ImGui::Text("test");
			ImGui::Text("test");
			ImGui::Text("test");
		}*/
		for (Scene* scene : m_sceneStack) {
			if(ImGui::Selectable(scene->getName().c_str(), selectedIndex == i,0,ImVec2(100,0))) {
				if (selectedIndex == i) {
					selectedIndex = -1;
				}
				else {
					selectedIndex = i;
				}
			}
			i++;
		}
		ImGui::EndGroupPanel();

		ImGui::SameLine();
		if (selectedIndex > -1) {
			static int selectedObjectID = -1;
			Scene* scene = m_sceneStack[selectedIndex];
			
			ImGui::BeginGroupPanel((scene->m_name + " Info").c_str());
			{	
				ImGui::BeginGroupPanel("Objects");
				int index = 0;
				for (auto& obj : scene->m_gameObjects) {
					int id = obj.second;
					if (ImGui::Selectable(obj.first.c_str(), id == selectedObjectID, 0, ImVec2(120,0))) {
						if (selectedObjectID == id) {
							selectedObjectID = -1;
						}
						else {
							selectedObjectID = id;
						}
					}
						
				}



				ImGui::EndGroupPanel();
			}
			ImGui::EndGroupPanel();

			if (selectedObjectID > -1) {
				ImGui::BeginGroupPanel("Object Info", ImVec2(-1, 40));
				ImGui::Text("ID");
				ImGui::SameLine();
				ImGui::InputInt("##OBJECTID", &selectedObjectID, 0, 0, ImGuiInputTextFlags_ReadOnly);
				ImGui::EndGroupPanel();

			}
			else {
				ImGui::BeginGroupPanel("Object Info", ImVec2(-1,40));
				ImGui::EndGroupPanel();

		
			}



		}
		else {
			ImGui::BeginGroupPanel("Scene Info", ImVec2(120,0));
			ImGui::EndGroupPanel();
		}
	}

	void Application::processSceneChanges() {

		while (!m_sceneChanges.empty()) {
			auto item = m_sceneChanges.front();
			m_sceneChanges.pop();
			std::vector<Scene*>::iterator it;
			#ifdef _DEBUG
				std::string scenes = "";
			#endif
			switch (item.first) {
			case SceneChanges::ADD:
				DEVLOG("Adding scene (" + item.second->getName() + ")");
				m_sceneStack.push_back(item.second);
				item.second->m_app = this;
				break;
			case SceneChanges::POP:
				DEVLOG("Popping scene (" + m_sceneStack.back()->getName() + ")");
				m_sceneStack.pop_back();
				break;
			case SceneChanges::REMOVE:
				DEVLOG("Removing scene (" + item.second->getName() + ")");
				it = std::find(m_sceneStack.begin(), m_sceneStack.end(), item.second);
				delete (*it);
				m_sceneStack.erase(it);
				break;
			case SceneChanges::CLEAR:
				#ifdef _DEBUG
					scenes = "";
					for (Scene* scene : m_sceneStack)
						scenes += "\t" + scene->getName() + "\n";
					DEVLOG("Clearing all scenes:\n" + scenes);
				#endif
				for (int i = 0; i < m_sceneStack.size(); i++) {
					if (m_sceneStack[i]) {
						delete m_sceneStack[i];
					}
				}
				m_sceneStack.clear();
				break;
			case SceneChanges::RESUME:
				DEVLOG("Resuming scene("+item.second->getName()+")");
				item.second->resume();
				break;
			case SceneChanges::PAUSE:
				DEVLOG("Pausing scene(" + item.second->getName() + ")");
				DEVLOG("Clearing all scenes:\n" + scenes);
				item.second->pause();
				break;
			}
		}
		
		
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
				m_input->setkeyDown((const int)wParam);
				DEVLOG("keydown(" + std::to_string((const int)wParam) + ")");
			}
			break;
		case WM_KEYUP: 
			m_input->setkeyUp((const int)wParam);
			DEVLOG("keyup(" + std::to_string((const int)wParam) + ")");
			break;
		case WM_SYSKEYDOWN:
			if ((HIWORD(lParam) & KF_REPEAT) == 0) {
				m_input->setkeyDown((const int)wParam);
				DEVLOG("syskeydown(" + std::to_string((const int)wParam) + ")");
			}
			break;
		case WM_SYSKEYUP:
			m_input->setkeyUp((const int)wParam);
			DEVLOG("syskeyup(" + std::to_string((const int)wParam) + ")");
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