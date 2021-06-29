#pragma once

#include "../API/Renderer.h"
#include <d3d12.h>
#include <d3dcompiler.h>
#include "imgui/imgui_impl_dx12.h"
#include <memory>

#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "d3dcompiler.lib")


class RendererDX12 : public Renderer {
public:
	RendererDX12(HWND hwnd);
	~RendererDX12();

	void init();

	void beginFrame();
	void present();
	void endFrame();

	void render(void);




private:
	void initImGui();

};

