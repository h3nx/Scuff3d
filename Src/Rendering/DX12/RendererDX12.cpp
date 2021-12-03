#include "pch.h"
#include "RendererDX12.h"
namespace scuff3d {

	RendererDX12::RendererDX12(HWND hwnd, const glm::vec2& initialSize) : Renderer::Renderer(initialSize) {
		initImGui(hwnd);
	}

	RendererDX12::~RendererDX12() {
	}

	void RendererDX12::init() {
	}

	bool RendererDX12::beginFrame(HWND hWnd, const std::string& viewport, CameraData* camera) {
		return true;
	}

	void RendererDX12::present(HWND hWnd) {
	}

	void RendererDX12::renderImGui(std::function<void()> imguiFunc)
	{
	}


	void RendererDX12::initImGui(HWND hWnd) {
		ImGui::CreateContext();
		//ImGuiIO& io = ImGui::GetIO(); (void)io;
		//ImGui::StyleColorsDark();
		//ImGui_ImplWin32_Init(m_handle);
		//ImGui_ImplDX11_Init(m_device, m_deviceContext);
	}

}