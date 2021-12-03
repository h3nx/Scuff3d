#include "pch.h"
#include "ResourceManager.h"
#include "glm/glm.hpp"
#include "Rendering/API/Geometry/Mesh.h"

namespace scuff3d {
	ResourceManager::ResourceManager() {

	}
	ResourceManager::~ResourceManager() {
		for (auto& pair : m_meshes) {
			if(pair.second)
				delete pair.second;
			pair.second = nullptr;
		}
	}
	Mesh* ResourceManager::getMesh(const std::string& name)
	{
		if (m_meshes.find(name) != m_meshes.end())
			return m_meshes[name];
		return nullptr;
	}
	std::map<std::string, Mesh*> ResourceManager::getAllMeshes()
	{
		return m_meshes;
	}
	void ResourceManager::loadFBX(const std::string& filename) {








	}
	void ResourceManager::loadFBXAnimation(const std::string& filename, const std::string& stackName) {

	}
	void ResourceManager::loadObj(const std::string& accessName, const std::string& filename) {

		std::string name;

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> tangents;

		std::vector<glm::vec3> finalPositions;
		std::vector<glm::vec3> finalNormals;
		std::vector<glm::vec2> finalUVs;
		std::vector<glm::vec4> finalColors;
		std::vector<int> finalIndices;

		std::fstream file(filename); //Open file
		std::string line;
		std::string temp;
		std::string innerTemp;
		size_t pos;
		size_t innerPos;


		std::function<std::vector<int>(const std::string& text)> vertexExtraction = [](const std::string& text) { 
			std::vector<int> v;
			std::string line = text;
			std::string current = "";
			int pos = 0;
			while (pos != std::string::npos) {
				pos = line.find("/");
				current = line.substr(0, pos);
				line = line.substr(pos + 1);
				v.push_back(std::stoi(current)-1);
			}

			return v; 
		};


		std::function<glm::vec3(std::string)> getVec3 = [](std::string line) {
			glm::vec3 vert;
			size_t pos = 0;
			//line = line.substr(pos + 2);
			pos = line.find(" ");
			vert.x = std::stof(line.substr(0, pos));

			line = line.substr(pos + 1);
			pos = line.find(" ");
			vert.y = std::stof(line.substr(0, pos));

			line = line.substr(pos + 1);
			pos = line.find(" ");
			vert.z = std::stof(line.substr(0, pos));
			return vert;
		};
		std::function<glm::vec2(std::string)> getVec2 = [](std::string line) {
			glm::vec2 vert;
			size_t pos = 0;
			//line = line.substr(pos + 3);
			pos = line.find(" ");
			vert.x = std::stof(line.substr(0, pos));

			line = line.substr(pos + 1);
			pos = line.find(" ");
			vert.y = std::stof(line.substr(0, pos));
			return vert;
		};



		//Good: While file is open. 
		while (file.good()) { 
		
			std::getline(file, line);
			pos = line.find(" ");

			// Empty line, continue to next.
			if (pos == std::string::npos) { 
			
				continue;
			}
			temp = line.substr(0, pos);
			if (temp == "#") {
				continue; //Comment, contine next line. 
			}
			else if (temp == "mtllib") //Material
			{

			}
			//objects name
			else if (temp == "o") {
				name = line.substr(pos + 1);

			}
			else if (temp == "v") {
				positions.push_back(getVec3(line.substr(pos + 1)));
			}
			else if (temp == "vn") {
				normals.push_back(getVec3(line.substr(pos + 2)));
			}
			else if (temp == "vt") {
				texCoords.push_back(getVec2(line.substr(pos + 2)));
			}
			else if (temp == "usemtl") //If we want materials
			{
			}
			//Smoothing something something
			else if (temp == "s") {
			}
			//Face, triangles. 
			else if (temp == "f") {

				line = line.substr(pos + 1);
				std::vector<std::vector<int>> faceData;

				
				while (pos != std::string::npos) {
					pos = line.find(" ");
					temp = line.substr(0, pos);
					line = line.substr(pos + 1);
					faceData.push_back(vertexExtraction(temp));
				}
				

				if (faceData.size() > 0) {
					for (auto& v : faceData) {
						
						int index = -1;
						glm::vec3& currentPos = positions[v[0]];
						for (int i = 0; i < finalPositions.size(); i++) {
							if (finalPositions[i] == currentPos) {
								if (finalUVs[i] == texCoords[v[1]]) {
									if (finalNormals[i] == normals[v[2]]) {
										index = i;
									}
								}
							}
						}
						if (index == -1) {
							finalIndices.push_back(finalPositions.size());
							finalPositions.push_back(currentPos);
							finalColors.push_back(glm::vec4(1, 1, 1, 1));
							if(v.size() > 1)
								finalUVs.push_back(texCoords[v[1]]);
							if (v.size() > 2)
								finalNormals.push_back(normals[v[2]]);
						}
						else {
							finalIndices.push_back(index);
						}


					}

				}



			}
			else
			{
				continue; //If weird stuff continue to next line
			}

		}

		glm::vec3* p = new glm::vec3[finalPositions.size()];
		glm::vec3* n = new glm::vec3[finalPositions.size()];
		//glm::vec2* uv = new glm::vec2[finalPositions.size()];
		glm::vec4* c = new glm::vec4[finalPositions.size()];
		unsigned int* ind = new unsigned int[finalIndices.size()];

		memcpy(p, finalPositions.data(), finalPositions.size() * sizeof(glm::vec3));
		memcpy(n, finalNormals.data(), finalPositions.size() * sizeof(glm::vec3));
		memcpy(c, finalColors.data(), finalPositions.size() * sizeof(glm::vec4));
		memcpy(ind, finalIndices.data(), finalIndices.size() * sizeof(unsigned int));


		addMesh(accessName, new Mesh(accessName, (unsigned int)finalPositions.size(), p, n, c, ind, (unsigned int)finalIndices.size()));




	}
	void ResourceManager::loadTexture(const std::string& filename) {

	}
	void ResourceManager::addMesh(const std::string& name, Mesh* mesh) {
		
		m_meshes[getNextAvailableName(mesh->getName(),m_meshes)] = mesh;
	}
}