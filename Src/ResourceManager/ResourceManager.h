#pragma once
#include <string>
#include <map>
#include <vector>
#include "Utils/Helpers.h"


namespace scuff3d {

	class Skeleton;
	class AnimationStack;
	class Mesh;
	class Sound;

	class ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();

		struct ModelFile {
			std::string fileName;
			Mesh* mesh = nullptr;

			Skeleton* skeleton = nullptr;
			AnimationStack* stack = nullptr;
			~ModelFile() { safeDelete(mesh); safeDelete(skeleton); safeDelete(stack); }
		};

		void addFile(const std::string& accessName, ModelFile* file);
		void addFile(const std::string& accessName, const std::string& fileName, Mesh* mesh, Skeleton* skeleton = nullptr, AnimationStack* animStack = nullptr);



		void addMesh(const std::string& accessName, Mesh* mesh);
		Mesh* getMesh(const std::string& name);
		std::map<std::string, Mesh*> getAllMeshes();
		std::map<std::string, ModelFile*>& getAllModelFiles();


		void loadFBX(const std::string& accessName, const std::string& filename, bool checkCache = false);
		void loadFBXMesh(const std:: string & filename);
		void loadFBXSkeleton(const std::string& filename);
		void loadFBXAnimation(const std::string& originalfilename, const std::string& filename);


		void loadObj(const std::string& accessName, const std::string& filename);

		void loadTexture(const std::string& filename);





	private:
		std::map<std::string, ModelFile*> m_files;
		//std::map<std::string, Mesh*> m_meshes;

		template<typename T>
		std::string getNextAvailableName(const std::string& name, std::map<std::string, T> m);

	};

	template<typename T>
	inline std::string ResourceManager::getNextAvailableName(const std::string& name, std::map<std::string, T> m) {
		std::string currentName = name;
		int i = 1;
		while (m.find(currentName) != m.end()) {
			currentName = name + " (" + std::to_string(i++) + ")";
		}
		return currentName;
	}

}