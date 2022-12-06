#include "pch.h"
#include "ResourceManager.h"
#include "glm/glm.hpp"
#include "Rendering/API/Geometry/Mesh.h"
#include "Utils/Helpers.h"


namespace scuff3d {
	ResourceManager::ResourceManager() {

	}
	ResourceManager::~ResourceManager() {
		scuff3d::safeDeleteMap(m_files);
		//scuff3d::safeDeleteMap(m_meshes);
	}
	Mesh* ResourceManager::getMesh(const std::string& name)
	{ 
		if (m_files.find(name) != m_files.end())
			return m_files[name]->mesh;
		return nullptr;
	}
	std::map<std::string, Mesh*> ResourceManager::getAllMeshes()
	{
		//return m_meshes;
		return std::map<std::string, Mesh*>();
	}

	std::map<std::string, ResourceManager::ModelFile*>& ResourceManager::getAllModelFiles() {
		return m_files;
	}

	void ResourceManager::loadFBX(const std::string& accessName, const std::string& filename, bool checkCache) {
		Mesh* m = nullptr;
		Skeleton* s = nullptr;
		AnimationStack* a = nullptr;

		if (m_fbxImporter.loadFile(filename, m, s, a)) {
			addFile(accessName, filename, m, s, a);
		}
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
		std::vector<unsigned int> finalIndices;

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
			if (text == "") {
				return v;
			}
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

		std::function<void(std::string& txt)> removeTrailingSpaces = [](std::string& txt) {
			if (txt == "")
				return;
			while (txt.rfind(" ") == txt.size() - 1)
				txt = txt.substr(0, txt.size() - 1);
		};

		//Good: While file is open. 
		while (file.good()) { 
			
			std::getline(file, line);
			removeTrailingSpaces(line);
			pos = line.find(" ");
			// Empty line, continue to next.
			if (line.size() == 0) {
				continue;
			}
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

		const size_t size = finalPositions.size();
	
		addFile(
			accessName, 
			filename, 
			new Mesh(
				accessName, 
				(unsigned int)finalPositions.size(), 
				finalPositions.data(), 
				finalNormals.data(), 
				finalColors.data(), 
				finalIndices.data(), 
				(unsigned int)finalIndices.size()
			)
		);
	}
	void ResourceManager::loadTexture(const std::string& filename) {

	}
	void ResourceManager::addFile(const std::string& accessName, ModelFile* file) {
		m_files[getNextAvailableName(accessName, m_files)] = file;
	}
	void ResourceManager::addFile(const std::string& accessName, const std::string& fileName, Mesh* mesh, Skeleton* skeleton, AnimationStack* animStack) {
		addFile(accessName, new ModelFile{ fileName, mesh, skeleton, animStack });
	}
	void ResourceManager::addMesh(const std::string& name, Mesh* mesh) {
		
		//m_meshes[getNextAvailableName(mesh->getName(),m_meshes)] = mesh;
	}
}