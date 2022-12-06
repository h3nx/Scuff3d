#include "pch.h"
#include "ModelData.h"





namespace scuff3d {
	ModelData::ModelData(Mesh* mesh) :
		m_mesh(mesh),
		m_vertexShader(nullptr),
		m_geometryShader(nullptr),
		m_hullShader(nullptr),
		m_domainShader(nullptr),
		m_pixelShader(nullptr)
	{
	}

	ModelData::~ModelData() {
	}

	void ModelData::init() {
	}

	void ModelData::setVertexShader(Shader* shader) {
		m_vertexShader = shader;
	}
	void ModelData::setGeometryShader(Shader* shader) {

		m_geometryShader = shader;
	}
	void ModelData::setHullShader(Shader* shader) {

		m_hullShader = shader;
	}
	void ModelData::setDomainShader(Shader* shader) {

		m_domainShader = shader;
	}
	void ModelData::setPixelShader(Shader* shader) {

		m_pixelShader = shader;
	}

	Mesh* ModelData::getMesh() {
		return m_mesh;
	}

	Shader* ModelData::getVertexShader() {
		return m_vertexShader;
	}
	Shader* ModelData::getGeometryShader() {
		return nullptr;
	}
	Shader* ModelData::getHullShader() {
		return nullptr;
	}
	Shader* ModelData::getDomainShader() {
		return nullptr;
	}
	Shader* ModelData::getPixelShader() {
		return m_pixelShader;
	}

	const unsigned int ModelData::getStride() const {
		return m_stride;
	}
	const unsigned int ModelData::getOffset() const {
		return m_offset;
	}

	

}









