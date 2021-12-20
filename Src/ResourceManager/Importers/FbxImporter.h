#pragma once
//#pragma warning(push, 0) 
#include <fbxsdk.h>
//#pragma warning(pop)

namespace scuff3d {


	class Mesh;
	class Skeleton;
	class AnimationStack;

	class FbxImporter {
	public:
		FbxImporter();
		~FbxImporter();

		bool loadFile(const std::string filename, Mesh*& mesh, Skeleton*& skeleton, AnimationStack*& animationStack);
		Mesh* getMesh(const std::string& name, fbxsdk::FbxNode* node, std::vector<std::vector<unsigned long>>& cpToVertMap);
		Skeleton* getSkeleton(fbxsdk::FbxScene* scene);
		AnimationStack* getAnimationStack(fbxsdk::FbxScene* scene);


		void traverseNodesForMesh(fbxsdk::FbxNode* node, std::vector<fbxsdk::FbxNode*>& nodes);
		void collectMeshData(fbxsdk::FbxNode* node);
		void traverseNodesForSkeleton(fbxsdk::FbxNode* node, Skeleton* skeleton);


	private:
		fbxsdk::FbxManager* m_manager;
		fbxsdk::FbxIOSettings* m_ios;
		//DEBUG
		std::string GetAttributeTypeName(FbxNodeAttribute::EType type);
		std::string PrintAttribute(FbxNodeAttribute* pAttribute);
		void printNodeTree(FbxNode* node, const std::string& indent = "\t");
		void printAnimationStack(const FbxNode* node);

	};
}


