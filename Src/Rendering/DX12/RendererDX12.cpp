#include "pch.h"
#include "RendererDX12.h"

RendererDX12::RendererDX12(HWND hwnd) : Renderer::Renderer(hwnd) {
	initImGui();
}

RendererDX12::~RendererDX12() {
}

void RendererDX12::init() {
}

void RendererDX12::beginFrame() {
}

void RendererDX12::present() {
}

void RendererDX12::renderImGui(std::function<void()> imguiFunc)
{
}


void RendererDX12::initImGui() {
	ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//ImGui::StyleColorsDark();
	//ImGui_ImplWin32_Init(m_handle);
	//ImGui_ImplDX11_Init(m_device, m_deviceContext);
}
