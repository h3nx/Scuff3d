#include "Window32.h"
#include "Input\Input.h"

namespace scuff3d {


	class Scuff3dWindow32 : public Window32, public Input {
	public:
		// windowed
		Scuff3dWindow32(HINSTANCE hInstance, WNDPROC proc, const std::string& name = "Scuff3d Window 32", const glm::vec2& size = glm::vec2(1280, 720), const glm::vec2& pos = glm::vec2(0, 0), const bool maximised = false);
		// fullscreen
		Scuff3dWindow32(HINSTANCE hInstance, WNDPROC proc, const std::string& name, const int monitor = 0);
		~Scuff3dWindow32();


		bool wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		void newFrame();
		void endFrame();


		void onActivate(const std::string& identifier, std::function<void()> func);
		void onDeactivate(const std::string& identifier, std::function<void()> func);
		void onStatusChange(const std::string& identifier, std::function<void(const bool state)> func);

		void onResizeStart(const std::string& identifier, std::function<void()> func);
		void onResizeEnd(const std::string& identifier, std::function<void()> func);
		void onClientResize(const std::string& identifier, std::function<void(const glm::vec2& size)> func);

		void onMoveStart(const std::string& identifier, std::function<void()> func);
		void onMoveEnd(const std::string& identifier, std::function<void()> func);
		void onMove(const std::string& identifier, std::function<void(const glm::vec2& pos)> func);





		void renderImGui();
	private:


		std::map<std::string, std::function<void()>> m_onActivate;
		std::map<std::string, std::function<void()>> m_onDeactivate;
		std::map<std::string, std::function<void(const bool state)>> m_onStatusChange;

		bool m_sizing;
		bool m_sizingPrevious;
		std::map<std::string, std::function<void()>> m_onResizeStart;
		std::map<std::string, std::function<void()>> m_onResizeEnd;
		std::map<std::string, std::function<void(const glm::vec2& size)>> m_onClientResize;

		bool m_moving;
		bool m_movingPrevious;
		std::map<std::string, std::function<void()>> m_onMoveStart;
		std::map<std::string, std::function<void()>> m_onMoveEnd;
		std::map<std::string, std::function<void(const glm::vec2& pos)>> m_onMove;





	};


}