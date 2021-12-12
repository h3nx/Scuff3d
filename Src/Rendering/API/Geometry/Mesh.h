#pragma once
#include "glm/glm.hpp"

namespace scuff3d
{
	typedef int MeshDataFlags;        // -> enum MeshDataFlags_ 
	//WARNING: data will be exported in the same order as the enum
	enum MeshDataFlags_ {
		MeshDataFlags_None = 0,
		MeshDataFlags_Position = 1 << 0,
		MeshDataFlags_Normal = 1 << 1,
		MeshDataFlags_Tangent = 1 << 2,
		MeshDataFlags_BiTangent = 1 << 3,
		MeshDataFlags_UV = 1 << 4,
		MeshDataFlags_Color = 1 << 5,
	};


	class Mesh
	{
	public:
		Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec3* normal);
		Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec4* colors);
		Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec3* normal, glm::vec2* uv);
		Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec3* normal, glm::vec4* colors);
		Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec3* normal, glm::vec3* tangent, glm::vec3* bitangent, glm::vec2* uv);

		Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec3* normal, unsigned int* indices, const unsigned int indCount);
		Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec4* colors, unsigned int* indices, const unsigned int indCount);
		Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec3* normal, glm::vec2* uv, unsigned int* indices, const unsigned int indCount);
		Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec3* normal, glm::vec4* colors, unsigned int* indices, const unsigned int indCount);
		Mesh(const std::string& name, const unsigned int count, glm::vec3* position, glm::vec3* normal, glm::vec3* tangent, glm::vec3* bitangent, glm::vec2* uv, unsigned int* indices, const unsigned int indCount);
		~Mesh();

		const std::string& getName() const;

		const unsigned int getCount() const;
		const unsigned int getIndicesCount() const;
		const unsigned int* getIndices() const;
		const glm::vec3* getPositions() const;
		const glm::vec3* getNormals() const;
		const glm::vec3* getTangents() const;
		const glm::vec3* getBitangents() const;
		const glm::vec2* getUvs() const;
		const glm::vec4* getColors() const;

		// Returns all existing data, see enum for order
		// delete when done, returns copy
		void* getData();
		// delete when done, returns copy
		void* getData(const MeshDataFlags flags);
		// Returns vertex Bytesize of existing data
		const unsigned int vertexByteSize();
		// Returns vertex Bytesize for selected data

		static const int vertexByteSize(const MeshDataFlags flags);
		const int numberOfFlags();
		// Returns number of existing dataflags
		static const int numberOfFlags(const MeshDataFlags flags);
		const MeshDataFlags getFlags();

		void renderImGui();

		static Mesh* CreateCube(const glm::vec3& size = { 1,1,1 }, const glm::vec3& origin = { 0,0,0 });
		static Mesh* CreateCube(const int size, const glm::vec3& origin = { 0,0,0 });
		static Mesh* CreateCubeQuadMesh(const glm::vec3& size = { 1,1,1 }, const glm::vec3& origin = { 0,0,0 });
	private:
		Mesh(const std::string& name, const unsigned int count);
		Mesh(const std::string& name, const unsigned int count, glm::vec3* positions, glm::vec3* normals, glm::vec3* tangents, glm::vec3* bitangents, glm::vec2* uv, glm::vec4* colors, unsigned int* indices, const unsigned int indCount);
		std::string m_name;
		unsigned int m_indicesCount;
		unsigned int m_count;
		unsigned int* m_indices;
		glm::vec3* m_positions;
		glm::vec3* m_normals;
		glm::vec3* m_tangents;
		glm::vec3* m_bitangents;
		glm::vec2* m_uvs;
		glm::vec4* m_colors;

		void setName(const std::string& name);
		void setCount(const int& count);
		void setIndicesCount(const int& indCount);
		void setIndices(unsigned int* indices);
		void setPositions(glm::vec3* positions);
		void setNormals(glm::vec3* normals);
		void setTangents(glm::vec3* tangents);
		void setBitangents(glm::vec3* bitangents);
		void setUvs(glm::vec2* uvs);
		void setColors(glm::vec4* colors);
	};


}