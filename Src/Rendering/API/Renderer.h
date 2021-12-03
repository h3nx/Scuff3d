#pragma once
#include <Windows.h>
#include <functional>
#include "Rendering/API/Geometry/Mesh.h"
#include "Rendering/API/Geometry/ModelData.h"
#include "Rendering/API/Camera/CameraData.h"

namespace scuff3d {

	typedef
	enum SHADER_TYPE {
		VERTEX_SHADER = 1,
		HULL_SHADER = 2,
		DOMAIN_SHADER = 3,
		GEOMETRY_SHADER = 4,
		PIXEL_SHADER = 5,
		COMPUTE_SHADER = 6
	} 	SHADER_TYPE;



	class Renderer {
	public:
		Renderer(const glm::vec2& initialSize);
		virtual ~Renderer();

		virtual void init() = 0;
		virtual void initImGui(HWND hWnd) = 0;

		virtual void renderWireFrame(ModelData* model, const glm::mat4& matrix) {};
		virtual void renderSolid(ModelData* model, const glm::mat4& matrix) {};

		
		virtual bool beginFrame(HWND hWnd, const std::string& viewport, CameraData* camera = nullptr) = 0;
		virtual void present(HWND hWnd) = 0;
		virtual void beginImGui() { };
		virtual void renderImGui(std::function<void()> imguiFunc) = 0;
		virtual void renderImGuiDebug() {};
		virtual void presentImGui() { };
		virtual void resizeRenderTarget(const glm::vec2& size);
		//virtual void resize(const std::string& name, const glm::vec2& size) = 0;
		virtual void setViewPort(const std::string& name, const glm::vec2& pos, const glm::vec2& size) { };
		virtual void resizeViewport(const std::string& name, const glm::vec2& size) { };

		//TODO add = 0
		virtual Shader* loadShader(const std::string& name, const std::string& path, const SHADER_TYPE type, const std::string& entryPoint = "main", const std::string& target = "5_0") { return nullptr; };
		virtual ModelData* createModel(const std::string& name, Mesh* data) { return nullptr; };
		virtual ModelData* createModel(const std::string& name, Mesh* data, Shader* vertex, Shader* pixel, Shader* geometry, Shader* hull, Shader* domain) { return nullptr; };
		virtual ModelData* createModel(const std::string& name, Mesh* data, const std::string& vertex, const std::string& pixel, const std::string& geometry = "", const std::string& hull = "", const std::string& domain = "") { return nullptr; };
		void setMainCamera(CameraData* camera);
		CameraData* getMainCamera();

		Shader* getShader(const std::string& name);
		const std::vector<std::string>& getShaderNames(const SHADER_TYPE type);
		const std::map<std::string, Shader*>& getAllShaders();


	protected:
		CameraData* m_mainCamera;
		HWND m_renderTarget;
		std::vector<std::string> m_pixelShaders;
		std::vector<std::string> m_vertexShaders;
		std::vector<std::string> m_geometryShaders;
		std::vector<std::string> m_hullShaders;
		std::vector<std::string> m_domainShaders;
		std::vector<std::string> m_computeShaders;
		std::map<std::string, Shader*> m_shaders;


		std::map<std::string, size_t> m_modelNames;
		std::map<size_t, ModelData*> m_models;
		std::queue<glm::vec2> m_toResize;



	protected:

		const std::string targetString(const SHADER_TYPE type);

	};


}