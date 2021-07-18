#pragma once
#include "glm/common.hpp"

namespace scuff3d
{



	class Mesh
	{
	public:
		Mesh(const int count, glm::vec3* position, glm::vec3* normal);
		Mesh(const int count, glm::vec3* position, glm::vec3* normal, glm::vec3* uv);
		Mesh(const int count, glm::vec3* position, glm::vec3* normal, glm::vec4* colors);
		Mesh(const int count, glm::vec3* position, glm::vec3* normal, glm::vec3* tangent, glm::vec3* bitangent, glm::vec3* uv);

		Mesh(const int count, glm::vec3* position, glm::vec3* normal, int* indices);
		Mesh(const int count, glm::vec3* position, glm::vec3* normal, glm::vec3* uv, int* indices);
		Mesh(const int count, glm::vec3* position, glm::vec3* normal, glm::vec4* colors, int* indices);
		Mesh(const int count, glm::vec3* position, glm::vec3* normal, glm::vec3* tangent, glm::vec3* bitangent, int* indices);
		~Mesh();


		const int getCount() const;
		const int* getIndices() const;
		const glm::vec3* getPositions() const;
		const glm::vec3* getNormals() const;
		const glm::vec3* getTangents() const;
		const glm::vec3* getBitangents() const;
		const glm::vec3* getUvs() const;
		const glm::vec4* getColors() const;





	private:

		int m_count;
		
		int* m_indices;
		glm::vec3* m_positions;
		glm::vec3* m_normals;
		glm::vec3* m_tangents;
		glm::vec3* m_bitangents;
		glm::vec3* m_uvs;
		glm::vec4* m_colors;
	};


}