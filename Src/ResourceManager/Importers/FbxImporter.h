#pragma once
#pragma warning(push, 0) 
#include <fbxsdk.h>
#pragma warning(pop)

namespace scuff3d {


	class Mesh;
	class Skeleton;
	class AnimationStack;

	class FbxImporter {
		FbxImporter();
		~FbxImporter();

		bool loadFile(const std::string filename, Mesh* mesh, Skeleton* skeleton, AnimationStack* animationStack);
		Mesh* getMesh(fbxsdk::FbxScene* scene);
		Skeleton* getSkeleton(fbxsdk::FbxScene* scene);
		AnimationStack* getAnimationStack(fbxsdk::FbxScene* scene);



	private:
		static fbxsdk::FbxManager* m_manager;
		static fbxsdk::FbxIOSettings* m_ios;


	};
}


