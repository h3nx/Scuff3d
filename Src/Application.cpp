#include "pch.h"
#include "Application.h"
#include "Utils/Development/ConsoleOutput.h"
#include <windowsx.h>
#include "Rendering/API/Camera/CameraData.h"
#include "imgui/imgui_internal.h"
#include <chrono>
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace scuff3d
{

	Application::Application() :
		m_running(true),
		m_handle(NULL),
		m_fixedTickTime(1.0f / 60.0f),
		m_basicSettings(NEW Settings("Settings/basic.txt")),
		m_resourceManager(NEW ResourceManager()),
		m_performanceTracker(NEW PerformanceTracker()),
		m_timeAcc(0.0f), m_dt(0.0f), m_fixedFrame(false), 
		m_runResizeMoveUpdateThread(false),
		m_editorCamera(nullptr),
		m_window(nullptr),
		m_inFrame(false)
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

		for (auto& pair : m_debugWindows) {
			if (pair.second)
				delete pair.second;
			pair.second = nullptr;
		}
		for (auto& scene : m_sceneStack) {
			if (scene)
				delete scene;
			scene = nullptr;
		}
		delete m_window;
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
	bool Application::setWindow(Scuff3dWindow32* window) {
		m_handle = window->getHandle();
		m_window = window;
		//m_window = std::make_unique<Window32>(window);
		//const glm::vec2 size = { 500,500 };

		//m_editorWindow = std::make_unique<Window32>(NEW Window32(window->getHInstance(), editorWndProc, "Editor: " + window->getTitle(), size, { 0,0 }));


		return false;
	}

	Scuff3dWindow32* Application::createWindow(HINSTANCE hInstance, WNDPROC wndProc, const std::string& name, const glm::vec2& size, const glm::vec2& pos, const bool maximised) {
		m_window = NEW Scuff3dWindow32(hInstance, wndProc, name, size, pos, maximised);
		setWindow(m_window->getHandle());
		return m_window;
	}
	Scuff3dWindow32* Application::createWindow(HINSTANCE hInstance, WNDPROC wndProc, const std::string& name) {
		m_window = NEW Scuff3dWindow32(hInstance, wndProc, name, 0);
		setWindow(m_window->getHandle());
		return m_window;
	}
	bool Application::initRenderer(RenderingAPI api) {
		bool ret = false;
		switch (api) {
		case RenderingAPI::DX11:
			m_renderer.reset(NEW RendererDX11(m_window->getHandle(), m_window->getClientSize()));
			m_renderer->setViewPort("standard", { 0,0 }, m_window->getClientSize());
			ret = true;
			break;
		case RenderingAPI::DX12:
			m_renderer.reset(NEW RendererDX12(m_handle, m_window->getClientSize()));
			ret = true;
			break;
		default:
			return ret;
		}
#ifdef _EDITOR
		glm::vec2 origo = m_window->getClientScreenPosition();
		m_debugWindows["Hub"] = NEW Scuff3dImGuiWindow("Debug Hub", [&]() { renderImGuiDebug_toggle(); }, origo + glm::vec2(0, 0));
		m_debugWindows["Testing"] = NEW Scuff3dImGuiWindow("Testing", [&]() { renderImGuiDebug_testing(); }, origo + glm::vec2(200, 200));
		m_debugWindows["Renderer"] = NEW Scuff3dImGuiWindow("Renderer", [&]() { m_renderer->renderImGuiDebug(); }, origo + glm::vec2(0, 0));
		m_debugWindows["Settings"] = NEW Scuff3dImGuiWindow("Settings", [&]() { renderImGuiDebug_settings(); }, origo + glm::vec2(0, 0));
		m_debugWindows["Scenes"] = NEW Scuff3dImGuiWindow("Scenes", [&]() { renderImGuiDebug_scenes(); }, origo + glm::vec2(0, 0));
		m_debugWindows["Resource Manager"] = NEW Scuff3dImGuiWindow("Resource Manager", [&]() { renderImGuiDebug_resourceManager(); }, origo + glm::vec2(500, 200));
		m_debugWindows["Application Window"] = NEW Scuff3dImGuiWindow("Application Window", [&]() { m_window->renderImGui(); }, origo + glm::vec2(0, 0));
		m_debugWindows["Performance Tracker"] = NEW Scuff3dImGuiWindow("Performance Tracker", [&]() { m_performanceTracker->renderImGui(); }, origo + glm::vec2(800, 0));
		int i = 0;
		for (auto& pair : m_debugWindows) {
			pair.second->setActive(false);
			//pair.second->setPosition((i++) * 100.0f, 0.0f);
		}
		m_debugWindows["Hub"]->setActive(true);
		m_debugWindows["Testing"]->setActive(true);
		m_debugWindows["Resource Manager"]->setActive(true);


#endif

		m_resizeMoveUpdateThread = std::thread([&] {
			while (this->isRunning()) {
				if (this->m_runResizeMoveUpdateThread) {
					this->Frame([&] {});
				}
				std::this_thread::sleep_for(std::chrono::milliseconds((int)(13)));
			}
		});

		return ret;
	}

	Input* Application::getInput() {
		return m_window;
	}

	Window32* Application::getWindow() {
		return m_window;
	}

	void Application::Frame(std::function<void()> imguiFunc) {
		if (!m_running) return;
		if (m_inFrame) {
			DEVLOGWARNING("Attemped dual frame");
			return;
		}
		m_inFrame = true;
		m_performanceTracker->begin("Application");
		processSceneChanges();
		beginFrame();
		m_performanceTracker->begin("__Update__");
		preUpdate(m_dt);
		update(m_dt);
		postUpdate(m_dt);
		m_performanceTracker->end("__Update__");

//#ifdef _EDITOR
//		updateEditor(m_dt);
//#endif


		m_performanceTracker->begin("__Fixed Update__");
		if (m_fixedFrame) {
			preFixedUpdate(m_fixedTickTime);
			fixedUpdate(m_fixedTickTime);
			postFixedUpdate(m_fixedTickTime);

		}
		m_performanceTracker->end("__Fixed Update__", m_fixedFrame);
//#ifdef _EDITOR
//		if (m_fixedFrame) {
//			updateEditorFixed(m_fixedTickTime);
//		}
//#endif


		m_performanceTracker->begin("__Render__");
		if (m_renderer != nullptr && m_window->getHandle() != 0) {
			
			if (preRender()) {
				render(imguiFunc);
				present();

			}
		}
		m_performanceTracker->end("__Render__", m_renderer != nullptr);
//#ifdef _EDITOR
//		if (m_renderer != nullptr) {
//			renderEditor();
//		}
//#endif
		endFrame();
		m_performanceTracker->end("Application");

		m_inFrame = false;
	}

	//returns dt
	void Application::beginFrame() {
		m_window->beginFrame();
//#ifdef _EDITOR
//		m_editorWindow->beginFrame();
//#endif
		m_fixedFrame = false;
		m_performanceTracker->beginFrame();
		m_dt = m_performanceTracker->getDT();
		m_timeAcc += m_dt;
		if (m_dt > (1.0f/60.0f))
			DEVLOG("FUCKED DT("+std::to_string(m_dt)+")");
		if (m_timeAcc >= m_fixedTickTime) {
			m_timeAcc -= m_fixedTickTime;
			m_fixedFrame = true;
		}
	}

	void Application::preUpdate(const float dt) {
		m_performanceTracker->begin("Pre Update");
		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->preUpdate(dt);
			if (m_processOnlyTopScene)
				break;
		}
		m_performanceTracker->end("Pre Update");
	}
	void Application::update(const float dt) {
		m_performanceTracker->begin("Update");
		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->update(dt);
			if (m_processOnlyTopScene)
				break;
		}
		m_performanceTracker->end("Update");
	}
	void Application::postUpdate(const float dt) {
		m_performanceTracker->begin("Post Update");
		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->postUpdate(dt);
			if (m_processOnlyTopScene)
				break;
		}
		m_performanceTracker->end("Post Update");
	}

	void Application::preFixedUpdate(const float dt) {
		m_performanceTracker->begin("Pre Fixed Update");
		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->preFixedUpdate(dt);
			if (m_processOnlyTopScene)
				break;
		}
		m_performanceTracker->end("Pre Fixed Update");
	}
	void Application::fixedUpdate(const float dt) {
		m_performanceTracker->begin("Fixed Update");
		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->fixedUpdate(dt);
			if (m_processOnlyTopScene)
				break;
		}
		m_performanceTracker->end("Fixed Update");
//#ifdef _EDITOR
//		updateEditorFixed(dt);
//#endif
	}
	void Application::postFixedUpdate(const float dt) {
		m_performanceTracker->begin("Post Fixed Update");
		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->postFixedUpdate(dt);
			if (m_processOnlyTopScene)
				break;
		}
		m_performanceTracker->end("Post Fixed Update");
	}

	bool Application::preRender() {
		m_performanceTracker->begin("Pre render");
		bool status = m_renderer->beginFrame(m_window->getHandle(),"standard");
		if (status == false)
			return status;
		m_renderer->beginImGui();

		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->preRender();
			if (m_processOnlyTopScene)
				break;
		}
		m_performanceTracker->end("Pre render");
		return status;
	}
	void Application::render(std::function<void()> imguiFunc) {
		m_performanceTracker->begin("Render");

		m_performanceTracker->begin("Imgui Render");
		m_renderer->renderImGui([&]() {
			imguiFunc;
			for (auto& pair : m_debugWindows) {
				pair.second->render();
			}
		});
		m_performanceTracker->end("Imgui Render");

		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->render();
			if (m_processOnlyTopScene)
				break;
		}
		m_performanceTracker->end("Render");
	}
	void Application::present() {
		m_performanceTracker->begin("Present");
		m_renderer->presentImGui();
		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
			if (!(*it)->isActive())
				continue;
			(*it)->postRender();
			if (m_processOnlyTopScene)
				break;
		}

		m_renderer->present(m_window->getHandle());
		m_performanceTracker->end("Present");
	}

	void Application::endFrame() {
		m_window->endFrame();

//#ifdef _EDITOR
//		m_editorWindow->endFrame();
//#endif
		m_performanceTracker->endFrame();
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

	
	void Application::resizedWindow(HWND hWnd) {
		m_renderer->resizeRenderTarget(m_window->getClientSize());
		glm::vec2 size = m_window->getClientSize();
		m_renderer->resizeViewport("standard", size);
		DEVLOG("clientSize(" + std::to_string(size.x) + "," + std::to_string(size.y) + ")");
		
	}

	void Application::resizedWindow(const int x, const int y) {
		//DEVLOG("Window Resized(" + std::to_string(x) + "," + std::to_string(y) + ")");
		//m_renderer->resize({x, y});
	}

	void Application::renderImGuiDebug_toggle() {
		static bool imguiDemo = false;

		if (imguiDemo) {
			ImGui::ShowDemoWindow(&imguiDemo);
		}

		if (ImGui::Checkbox("ImGui Demo", &imguiDemo)) {
		}
		bool status = false;
		for (auto& pair : m_debugWindows) {
			auto* window = pair.second;
			status = window->isActive();
			if (ImGui::Checkbox(window->getTitle().c_str(), &status)) {
				window->setActive(status);
			}
		}
		if (ImGui::Button("Exit")) {
			exit();
		}
	}
	void Application::renderImGuiDebug_testing() {
		
		/*
		
		//auto* vp = ImGui::GetMainViewport();
		//vp->WorkPos;
		//ImGui::Text(to_string(vp->Pos));
		//ImGui::Text(to_string(vp->WorkPos));

		//const glm::vec3 transOrig = glm::vec3(0.02f, 0.95f, -0.03f);
		//static float pitch = 0.0f;
		//ImGui::SliderFloat("pitch", &pitch,0,5);

		//auto s1 = glm::translate(-transOrig);
		//auto s2 = glm::rotate(pitch, glm::vec3(1.f, 0.f, 0.f));
		//auto s3 = glm::translate(transOrig);

		//auto trans = s3 * s2 * s1;

		//ImGui::Text("s1");
		//scuff3dImGui::Mat4(s1);
		//ImGui::Text("s2");
		//scuff3dImGui::Mat4(s2);
		//ImGui::Text("s3");
		//scuff3dImGui::Mat4(s3);
		//ImGui::Text("trans");
		//scuff3dImGui::Mat4(trans);


		*/





	}
	void Application::renderImGuiDebug_renderer() {
	}
	
	void Application::renderImGuiDebug_settings() {
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
					size_t id = obj.first;
					if (ImGui::Selectable(obj.second->getName().c_str(), id == selectedObjectID, 0, ImVec2(120,0))) {
						if (selectedObjectID == id) {
							selectedObjectID = -1;
						}
						else {
							selectedObjectID = (int)id;
						}
					}
						
				}



				ImGui::EndGroupPanel();
			}
			ImGui::EndGroupPanel();

			if (selectedObjectID > -1) {
				const int line = 50;
				GameObject* obj = scene->m_gameObjects[selectedObjectID];
				ImGui::BeginGroupPanel(obj->getName() + " info", ImVec2(-1, 0));
				ImGui::Text("ID");
				ImGui::SameLine(line);
				ImGui::Text(std::to_string(selectedObjectID));
				//ImGui::InputInt("##OBJECTID", &selectedObjectID, 0, 0, ImGuiInputTextFlags_ReadOnly);

				ImGui::Text("Name");
				ImGui::SameLine(line);
				ImGui::Text(obj->getName());


				obj->renderImGui();




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
	void Application::renderImGuiDebug_resourceManager() {

		const ImGuiTabBarFlags tabBarFlags = 0;
		if(ImGui::BeginTabBar("ResourceManagerThings", tabBarFlags)) {
			if (ImGui::BeginTabItem("Models")) {
				ImGui::BeginGroupPanel("Models");
				static ResourceManager::ModelFile* selectedFile = nullptr;
				static std::string selectedName = "";
				for (auto& pair : m_resourceManager->getAllModelFiles()) {
					ResourceManager::ModelFile* file = pair.second;
					if (ImGui::Selectable(pair.first.c_str(), file == selectedFile, 0, ImVec2(100, 0))) {
						if (file == selectedFile) {
							selectedFile = nullptr;
							selectedName = "";
						}
						else {
							selectedFile = file;
							selectedName = pair.first;
						}
					}

				}
				ImGui::EndGroupPanel();
				ImGui::SameLine(); 
				ImGui::BeginGroupPanel(selectedFile ? selectedName : "Name"); {


					if (selectedFile) {
						Mesh* mesh = selectedFile->mesh;
						if (mesh) {
							ImGui::BeginGroupPanel("Mesh", ImVec2(200, 0)); {
								mesh->renderImGui();
							}
							ImGui::EndGroupPanel();
						}
					}
					else {
						ImGui::Text("No file selected");
					}
				}
				ImGui::EndGroupPanel();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Sounds")) {
				ImGui::BeginGroupPanel("Sounds");
				static std::string selectedIndex = "";
				for (auto& pair : m_resourceManager->getAllMeshes()) {
					Mesh* mesh = pair.second;
					if (ImGui::Selectable(pair.first.c_str(), selectedIndex == pair.first, 0, ImVec2(100, 0))) {
						if (selectedIndex == pair.first) {
							selectedIndex = "";
						}
						else {
							selectedIndex = pair.first;
						}
					}

				}

				ImGui::EndGroupPanel();
				ImGui::BeginGroupPanel("Sound", ImVec2(200, 0));



				ImGui::EndGroupPanel();
				
				ImGui::EndTabItem();
			}

		}
		ImGui::EndTabBar();


		
	}
	void Application::renderImGuiDebug_input()
	{
	}
	void Application::renderImGuiDebug_perf() { 
		
	}

//#ifdef _EDITOR
//	void Application::updateEditor(const float dt) {
//		m_editorCamera->getComponent<CameraController>()->update(dt);
//	}
//
//	void Application::updateEditorFixed(const float dt) { 
//
//	}
//
//	void Application::renderEditor() {
//		m_renderer->beginFrame(m_editorWindow->getHandle(),"editor", m_editorCamera->getComponent<Camera>());
//
//		for (auto it = m_sceneStack.rbegin(); it != m_sceneStack.rend(); ++it) {
//			if (!(*it)->isActive())
//				continue;
//			(*it)->render();
//			if (m_processOnlyTopScene)
//				break;
//		}
//
//
//
//		m_renderer->present(m_editorWindow->getHandle());
//	}
//#endif

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
				item.second->init();
				item.second->setActive(true);
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

	/*bool Application::editorWndProc(HWND hWnd, UINT message, WPARAM, LPARAM lParam) {

	}*/

	bool Application::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		
		ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
		m_window->wndProc(hWnd, message, wParam, lParam);

		switch (message) {

		case WM_DESTROY:

			exit();
			m_resizeMoveUpdateThread.join();
			break;
		case WM_SYSCOMMAND:
			if (wParam == SC_KEYMENU && (lParam >> 16) <= 0) return 0;
			break;
		case WM_NCPAINT:
			DEVLOG("NCPAINT")
				break;
		case WM_PAINT:
			DEVLOG("PAINT");
			break;
		case WM_ENTERSIZEMOVE:
			DEVLOG("WM_ENTERSIZEMOVE");
			m_runResizeMoveUpdateThread = true;
			break;
		case WM_EXITSIZEMOVE:
			DEVLOG("WM_EXITSIZEMOVE");
			m_runResizeMoveUpdateThread = false;
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