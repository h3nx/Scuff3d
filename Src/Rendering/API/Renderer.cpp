#include "pch.h"
#include "Renderer.h"
namespace scuff3d
{

	Renderer::Renderer(const glm::vec2& initialSize) 
		// :
		//m_size(initialSize)
		//,m_hwnd(hwnd)
	{
	}

	Renderer::~Renderer() {
		safeDeleteMap(m_models);
		safeDeleteMap(m_shaders);
	}


	void Renderer::resizeRenderTarget(const glm::vec2& size) {
		m_toResize.push(size);
		DEVLOG(std::string("adding " + to_string(size)) + " to rendertarget resize queue");
	}

	void Renderer::setMainCamera(CameraData* camera) {
		m_mainCamera = camera;
	}

	CameraData* Renderer::getMainCamera() {
		return m_mainCamera;
	}



	Shader* Renderer::getShader(const std::string& name) {
		if(m_shaders.find(name) == m_shaders.end())
			return nullptr;
		return m_shaders[name];
	}
	const std::vector<std::string>& Renderer::getShaderNames(const SHADER_TYPE type) {
		switch (type) {
			case VERTEX_SHADER:
				return m_vertexShaders;
			case HULL_SHADER:
				return m_hullShaders;
			case DOMAIN_SHADER:
				return m_domainShaders;
			case GEOMETRY_SHADER:
				return m_geometryShaders;
			case PIXEL_SHADER:
				return m_pixelShaders;
			case COMPUTE_SHADER:
				return m_computeShaders;
		}
	}
	const std::map<std::string, Shader*>& Renderer::getAllShaders() {
		return m_shaders;
	}


	const std::string Renderer::targetString(const SHADER_TYPE type) {
		const std::string types[6] = {
			"vs_",
			"hs_",
			"ds_",
			"gs_",
			"ps_",
			"cs_"
		};
		return types[type - 1];
	}


}