#pragma once
#include "../API/Renderer.h"
#include <d3d11.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>
#include "glm/glm.hpp"
#include <d3d11shadertracing.h>


#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

namespace scuff3d
{
	class RendererDX11 : public Renderer {
	public:
		RendererDX11(HWND renderTarget, const glm::vec2& initialSize);
		~RendererDX11();

		void init();

		void beginImGui();
		void presentImGui();
		bool beginFrame(HWND hWnd, const std::string& viewport, CameraData* camera = nullptr);
		void present(HWND hWnd);

		void renderSolid(ModelData* model, const glm::mat4& matrix);
		void renderWireFrame(ModelData* model, const glm::mat4& matrix);

		//void resize(const std::string& name, const glm::vec2& size);
		void resizeViewport(const std::string& name, const glm::vec2& size);
		void renderImGui(std::function<void()> imguiFunc);

		Shader* loadShader(const std::string& name, const std::string& path, const SHADER_TYPE type, const std::string& entryPoint = "main", const std::string& target = "5_0");

		ModelData* createModel(const std::string& name, Mesh* data);
		ModelData* createModel(
			const std::string& name,
			Mesh* data,
			Shader* vertex,
			Shader* pixel,
			Shader* geometry,
			Shader* hull,
			Shader* domain
		);
		ModelData* createModel(
			const std::string& name,
			Mesh* data,
			const std::string& vertex,
			const std::string& pixel,
			const std::string& geometry = "",
			const std::string& hull = "",
			const std::string& domain = ""
		);

		void renderImGuiDebug();
		void setViewPort(const std::string& name, const glm::vec2& pos, const glm::vec2& size);


	private:
		void initImGui(HWND hWnd);

		void renderModel(ModelData* model, const glm::mat4& matrix, ID3D11RasterizerState* state);

		HRESULT initDevice();
		IDXGISwapChain1* createSwapChain(HWND hWnd);
		
		HRESULT resizeSwapChain(const glm::vec2& size);
		ID3D11RenderTargetView* createRenderTargetView(IDXGISwapChain1* swapChain);
		ID3D11RenderTargetView* createRenderTargetView(ID3D11Texture2D* buffer);

		ID3D11RasterizerState* createRasterizerState(const D3D11_FILL_MODE fillmode);
		ID3D11RasterizerState* createRasterizerState(const D3D11_RASTERIZER_DESC& desc);

		ID3D11Texture2D* createTexture(const glm::vec2& size, DXGI_FORMAT format, D3D11_USAGE usage, UINT flags);
		ID3D11Texture2D* createTexture(const D3D11_TEXTURE2D_DESC desc);

		ID3D11DepthStencilView* createDepthStencilView(ID3D11Texture2D* buffer);
		//ID3D11Buffer* 

		ID3D11DeviceChild* createShader(const std::string fileName, D3D11_SHADER_TYPE type, const std::string& entryPoint, const std::string target);

		ID3D11VertexShader* createVertexShader(const std::string fileName, const std::string& entryPoint, const std::string target);
		ID3D11PixelShader* createPixelShader(const std::string fileName, const std::string& entryPoint, const std::string target);
		ID3D11GeometryShader* createGeometryShader(const std::string fileName, const std::string& entryPoint, const std::string target);
		ID3D11ComputeShader* createComputeShader(const std::string fileName, const std::string& entryPoint, const std::string target);
		ID3D11HullShader* createHullShader(const std::string fileName, const std::string& entryPoint, const std::string target);
		ID3D11DomainShader* createDomainShader(const std::string fileName, const std::string& entryPoint, const std::string target);

		ID3D11InputLayout* createInputLayout(ID3D11DeviceChild* shader, const MeshDataFlags flags);
		ID3D11InputLayout* createInputLayout(ID3D11DeviceChild* shader, std::vector<D3D11_INPUT_ELEMENT_DESC>& desc);

		template<typename T>
		ID3D11Buffer* createBuffer(const T& data, D3D11_USAGE usage, UINT bindflags, UINT cpuAccessFlags);
		template<typename T>
		bool setBufferData(ID3D11Buffer* buffer, const T& data);

		void resizeHwnd(const glm::vec2& size);
		void resizeDepthsStencil(const std::string& name, const glm::vec2& size);

		void clean();

		ID3D11Device* m_device;
		ID3D11Debug* m_debug;
		ID3D11DeviceContext* m_deviceContext;
		std::map<HWND, IDXGISwapChain1*> m_swapChains;
		std::map<IDXGISwapChain1*, ID3D11RenderTargetView*> m_renderTargetViews;
		glm::vec2 m_size;
		std::map<std::string, ID3D11RasterizerState*> m_rasterizerStates;
		std::map<std::string, ID3D11Buffer*> m_constantBuffers;
		std::map<std::string, D3D11_VIEWPORT> m_viewports;
		std::map<std::string, ID3D11DeviceChild*> m_shaderData;
		std::map<ID3D11DeviceChild*, ID3D11InputLayout*> m_inputLayouts;
		std::map<ID3D11DeviceChild*, ID3DBlob*> m_shaderBlobs;
		std::map<std::string, ID3D11SamplerState*> m_samplerStates;
		std::map<std::string, ID3D11DepthStencilView*> m_depthStencilViews;
		std::map<std::string, ID3D11ShaderResourceView*> m_shaderResourceViews;
		std::map<std::string, ID3D11Texture2D*> m_textures;
		
		void temp(const float& te) {};

		struct ObjectBuffer {
			glm::mat4 world;
			ObjectBuffer(const glm::mat4& _world) {
				world = _world;
			}
			/*ObjectBuffer(glm::mat4 _world, glm::mat4 _view, glm::mat4 _projection) {
				world = _world;
				view = _view;
				projection = _projection;
			}*/
		};
		struct FrameBuffer {
			glm::mat4 view;
			glm::mat4 projection;
			FrameBuffer(const glm::mat4& _view, const glm::mat4& _projection) {
				view = _view;
				projection = _projection;
			}

			//glm::vec4 cameraPosition;
			//glm::vec4 cameraDirection;
		};


	};


}