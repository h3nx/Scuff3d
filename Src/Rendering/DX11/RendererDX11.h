#pragma once
#include "../API/Renderer.h"
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")


class RendererDX11 : public Renderer {
public:
	RendererDX11(HWND hwnd);
	~RendererDX11();

	void init();

	void beginFrame();
	void present();
	void endFrame();

	void render(std::function<void()> imguiFunc);




private:
	HRESULT initContext();
	void initImGui();

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11RasterizerState* m_rasterizerState;
};

