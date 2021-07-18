#pragma once
#include <string>
#include <map>
#include <vector>


class Skeleton;
class AnimationStack;
class Mesh;
class Sound;

namespace scuff3d {

	class ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();


		void loadFBX(const std::string& filename);
		void loadFBXAnimation(const std::string& filename, const std::string& stackName);
		void loadObj(const std::string& filename);

		void loadTexture(const std::string& filename);





	private:

		void addMesh(Mesh* mesh);
		void addAnimationStack(AnimationStack* animationStack);
		void addSound(Sound* sound);







	};

}