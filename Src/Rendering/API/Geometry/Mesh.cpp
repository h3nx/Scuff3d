#include "pch.h"
#include "Mesh.h"

namespace scuff3d {

	size_t meshIdCounter = 0;
	Mesh::Mesh(const std::string& name, const unsigned int count) : m_name(name),m_count(count),m_positions(nullptr),m_normals(nullptr),m_colors(nullptr),m_uvs(nullptr),m_tangents(nullptr),m_bitangents(nullptr),m_indicesCount(0), m_indices(nullptr) { }
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* positions, glm::vec3* normals, glm::vec3* tangents, glm::vec3* bitangents, glm::vec2* uv, glm::vec4* colors, unsigned int* indices, const unsigned int indCount) : Mesh(name, count) {
		if (positions) setPositions(positions);
		if (normals) setNormals(normals);
		if (tangents) setTangents(tangents);
		if (bitangents) setBitangents(bitangents);
		if (uv) setUvs(uv);
		if (colors) setColors(colors);
		if (indCount) setIndicesCount(indCount);
		if (indices) setIndices(indices);
	}
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* positions, glm::vec3* normals) : 
		Mesh(name, count, positions, normals, nullptr, nullptr, nullptr, nullptr, nullptr, 0) { }
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* positions, glm::vec4* colors) :
		Mesh(name, count, positions, nullptr, nullptr, nullptr, nullptr, colors, nullptr, 0) { }
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* positions, glm::vec3* normals, glm::vec2* uvs) :
		Mesh(name, count, positions, normals, nullptr, nullptr, uvs, nullptr, nullptr, 0) { }
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* positions, glm::vec3* normals, glm::vec4* colors) :
		Mesh(name, count, positions, normals, nullptr, nullptr, nullptr, colors, nullptr, 0) { }
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* positions, glm::vec3* normals, glm::vec3* tangents, glm::vec3* bitangents, glm::vec2* uv) :
		Mesh(name, count, positions, normals, tangents, nullptr, nullptr, nullptr, nullptr, 0) { }
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* positions, glm::vec3* normals, unsigned int* indices, const unsigned int indCount) :
		Mesh(name, count, positions, normals, nullptr, nullptr, nullptr, nullptr, indices, indCount) { }
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* positions, glm::vec4* colors, unsigned int* indices, const unsigned int indCount) :
		Mesh(name, count, positions, nullptr, nullptr, nullptr, nullptr, colors, indices, indCount) { }
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* positions, glm::vec3* normals, glm::vec2* uvs, unsigned int* indices, const unsigned int indCount) :
		Mesh(name, count, positions, normals, nullptr, nullptr, uvs, nullptr, indices, indCount) { }
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* positions, glm::vec3* normals, glm::vec4* colors, unsigned int* indices, const unsigned int indCount) :
		Mesh(name, count, positions, normals, nullptr, nullptr, nullptr, colors, indices, indCount) { }
	Mesh::Mesh(const std::string& name, const unsigned int count, glm::vec3* positions, glm::vec3* normals, glm::vec3* tangents, glm::vec3* bitangents, glm::vec2* uvs, unsigned int* indices, const unsigned int indCount) :
		Mesh(name, count, positions, normals, tangents, bitangents, uvs, nullptr, indices, indCount) { }

	Mesh::~Mesh() {
		if (m_indices) delete m_indices;
		if (m_positions) delete m_positions;
		if (m_normals) delete m_normals;
		if (m_tangents) delete m_tangents;
		if (m_bitangents) delete m_bitangents;
		if (m_uvs) delete m_uvs;
		if (m_colors) delete m_colors;
	}

	void Mesh::setName(const std::string& name) {
		m_name = name;
	}
	void Mesh::setCount(const int& count) {
		m_count = count;
	}
	void Mesh::setIndicesCount(const int& indCount) {
		m_indicesCount = indCount;
	}
	void Mesh::setIndices(unsigned int* indices) {
		safeDelete(m_indices);
		m_indices = NEW unsigned int[m_indicesCount];
		memcpy(m_indices, indices, m_indicesCount * sizeof(unsigned int));
	}
	void Mesh::setPositions(glm::vec3* positions) {
		safeDelete(m_positions);
		m_positions = NEW glm::vec3[m_count];
		memcpy(m_positions, positions, m_count * sizeof(glm::vec3));
	}
	void Mesh::setNormals(glm::vec3* normals) {
		safeDelete(m_normals);
		m_normals = NEW glm::vec3[m_count];
		memcpy(m_normals, normals, m_count * sizeof(glm::vec3));
	}
	void Mesh::setTangents(glm::vec3* tangents) {
		safeDelete(m_tangents);
		m_tangents = NEW glm::vec3[m_count];
		memcpy(m_tangents, tangents, m_count * sizeof(glm::vec3));
	}
	void Mesh::setBitangents(glm::vec3* bitangents) {
		safeDelete(m_bitangents);
		m_bitangents = NEW glm::vec3[m_count];
		memcpy(m_bitangents, bitangents, m_count * sizeof(glm::vec3));
	}
	void Mesh::setUvs(glm::vec2* uvs) {
		safeDelete(m_uvs);
		m_uvs = NEW glm::vec2[m_count];
		memcpy(m_uvs, uvs, m_count * sizeof(glm::vec2));
	}
	void Mesh::setColors(glm::vec4* colors) {
		safeDelete(m_colors);
		m_colors = NEW glm::vec4[m_count];
		memcpy(m_colors, colors, m_count * sizeof(glm::vec4));
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
	const glm::vec2* Mesh::getUvs() const {
		return m_uvs;
	}
	const glm::vec4* Mesh::getColors() const {
		return m_colors;
	}
	const std::string& Mesh::getName() const {
		return m_name;
	}


	void* Mesh::getData() {
		return getData(getFlags());
	}
	void* Mesh::getData(const MeshDataFlags flags) {
		unsigned int vertexSize = vertexByteSize(flags);
		void* vertices = malloc(m_count * vertexSize);
		for (unsigned int i = 0; i < m_count; i++) {
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

	const int Mesh::numberOfFlags() {
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

	void Mesh::renderImGui() {
		ImGui::Text("Vertices");
		ImGui::SameLine();
		ImGui::rText(std::to_string(getCount()).c_str());
		ImGui::Text("Indices");
		ImGui::SameLine();
		ImGui::rText(std::to_string(getIndicesCount()).c_str());
		ImGui::Text("Vertex ByteSize");
		ImGui::SameLine();
		ImGui::rText(std::to_string(vertexByteSize()).c_str());
		ImGui::Separator();
		ImGui::BeginGroupPanel("Layout"); {
			if (getPositions()) {
				ImGui::Text("Position");
			}
			if (getNormals()) {
				ImGui::Text("Normal");
			}
			if (getTangents()) {
				ImGui::Text("Tangent");
			}
			if (getBitangents()) {
				ImGui::Text("BiTangent");
			}
			if (getUvs()) {
				ImGui::Text("UV");
			}
			if (getColors()) {
				ImGui::Text("Color");
			}
		}
		ImGui::EndGroupPanel();


	}


	Mesh* Mesh::CreateCube(const int size, const glm::vec3& origin) {
		return CreateCube({ size, size, size });
	}
	Mesh* Mesh::CreateCube(const glm::vec3& size, const glm::vec3& origin) {
		const auto& o = origin;
		const auto& s = size;
		const auto hs = size * 0.5f;
		glm::vec3 positions[] = {
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
		glm::vec4 colors[] = {
			{1,0,0,1 },
			{0,1,0,1 },
			{0,0,1,1 },
			{1,0,1,1 },
			{1,1,1,1 },
			{1,1,1,1 },
			{1,1,1,1 },
			{1,1,1,1 },
		};



		unsigned int indices[] = {
			1, 2, 0, 
			0, 2, 3,

			5,6,4
		};

		return NEW Mesh("Cube", 8, positions, colors, indices, 9);
	}

	Mesh* Mesh::CreateCubeQuadMesh(const glm::vec3& size, const glm::vec3& origin) {
		return nullptr;
	}

	

}


