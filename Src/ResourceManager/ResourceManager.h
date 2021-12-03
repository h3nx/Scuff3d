#pragma once
#include <string>
#include <map>
#include <vector>



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


		Mesh* getMesh(const std::string& name);
		std::map<std::string, Mesh*> getAllMeshes();

		void addMesh(const std::string& accessName, Mesh* mesh);
		void loadFBX(const std::string& filename);
		void loadFBXAnimation(const std::string& filename, const std::string& stackName);
		void loadObj(const std::string& accessName, const std::string& filename);

		void loadTexture(const std::string& filename);





	private:

		void addAnimationStack(AnimationStack* animationStack);
		void addSound(Sound* sound);

		std::map<std::string, Mesh*> m_meshes;

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