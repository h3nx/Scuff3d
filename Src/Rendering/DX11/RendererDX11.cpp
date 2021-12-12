#include "pch.h"
#include "RendererDX11.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Rendering/API/Geometry/Mesh.h"
#include "Rendering/DX11/Model/ModelDX11.h"
#include "Rendering/DX11/Shaders/ShadersDX11.h"


namespace scuff3d
{

	RendererDX11::RendererDX11(HWND renderTarget, const glm::vec2& initialSize) : Renderer::Renderer(initialSize) {
		m_renderTarget = renderTarget;
		m_size = initialSize;
		init();

	}
	void RendererDX11::init() {
		ImGui_ImplWin32_EnableDpiAwareness();
		UpdateWindow(m_renderTarget);

		initDevice();
		m_swapChains[m_renderTarget] = createSwapChain(m_renderTarget);
		m_renderTargetViews[m_swapChains[m_renderTarget]] = createRenderTargetView(m_swapChains[m_renderTarget]);

		m_rasterizerStates["solid"] = createRasterizerState(D3D11_FILL_SOLID);
		m_rasterizerStates["wireframe"] = createRasterizerState(D3D11_FILL_WIREFRAME);

		m_textures["_depthStencil"] = createTexture(m_size, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_USAGE_DEFAULT, D3D11_BIND_DEPTH_STENCIL);
		m_depthStencilViews["_depthStencil"] = createDepthStencilView(m_textures["_depthStencil"]);


		//std::string path = "../Scuff3d/Resources/DefaultShaders/";
		//for (const auto& entry : std::filesystem::directory_iterator(path)) {

		//	DEVLOG(entry.path().filename().string());
		//}


	/*	m_shaders["Vertex Default"] = createVertexShader("../Scuff3d/Resources/DefaultShaders/VertexColor.hlsl", "main", "vs_5_0");
		m_shaders["Pixel Default Lit"] = createPixelShader("../Scuff3d/Resources/DefaultShaders/PixelColor.hlsl", "main", "ps_5_0");

		m_inputLayouts[m_shaders["Vertex Default"]] = createInputLayout(m_shaders["Vertex Default"], MeshDataFlags_Position | MeshDataFlags_Normal | MeshDataFlags_Color);*/


		FrameBuffer buffer(glm::identity<glm::mat4>(),
			glm::identity<glm::mat4>());
		
		m_constantBuffers["FrameData"] = createBuffer(
			FrameBuffer(glm::identity<glm::mat4>(),
				glm::identity<glm::mat4>()),
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_CPU_ACCESS_WRITE
		);
		m_constantBuffers["ObjectData"] = createBuffer(
			ObjectBuffer(glm::identity<glm::mat4>()),
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_CPU_ACCESS_WRITE
		);



		//int size = Mesh::vertexByteSize(MeshDataFlags_Position);
		//DEVLOG("size("+std::to_string(size)+")");

		IMGUI_CHECKVERSION();
		initImGui(m_renderTarget);

		//clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	}

	void RendererDX11::beginImGui() { 
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void RendererDX11::presentImGui() {
		ImGui::Render();
		// Update and Render additional Platform Windows
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	RendererDX11::~RendererDX11() {
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		DEVLOG("CLEANING RENDERER");
		clean();
		safeRelease(m_device);
		m_debug->ReportLiveDeviceObjects(D3D11_RLDO_FLAGS::D3D11_RLDO_DETAIL);
		m_debug->Release();
	}
	void RendererDX11::clean() {
		safeReleaseMap(m_renderTargetViews);
		safeReleaseMap(m_rasterizerStates);
		safeReleaseMap(m_constantBuffers);
		safeReleaseMap(m_shaderData);
		safeReleaseMap(m_inputLayouts);
		safeReleaseMap(m_shaderBlobs);
		safeReleaseMap(m_samplerStates);
		safeReleaseMap(m_depthStencilViews);
		safeReleaseMap(m_shaderResourceViews);
		safeReleaseMap(m_textures);
		safeReleaseMap(m_swapChains);
		safeDeleteMap(m_models);
		safeRelease(m_deviceContext);
	}


	// RENDERING STUFF

	bool RendererDX11::beginFrame(HWND hWnd, const std::string& viewport, CameraData* camera) {

		
		/*if (i > 0)
			return false;*/

		glm::vec4 gvec4(0, 0, 0, 1);
		if (!&m_renderTargetViews[m_swapChains[hWnd]]) {
			return false;
		}

		auto* rtv = m_renderTargetViews[m_swapChains[hWnd]];
		auto* dsv = m_depthStencilViews["_depthStencil"];
		m_deviceContext->OMSetRenderTargets(1, &rtv, dsv);
		m_deviceContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		m_deviceContext->ClearRenderTargetView(rtv, (float*)&gvec4);
		m_deviceContext->RSSetViewports(1, &m_viewports[viewport]);
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		CameraData* cam = camera ? camera : m_mainCamera;
		setBufferData(
			m_constantBuffers["FrameData"], 
			FrameBuffer(
				cam->getViewMatrix(),
				cam->getProjectionMatrix()
			)
		);

		m_deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffers["FrameData"]);
		return true;
	}
	
	void RendererDX11::renderSolid(ModelData* model, const glm::mat4& matrix) {
		renderModel(model, matrix, m_rasterizerStates["solid"]);
		
	}
	void RendererDX11::renderWireFrame(ModelData* model, const glm::mat4& matrix) {
		renderModel(model, matrix, m_rasterizerStates["wireframe"]);
	}


	void RendererDX11::present(HWND hWnd) {

		m_swapChains[hWnd]->Present(0, DXGI_PRESENT_DO_NOT_WAIT);
		if (!m_toResize.empty()) {
			resizeHwnd(m_toResize.back());
			while (!m_toResize.empty()) {
				//DEVLOG(std::string(std::to_string(i) + "resized " + to_string(m_toResize.front().second)).c_str());
				m_toResize.pop();
			}
		}
		

	}


	// BACKEND CREATION STUFF



	void RendererDX11::resizeHwnd(const glm::vec2& size) {
		if (m_size == size) return;
		m_size = size;
		resizeSwapChain(size);
		resizeDepthsStencil("_depthsStencil", size);
	}
	void RendererDX11::resizeDepthsStencil(const std::string& name, const glm::vec2& size) {
		//remove old
		safeRelease(m_depthStencilViews["_depthStencil"]);
		safeRelease(m_textures["_depthStencil"]);
		//create new 
		m_textures["_depthStencil"] = createTexture(size, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_USAGE_DEFAULT, D3D11_BIND_DEPTH_STENCIL);
		m_depthStencilViews["_depthStencil"] = createDepthStencilView(m_textures["_depthStencil"]);
	}
	void RendererDX11::resizeViewport(const std::string& name, const glm::vec2& size) { 
		m_viewports[name].Width = size.x;
		m_viewports[name].Height = size.y;
	}

	void RendererDX11::renderImGui(std::function<void()> imguiFunc) {
		imguiFunc();
	}

	Shader* RendererDX11::loadShader(const std::string& name, const std::string& path, const SHADER_TYPE type, const std::string& entryPoint, const std::string& target) {
		ID3D11DeviceChild* temp = createShader(path, (D3D11_SHADER_TYPE)type, entryPoint, targetString(type) + target);
		if (!temp)
			return nullptr;

		m_shaderData[name] = temp;



		Shader* shader = nullptr;
		switch (type) {
		case SHADER_TYPE::VERTEX_SHADER:
			shader = NEW VertexShaderDX11(temp);
			m_vertexShaders.push_back(name);
			break;
		case SHADER_TYPE::GEOMETRY_SHADER:
			shader = NEW Shader();
			m_geometryShaders.push_back(name);
			break;
		case SHADER_TYPE::PIXEL_SHADER:
			shader = NEW PixelShaderDX11(temp);
			m_pixelShaders.push_back(name);
			break;
		case SHADER_TYPE::HULL_SHADER:
			shader = NEW Shader();
			m_hullShaders.push_back(name);
			break;
		case SHADER_TYPE::DOMAIN_SHADER:
			shader = NEW Shader();
			m_domainShaders.push_back(name);
			break;
		case SHADER_TYPE::COMPUTE_SHADER:
			shader = NEW Shader();
			m_computeShaders.push_back(name);
			break;
		}
		m_shaders[name] = shader;
		return shader;
	}

	ModelData* RendererDX11::createModel(const std::string& name, Mesh* data) {
		static size_t uuid = 0;
		m_modelNames[name] = uuid;
		return m_models[uuid++] = NEW ModelDX11(data, m_device);
	}
	ModelData* RendererDX11::createModel( const std::string& name, Mesh* data, Shader* vertex, Shader* pixel, Shader* geometry, Shader* hull, Shader* domain) {
		ModelData* model = createModel(name, data);
		model->setVertexShader(vertex);
		if (vertex) {
			ID3D11DeviceChild* vShader = ((ShaderDX11*)vertex)->getData();
			if (m_inputLayouts.find(vShader) == m_inputLayouts.end())
				m_inputLayouts[vShader] = createInputLayout(vShader, data->getFlags());
		}
		model->setGeometryShader(geometry);
		model->setHullShader(hull);
		model->setDomainShader(domain);
		model->setPixelShader(pixel);
		return model;
	}
	ModelData* RendererDX11::createModel(const std::string& name, Mesh* data, const std::string& vertex, const std::string& pixel, const std::string& geometry, const std::string& hull, const std::string& domain) {
		return createModel(
			name,
			data,
			vertex == "" ? nullptr : getShader(vertex),
			pixel == "" ? nullptr : getShader(pixel),
			geometry == "" ? nullptr : getShader(geometry),
			hull == "" ? nullptr : getShader(hull),
			domain == "" ? nullptr : getShader(domain)
		);
	}

	void RendererDX11::renderImGuiDebug() {
		ImGui::BeginGroupPanel("Viewports");
		{
			for (auto& pair : m_viewports) {
				ImGui::BeginGroupPanel(pair.first);
				ImGui::Text("Position");
				ImGui::SameLine();
				glm::ivec2 p = { pair.second.TopLeftX, pair.second.TopLeftY };
				if (ImGui::DragInt2("##pos", &p.x)) {
					pair.second.TopLeftX = p.x;
					pair.second.TopLeftY = p.y;
				}
				ImGui::Text("Size");
				ImGui::SameLine();
				glm::ivec2 s = { pair.second.Width, pair.second.Height};
				if (ImGui::DragInt2("##size", &s.x)) {
					pair.second.Width = s.x;
					pair.second.Height = s.y;
				}


				ImGui::EndGroupPanel();
			}

			
		} ImGui::EndGroupPanel();
	}

	void RendererDX11::setViewPort(const std::string& name, const glm::vec2& pos, const glm::vec2& size) { 
		m_viewports[name] = { pos.x, pos.y, size.x, size.y, 0.0f, 0.1f };
	}

	

	void RendererDX11::initImGui(HWND hWnd) {
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ImGui::StyleColorsDark();
		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(m_device, m_deviceContext);
	}

	void RendererDX11::renderModel(ModelData* model, const glm::mat4& matrix, ID3D11RasterizerState* state) { 
		ModelDX11* modeldx11 = (ModelDX11*)model;
		setBufferData(m_constantBuffers["ObjectData"], ObjectBuffer(matrix));
		const Mesh* mesh = model->getMesh();
		ShaderDX11* vShader = (ShaderDX11*)modeldx11->getVertexShader();
		ShaderDX11* pShader = (ShaderDX11*)modeldx11->getPixelShader();
		ShaderDX11* gShader = (ShaderDX11*)modeldx11->getGeometryShader();
		ShaderDX11* hShader = (ShaderDX11*)modeldx11->getHullShader();
		ShaderDX11* dShader = (ShaderDX11*)modeldx11->getDomainShader();

		ID3D11Buffer* b = modeldx11->getVertBuffer();
		UINT stride = (UINT)modeldx11->getStride();
		UINT offset = (UINT)modeldx11->getOffset();
		m_deviceContext->IASetVertexBuffers(0, 1, &b, &stride, &offset);
		m_deviceContext->RSSetState(state);
		bool index = mesh->getIndicesCount() > 0;

		if (index) {
			ID3D11Buffer* ib = modeldx11->getIndBuffer();

			m_deviceContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, offset);
		}
		m_deviceContext->VSSetConstantBuffers(1, 1, &m_constantBuffers["ObjectData"]);

		m_deviceContext->IASetInputLayout(m_inputLayouts[vShader->getData()]);
		if (vShader) {
			m_deviceContext->VSSetShader((ID3D11VertexShader*)vShader->getData(), nullptr, 0);
		}
		if (dShader) {
			m_deviceContext->DSSetShader((ID3D11DomainShader*)dShader->getData(), nullptr, 0);
		}
		if (hShader) {
			m_deviceContext->HSSetShader((ID3D11HullShader*)hShader->getData(), nullptr, 0);
		}
		if (gShader) {
			m_deviceContext->GSSetShader((ID3D11GeometryShader*)gShader->getData(), nullptr, 0);
		}
		if (pShader) {
			m_deviceContext->PSSetShader((ID3D11PixelShader*)pShader->getData(), nullptr, 0);
		}

		if (index)
			m_deviceContext->DrawIndexed(mesh->getIndicesCount(), 0, 0);
		else
			m_deviceContext->Draw(mesh->getCount(), 0);
	
	
	}

	HRESULT RendererDX11::initDevice()
	{
	

		UINT createDeviceFlags = 0;
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_10_1, };
		ID3D11Device* temp;

		HRESULT hr = D3D11CreateDevice(NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,createDeviceFlags,
			featureLevelArray,
			2,
			D3D11_SDK_VERSION,
			&m_device,
			&featureLevel,
			&m_deviceContext
		);
		


		/*HRESULT hr = D3D11CreateDeviceAndSwapChain(
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
		);*/

		DEVLOG("CreateDevice(" + std::to_string(hr) + ")");
		if (!SUCCEEDED(hr))
			return hr;

		m_device->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_debug);




		return hr;
	}

	IDXGISwapChain1* RendererDX11::createSwapChain(HWND hWnd) {
		RECT rect;
		GetClientRect(hWnd, &rect);
		
		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.Width = rect.right;
		sd.Height = rect.bottom;
		//sd.BufferDesc.Width = m_size.x;
		//sd.BufferDesc.Height = m_size.y;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//sd.BufferDesc.RefreshRate.Numerator = 60;
		//sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		//sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		//sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		IDXGIDevice2* pDXGIDevice;
		HRESULT hr = m_device->QueryInterface(__uuidof(IDXGIDevice2), (void**)&pDXGIDevice);
		IDXGIAdapter* pDXGIAdapter;
		hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDXGIAdapter);
		IDXGIFactory2* pIDXGIFactory;
		pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&pIDXGIFactory);
		IDXGISwapChain1* temp;
		hr = pIDXGIFactory->CreateSwapChainForHwnd(m_device, hWnd, &sd, NULL, NULL, &temp);

		pIDXGIFactory->Release();
		pDXGIAdapter->Release();
		pDXGIDevice->Release();

		return temp;
	}

	HRESULT RendererDX11::resizeSwapChain(const glm::vec2& size) {
		HRESULT hr;
		IDXGISwapChain1* chain = m_swapChains[m_renderTarget];
		ID3D11RenderTargetView* rtv = m_renderTargetViews[chain];
		m_deviceContext->OMSetRenderTargets(0, 0, 0);
		rtv->Release();
		hr = chain->ResizeBuffers(0, size.x, size.y, DXGI_FORMAT_UNKNOWN, 0);

		if (FAILED(hr))
			DEVLOG("swapchain resize buffer failed");

		m_renderTargetViews[chain] = createRenderTargetView(chain);
		if (m_renderTargetViews[chain])
			return S_OK;

		return S_FALSE;
	}


	ID3D11RenderTargetView* RendererDX11::createRenderTargetView(IDXGISwapChain1* swapChain) {
		ID3D11Texture2D* pBackBuffer;
		ID3D11RenderTargetView* temp;
		swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		temp = createRenderTargetView(pBackBuffer);
		pBackBuffer->Release();
		return temp;
	}
	ID3D11RenderTargetView* RendererDX11::createRenderTargetView(ID3D11Texture2D* buffer) {

		ID3D11RenderTargetView* temp;
		HRESULT hr = m_device->CreateRenderTargetView(buffer, NULL, &temp);
		if (FAILED(hr)) {
			
			return nullptr;
		}
		return temp;
	}

	ID3D11RasterizerState* RendererDX11::createRasterizerState(const D3D11_FILL_MODE fillmode) {
		// Setup rasterizer state.
		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

		rasterizerDesc.AntialiasedLineEnable = FALSE;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.FillMode = fillmode;
		rasterizerDesc.FrontCounterClockwise = FALSE;
		rasterizerDesc.MultisampleEnable = FALSE;
		rasterizerDesc.ScissorEnable = FALSE;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;

		
		return createRasterizerState(rasterizerDesc);
	}

	ID3D11RasterizerState* RendererDX11::createRasterizerState(const D3D11_RASTERIZER_DESC& desc) {
		ID3D11RasterizerState* temp;
		// Create the rasterizer state object.
		HRESULT hr = m_device->CreateRasterizerState(&desc, &temp);

		if (FAILED(hr)) {
			DEVLOG("createRasterizerState failed");
			return nullptr;
		}
		return temp;
	}

	ID3D11Texture2D* RendererDX11::createTexture(const D3D11_TEXTURE2D_DESC desc) {
		ID3D11Texture2D* temp;
		HRESULT hr = m_device->CreateTexture2D(&desc, nullptr, &temp);
		if (FAILED(hr)) {
			DEVLOG("texture creation failed");
			return nullptr;
		}
		return temp;
	}
	ID3D11Texture2D* RendererDX11::createTexture(const glm::vec2& size, DXGI_FORMAT format, D3D11_USAGE usage, UINT flags) {

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = (float)size.x;
		desc.Height = (float)size.y;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = format;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = usage;
		desc.BindFlags = flags;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		
		return createTexture(desc);
	}

	ID3D11DepthStencilView* RendererDX11::createDepthStencilView(ID3D11Texture2D* buffer) {
		ID3D11DepthStencilView* temp = nullptr;
		HRESULT hr = m_device->CreateDepthStencilView(buffer, nullptr, &temp);
		if (FAILED(hr)) {
			DEVLOG("CreateDepthStencilView failed");

			return nullptr;
		}
		return temp;
	}

	ID3D11DeviceChild* RendererDX11::createShader(const std::string fileName, D3D11_SHADER_TYPE type, const std::string& entryPoint, const std::string target) {
		ID3DBlob* compiledShaderBlob = nullptr;
		ID3DBlob* error = nullptr;
		HRESULT hr;
		hr = D3DCompileFromFile(
			stringToWstring(fileName).c_str(),
			nullptr,
			nullptr,
			entryPoint.c_str(),
			target.c_str(),
			0,
			0,
			&compiledShaderBlob,
			&error
		);
		if (FAILED(hr))
			return nullptr;

		ID3D11DeviceChild* temp = nullptr;
		switch (type) {
		case D3D11_VERTEX_SHADER:
			hr = m_device->CreateVertexShader(compiledShaderBlob->GetBufferPointer(), compiledShaderBlob->GetBufferSize(), nullptr, (ID3D11VertexShader**)&temp);
			break;
		case D3D11_PIXEL_SHADER:
			hr = m_device->CreatePixelShader(compiledShaderBlob->GetBufferPointer(), compiledShaderBlob->GetBufferSize(), nullptr, (ID3D11PixelShader**)&temp);
			break;
		case D3D11_GEOMETRY_SHADER:
			hr = m_device->CreateGeometryShader(compiledShaderBlob->GetBufferPointer(), compiledShaderBlob->GetBufferSize(), nullptr, (ID3D11GeometryShader**)&temp);
			break;
		case D3D11_COMPUTE_SHADER:
			hr = m_device->CreateComputeShader(compiledShaderBlob->GetBufferPointer(), compiledShaderBlob->GetBufferSize(), nullptr, (ID3D11ComputeShader**)&temp);
			break;
		case D3D11_HULL_SHADER:
			hr = m_device->CreateHullShader(compiledShaderBlob->GetBufferPointer(), compiledShaderBlob->GetBufferSize(), nullptr, (ID3D11HullShader**)&temp);
			break;
		case D3D11_DOMAIN_SHADER:
			hr = m_device->CreateDomainShader(compiledShaderBlob->GetBufferPointer(), compiledShaderBlob->GetBufferSize(), nullptr, (ID3D11DomainShader**)&temp);
			break;
		}
		
		if (FAILED(hr)) {
			DEVLOG("SHADERCOMPILE FAILED("+fileName+","+entryPoint+","+target+")");
			return nullptr;
		}
		m_shaderBlobs[temp] = compiledShaderBlob;
		//DEVLOG(std::to_string(compiledShaderBlob->GetBufferSize()));
		return temp;
	}

	ID3D11VertexShader* RendererDX11::createVertexShader(const std::string fileName, const std::string& entryPoint, const std::string target) {
		return (ID3D11VertexShader*)createShader(fileName, D3D11_SHADER_TYPE::D3D11_VERTEX_SHADER, entryPoint, target);
	}
	ID3D11PixelShader* RendererDX11::createPixelShader(const std::string fileName, const std::string& entryPoint, const std::string target) {
		return (ID3D11PixelShader*)createShader(fileName, D3D11_SHADER_TYPE::D3D11_PIXEL_SHADER, entryPoint, target);
	}
	ID3D11GeometryShader* RendererDX11::createGeometryShader(const std::string fileName, const std::string& entryPoint, const std::string target) {
		return (ID3D11GeometryShader*)createShader(fileName, D3D11_SHADER_TYPE::D3D11_GEOMETRY_SHADER, entryPoint, target);
	}
	ID3D11ComputeShader* RendererDX11::createComputeShader(const std::string fileName, const std::string& entryPoint, const std::string target) {
		return (ID3D11ComputeShader*)createShader(fileName, D3D11_SHADER_TYPE::D3D11_COMPUTE_SHADER, entryPoint, target);;
	}
	ID3D11HullShader* RendererDX11::createHullShader(const std::string fileName, const std::string& entryPoint, const std::string target) {
		return (ID3D11HullShader*)createShader(fileName, D3D11_SHADER_TYPE::D3D11_HULL_SHADER, entryPoint, target);
	}
	ID3D11DomainShader* RendererDX11::createDomainShader(const std::string fileName, const std::string& entryPoint, const std::string target) {
		return (ID3D11DomainShader*)createShader(fileName, D3D11_SHADER_TYPE::D3D11_DOMAIN_SHADER, entryPoint, target);
	}


	ID3D11InputLayout* RendererDX11::createInputLayout(ID3D11DeviceChild* shader, const MeshDataFlags flags) {

		std::vector< D3D11_INPUT_ELEMENT_DESC> descs;

		D3D11_INPUT_ELEMENT_DESC tempdesc;
		if (flags & MeshDataFlags_Position) 
			descs.push_back({"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (flags & MeshDataFlags_Normal) 
			descs.push_back({"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0});
		if (flags & MeshDataFlags_Tangent)
			descs.push_back({"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0});
		if (flags & MeshDataFlags_BiTangent)
			descs.push_back({"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0});
		if (flags & MeshDataFlags_UV)
			descs.push_back({"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0});
		if (flags & MeshDataFlags_Color)
			descs.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		return createInputLayout(shader, descs);
	}
	ID3D11InputLayout* RendererDX11::createInputLayout(ID3D11DeviceChild* shader, std::vector<D3D11_INPUT_ELEMENT_DESC>& desc) {
	
		
		ID3D11InputLayout* temp = nullptr;
		ID3DBlob* blob = m_shaderBlobs[shader];
		LPVOID lpv = blob->GetBufferPointer();
		SIZE_T st = blob->GetBufferSize();
		HRESULT hr = m_device->CreateInputLayout(desc.data(), desc.size(), lpv, st, &temp);
		if (FAILED(hr)) {
			DEVLOG("Inputdesc creation failed");
			return nullptr;
		}
		return temp;
	}

	template<typename T>
	ID3D11Buffer* RendererDX11::createBuffer(const T& data, D3D11_USAGE usage, UINT bindflags, UINT cpuAccessFlags) {
		D3D11_BUFFER_DESC cbDesc;
		ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));
		cbDesc.ByteWidth = sizeof(T);
		cbDesc.Usage = usage;
		cbDesc.BindFlags = bindflags;
		cbDesc.CPUAccessFlags = cpuAccessFlags;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;
		ID3D11Buffer* temp = nullptr;
		//DEVLOG("\tBuffer Created");
		HRESULT hr = m_device->CreateBuffer(&cbDesc, nullptr, &temp);
		if (FAILED(hr)) {
			DEVLOG("buffer creation failed");
			return nullptr;
		}
		return temp;
	}

	template<typename T>
	bool RendererDX11::setBufferData(ID3D11Buffer* buffer, const T& data) {
		D3D11_MAPPED_SUBRESOURCE dataPtr;
		m_deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
		size_t s = sizeof(T);
		memcpy(dataPtr.pData, &data, sizeof(T));
		m_deviceContext->Unmap(buffer, 0);
		return true;
	}






}