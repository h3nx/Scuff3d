#include "pch.h"
#include "RendererDX11.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

RendererDX11::RendererDX11(HWND hwnd) : Renderer::Renderer(hwnd) {
	init();
}

RendererDX11::~RendererDX11() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	clean();

}

void RendererDX11::init() {

	ImGui_ImplWin32_EnableDpiAwareness();
	UpdateWindow(m_hwnd);
	initDeviceAndSwapChain();
	m_renderTargetView = createRenderTargetView();


	IMGUI_CHECKVERSION(); 
	initImGui();
	//clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


}



void RendererDX11::beginFrame() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	glm::vec4 gvec4(0, 0, 0, 1);
	ImVec4 ivec4(0, 0, 0, 1);

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, (float*)&gvec4);

}

void RendererDX11::present() {
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	m_swapChain->Present(0, 0);

}


void RendererDX11::renderImGui(std::function<void()> imguiFunc) {
	imguiFunc();
}

void RendererDX11::initImGui() {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(m_hwnd);
	ImGui_ImplDX11_Init(m_device, m_deviceContext);
}

HRESULT RendererDX11::initDeviceAndSwapChain()
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_10_1, };
	

	
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		featureLevelArray,
		2,
		D3D11_SDK_VERSION,
		&sd,
		&m_swapChain,
		&m_device,
		&featureLevel,
		&m_deviceContext
	);
	DEVLOG(std::to_string(hr));
	if (!SUCCEEDED(hr))
		return hr;

	return hr;
}

ID3D11RenderTargetView* RendererDX11::createRenderTargetView()
{
	ID3D11RenderTargetView* rtv;
	ID3D11Texture2D* pBackBuffer;
	m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	m_device->CreateRenderTargetView(pBackBuffer, NULL, &rtv);
	pBackBuffer->Release();
	return rtv;
}

void RendererDX11::clean()
{
	cleanupDX11Object(m_renderTargetView);
	cleanupDX11Object(m_rasterizerState);
	cleanupDX11Object(m_swapChain);
	cleanupDX11Object(m_deviceContext);
	cleanupDX11Object(m_device);
}

template<typename T>
inline void RendererDX11::cleanupDX11Object(T* ptr) {
	if (ptr) {
		ptr->Release();
		ptr = nullptr;
	}
}

//void RendererDX11::cleanupRenderTargetView(std::unique_ptr<ID3D11RenderTargetView>& u_ptr)
//{
//	if (u_ptr.get() != nullptr) {
//		u_ptr->Release();
//		u_ptr.reset(nullptr);
//	}
//}
