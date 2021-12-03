#pragma once

#include "../API/Renderer.h"
#include <d3d12.h>
#include <d3dcompiler.h>
#include "imgui/imgui_impl_dx12.h"
#include <memory>

#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "d3dcompiler.lib")
namespace scuff3d {


	class RendererDX12 : public Renderer {
	public:
		RendererDX12(HWND hwnd, const glm::vec2& initialSize);
		~RendererDX12();

		void init();

		bool beginFrame(HWND hWnd, const std::string& viewport, CameraData* camera = nullptr);
		void present(HWND hWnd);
		//void renderObject(GameObject* object) {};

		void renderImGui(std::function<void()> imguiFunc);

		void resize(HWND hwnd, const glm::vec2& size) {};
		//void resize(const std::string& name, const glm::vec2& size) {};


	private:
		void initImGui(HWND hWnd);

	};


}