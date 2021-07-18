#include "pch.h"
#include "Mesh.h"

namespace scuff3d {


	Mesh::Mesh(const int count, glm::vec3* position, glm::vec3* normal) {
		m_count = count;
		m_positions = position;
		m_normals = normal;
	}
	Mesh::Mesh(const int count, glm::vec3* position, glm::vec3* normal, glm::vec3* uv) : Mesh(count, position, normal){
		m_uvs = uv;
	}
	Mesh::Mesh(const int count, glm::vec3* position, glm::vec3* normal, glm::vec4* colors) : Mesh(count, position, normal) {
		m_colors = colors;
	}
	Mesh::Mesh(const int count, glm::vec3* position, glm::vec3* normal, glm::vec3* tangent, glm::vec3* bitangent, glm::vec3* uv) : Mesh(count, position, normal) {
		m_tangents = tangent;
		m_bitangents = bitangent;
	}

	Mesh::Mesh(const int count, glm::vec3* position, glm::vec3* normal, int* indices) : Mesh(count, position, normal){
		m_indices = indices;
	}
	Mesh::Mesh(const int count, glm::vec3* position, glm::vec3* normal, glm::vec3* uv, int* indices) : Mesh(count, position, normal, indices) {
		m_uvs = uv;
	}
	Mesh::Mesh(const int count, glm::vec3* position, glm::vec3* normal, glm::vec4* colors, int* indices) : Mesh(count, position, normal, indices) {
		m_colors = colors;
	}
	Mesh::Mesh(const int count, glm::vec3* position, glm::vec3* normal, glm::vec3* tangent, glm::vec3* bitangent, int* indices) : Mesh(count, position, normal, indices) {
		m_tangents = tangent;
		m_bitangents = bitangent;
	}

	Mesh::~Mesh() {
		if (m_indices) delete m_indices;
		if (m_positions) delete m_positions;
		if (m_normals) delete m_normals;
		if (m_tangents) delete m_tangents;
		if (m_bitangents) delete m_bitangents;
		if (m_uvs) delete m_uvs;
		if (m_colors) delete m_colors;
	}

	const int Mesh::getCount() const {
		return m_count;
	}
	const int* Mesh::getIndices() const {
		return m_indices;
	}
	const glm::vec3* Mesh::getPositions() const {
		return m_positions;
	}
	const glm::vec3* Mesh::getNormals() const {
		return m_normals;
	}
	const glm::vec3* Mesh::getTangents() const {
		return m_tangents;
	}
	const glm::vec3* Mesh::getBitangents() const {
		return m_bitangents;
	}
	const glm::vec3* Mesh::getUvs() const {
		return m_uvs;
	}
	const glm::vec4* Mesh::getColors() const {
		return m_colors;
	}

}


