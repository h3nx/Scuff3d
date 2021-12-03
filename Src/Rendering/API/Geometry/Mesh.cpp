#include "pch.h"
#include "Mesh.h"

namespace scuff3d {

	size_t meshIdCounter = 0;
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec3* normal) {
		m_name = name;
		m_count = count;
		m_positions = position;
		m_normals = normal;
	}
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec4* colors) {
		m_name = name;
		m_count = count;
		m_positions = position;
		m_colors = colors;
	}
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec3* normal, glm::vec3* uv) : Mesh(name, count, position, normal) {
		m_uvs = uv;
	}
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec3* normal, glm::vec4* colors) : Mesh(name, count, position, normal) {
		m_colors = colors;
	}
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec3* normal, glm::vec3* tangent, glm::vec3* bitangent, glm::vec3* uv) : Mesh(name, count, position, normal) {
		m_tangents = tangent;
		m_bitangents = bitangent;
	}

	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec3* normal, unsigned int* indices, const unsigned int indCount) : Mesh(name, count, position, normal) {
		m_indices = indices;
		m_indicesCount = indCount;
	}
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec4* colors, unsigned int* indices, const unsigned int indCount) : Mesh(name, count, position, colors){
		m_indices = indices;
		m_indicesCount = indCount;
	}
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec3* normal, glm::vec3* uv, unsigned int* indices, const unsigned int indCount) : Mesh(name, count, position, normal, indices, indCount) {
		m_uvs = uv;
	}
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec3* normal, glm::vec4* colors, unsigned int* indices, const unsigned int indCount) : Mesh(name, count, position, normal, indices, indCount) {
		m_colors = colors;
	}
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec3* normal, glm::vec3* tangent, glm::vec3* bitangent, unsigned int* indices, const unsigned int indCount) : Mesh(name, count, position, normal, indices, indCount) {
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

	const std::string& Mesh::getName() const {
		return m_name;
	}

	const unsigned int Mesh::getCount() const {
		return m_count;
	}
	const unsigned int Mesh::getIndicesCount() const {
		return m_indicesCount;
	}
	const unsigned int* Mesh::getIndices() const {
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

	// Returns all existing data, see enum for order
	void* Mesh::getData() {
		return getData(getFlags());
	}
	void* Mesh::getData(const MeshDataFlags flags) {
		unsigned int vertexSize = vertexByteSize(flags);
		void* vertices = malloc(m_count * vertexSize);
		for (int i = 0; i < m_count; i++) {
			unsigned int offset = 0;
			void* addr = (char*)vertices + vertexSize*i;
			if (flags & MeshDataFlags_Position) {
				memcpy(addr, &m_positions[i], sizeof(glm::vec3));
				offset += sizeof(glm::vec3);
				addr = (char*)vertices + vertexSize * i + offset;
			}
			if (flags & MeshDataFlags_Normal) {
				memcpy(addr, &m_normals[i], sizeof(glm::vec3));
				offset += sizeof(glm::vec3);
				addr = (char*)vertices + vertexSize * i + offset;
			}
			if (flags & MeshDataFlags_Tangent) {
				memcpy(addr, &m_tangents[i], sizeof(glm::vec3));
				offset += sizeof(glm::vec3);
				addr = (char*)vertices + vertexSize * i + offset;
			}
			if (flags & MeshDataFlags_BiTangent) {
				memcpy(addr, &m_bitangents[i], sizeof(glm::vec3));
				offset += sizeof(glm::vec3);
				addr = (char*)vertices + vertexSize * i + offset;
			}
			if (flags & MeshDataFlags_UV) {
				memcpy(addr, &m_uvs[i], sizeof(glm::vec3));
				offset += sizeof(glm::vec3);
				addr = (char*)vertices + vertexSize * i + offset;
			}
			if (flags & MeshDataFlags_Color) {
				memcpy(addr, &m_colors[i], sizeof(glm::vec4));
			}
		}
		return vertices;
	}

	// Returns vertex Bytesize of existing data 
	const unsigned int Mesh::vertexByteSize() {
		return vertexByteSize(getFlags());
	}
	const int Mesh::vertexByteSize(const MeshDataFlags flags) {
		int sum = 0;
		if (flags & MeshDataFlags_Position)
			sum += sizeof(glm::vec3);
		if (flags & MeshDataFlags_Normal)
			sum += sizeof(glm::vec3);
		if (flags & MeshDataFlags_Tangent)
			sum += sizeof(glm::vec3);
		if (flags & MeshDataFlags_BiTangent)
			sum += sizeof(glm::vec3);
		if (flags & MeshDataFlags_UV)
			sum += sizeof(glm::vec3);
		if (flags & MeshDataFlags_Color)
			sum += sizeof(glm::vec4);
		return sum;
	}

	// Returns number of existing dataflags
	const int Mesh::numberOfFlags(){
		return numberOfFlags(getFlags());
	}
	const int Mesh::numberOfFlags(const MeshDataFlags flags) {
		int sum = 0;
		if (flags & MeshDataFlags_Position)
			sum += 1;
		if (flags & MeshDataFlags_Normal)
			sum += 1;
		if (flags & MeshDataFlags_Tangent)
			sum += 1;
		if (flags & MeshDataFlags_BiTangent)
			sum += 1;
		if (flags & MeshDataFlags_UV)
			sum += 1;
		if (flags & MeshDataFlags_Color)
			sum += 1;
		return sum;
	}

	const MeshDataFlags Mesh::getFlags() {
		MeshDataFlags flags = 0;
		if (m_positions != nullptr)
			flags |= MeshDataFlags_Position;
		if (m_normals != nullptr)
			flags |= MeshDataFlags_Normal;
		if (m_tangents != nullptr)
			flags |= MeshDataFlags_Tangent;
		if (m_bitangents != nullptr)
			flags |= MeshDataFlags_BiTangent;
		if (m_uvs != nullptr)
			flags |= MeshDataFlags_UV;
		if (m_colors != nullptr)
			flags |= MeshDataFlags_Color;
		return flags;
	}

	Mesh* Mesh::CreateCube(const glm::vec3& size, const glm::vec3& origin) {
		const auto& o = origin;
		const auto& s = size;
		const auto hs = size * 0.5f;
		glm::vec3* positions = new glm::vec3[8]{
			{-hs.x,		-hs.y,		hs.z},
			{-hs.x,		-hs.y,		-hs.z},
			{hs.x,		-hs.y,		-hs.z},
			{hs.x,		-hs.y,		hs.z},

			{-hs.x,		hs.y,		hs.z},
			{-hs.x,		hs.y,		-hs.z},
			{hs.x,		hs.y,		-hs.z},
			{hs.x,		hs.y,		hs.z},
		};
		/*for (int i = 0; i < 8; i++)
			positions[i] -= o;*/
		glm::vec4* colors = new glm::vec4[8]{
			{1,0,0,1 },
			{0,1,0,1 },
			{0,0,1,1 },
			{1,0,1,1 },
			{1,1,1,1 },
			{1,1,1,1 },
			{1,1,1,1 },
			{1,1,1,1 },
		};



		unsigned int* indices = new unsigned int[]{ 
			1, 2, 0, 
			0, 2, 3,

			5,6,4
		};

		return new Mesh("Cube", 8, positions, colors, indices, 9);
	}

	Mesh* Mesh::CreateCubeQuadMesh(const glm::vec3& size, const glm::vec3& origin) {
		return nullptr;
	}

}


