#pragma once
#include "Mesh.h"
#include "Rendering/API/Shader/Shader.h"

namespace scuff3d
{

	class ModelData
	{
	public:
		ModelData(Mesh* mesh);
		virtual ~ModelData();
		virtual void init();
		
		void setVertexShader(Shader* shader);
		void setGeometryShader(Shader* shader);
		void setHullShader(Shader* shader);
		void setDomainShader(Shader* shader);
		void setPixelShader(Shader* shader);

		const Mesh* getMesh() const;
		Shader* getVertexShader();
		Shader* getGeometryShader();
		Shader* getHullShader();
		Shader* getDomainShader();
		Shader* getPixelShader();

		const unsigned int getStride() const;
		const unsigned int getOffset() const;

	protected:
		Mesh* m_mesh;

		Shader* m_vertexShader;
		Shader* m_geometryShader;
		Shader* m_hullShader;
		Shader* m_domainShader;
		Shader* m_pixelShader;

		unsigned int m_offset;
		unsigned int m_stride;
	};

}