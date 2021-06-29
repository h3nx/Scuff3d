#include "pch.h"
#include "RendererDX11.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

RendererDX11::RendererDX11(HWND hwnd) : Renderer::Renderer(hwnd) {
	initContext();
	initImGui();
}

RendererDX11::~RendererDX11() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
}

void RendererDX11::init() {
}

void RendererDX11::beginFrame() {
}

void RendererDX11::present() {
}

void RendererDX11::endFrame() {
}

void RendererDX11::render(std::function<void()> imguiFunc) {
}

HRESULT RendererDX11::initContext() {
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;                                    
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      
	scd.OutputWindow = m_handle;								
	scd.SampleDesc.Count = 1;                           
	scd.Windowed = TRUE;


	// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL,
		NULL, D3D11_SDK_VERSION, &scd, &m_swapChain, &m_device, NULL, &m_deviceContext);

	if (!SUCCEEDED(hr))
		return hr;

	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	if (!SUCCEEDED(hr))
		return hr;


	// use the back buffer address to create the render target
	hr = m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_renderTargetView);
	pBackBuffer->Release();


	return hr;
}

void RendererDX11::initImGui() {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(m_handle);
	ImGui_ImplDX11_Init(m_device, m_deviceContext);
}
