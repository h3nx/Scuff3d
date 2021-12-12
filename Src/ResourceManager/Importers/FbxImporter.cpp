#pragma once
#include "pch.h"
#include "FbxImporter.h"


namespace scuff3d {

	FbxImporter::FbxImporter() {
		if(m_manager == nullptr)
		m_manager = FbxManager::Create();
		m_ios = FbxIOSettings::Create(m_manager, IOSROOT);
		m_manager->SetIOSettings(m_ios);
	}

	FbxImporter::~FbxImporter() {
		m_manager->Destroy();
	}

	bool FbxImporter::loadFile(const std::string filename, Mesh* mesh, Skeleton* skeleton, AnimationStack* animationStack) {
		fbxsdk::FbxImporter* importer = fbxsdk::FbxImporter::Create(m_manager, "");
		if (!importer->Initialize(filename.c_str(), -1, m_manager->GetIOSettings())) {
			importer->Destroy();
			DEVLOGERROR("NO IMPORT (" + filename + ")");
			return false;
		}
		FbxScene* scene = FbxScene::Create(m_manager, filename.c_str());
		importer->Import(scene);

		if (!scene) {
			return false;
		}
		FbxGeometryConverter geoConverter(m_manager);
		if (!geoConverter.Triangulate(scene, true)) {
			assert(scene);
		}



		return false;
	}

	Mesh* FbxImporter::getMesh(fbxsdk::FbxScene* scene) {
		return nullptr;
	}

	Skeleton* FbxImporter::getSkeleton(fbxsdk::FbxScene* scene) {
		return nullptr;
	}

	AnimationStack* FbxImporter::getAnimationStack(fbxsdk::FbxScene* scene) {
		return nullptr;
	}

}


