#pragma once
#include "pch.h"
#include "FbxImporter.h"
#include "Rendering/API/Geometry/Mesh.h"


namespace scuff3d {

	//transform ripped from https ://github.com/Caspila/GUInity/blob/master/Source/Converter.cpp
	static void FBXtoGLM(glm::mat4& newMat, const FbxAMatrix& mat) {
		newMat[0][0] = (float)mat[0][0];
		newMat[0][1] = (float)mat[0][1];
		newMat[0][2] = (float)mat[0][2];
		newMat[0][3] = (float)mat[0][3];

		newMat[1][0] = (float)mat[1][0];
		newMat[1][1] = (float)mat[1][1];
		newMat[1][2] = (float)mat[1][2];
		newMat[1][3] = (float)mat[1][3];

		newMat[2][0] = (float)mat[2][0];
		newMat[2][1] = (float)mat[2][1];
		newMat[2][2] = (float)mat[2][2];
		newMat[2][3] = (float)mat[2][3];

		newMat[3][0] = (float)mat[3][0];
		newMat[3][1] = (float)mat[3][1];
		newMat[3][2] = (float)mat[3][2];
		newMat[3][3] = (float)mat[3][3];
	}
	static void FBXtoGLM(glm::vec3& newVec, const FbxVector4& vec) {
		newVec.x = (float)vec[0];
		newVec.y = (float)vec[1];
		newVec.z = (float)vec[2];
	}
	static void FBXtoGLM(glm::vec2& newVec, const FbxVector2& vec) {
		newVec.x = (float)vec[0];
		newVec.y = (float)vec[1];
	}

	FbxImporter::FbxImporter() {
		m_manager = FbxManager::Create();
		m_ios = FbxIOSettings::Create(m_manager, IOSROOT);
		m_manager->SetIOSettings(m_ios);
	}

	FbxImporter::~FbxImporter() {
		m_manager->Destroy();
	}

	bool FbxImporter::loadFile(const std::string filename, Mesh*& mesh, Skeleton*& skeleton, AnimationStack*& animationStack) {
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

		std::string name = filename.substr(filename.find_last_of("/") + 1, std::string::npos);

		FbxNode* root = scene->GetRootNode();
		//printNodeTree(root);
		std::vector<fbxsdk::FbxNode*> nodes;
		traverseNodesForMesh(root, nodes);


		std::vector<std::vector<unsigned long>> cpToVertMap;
		mesh = getMesh(name, nodes[0], cpToVertMap);
		if (!mesh)
			return false;
		//skeleton = getSkeleton(scene);
		//if(skeleton)
			//animationStack = getAnimationStack(scene);


		return true;
	}

	Mesh* FbxImporter::getMesh(const std::string& name, fbxsdk::FbxNode* node, std::vector<std::vector<unsigned long>>& cpToVertMap) {
		FbxMesh* mesh = node->GetMesh();

		const unsigned int numVertices = mesh->GetPolygonVertexCount();
		int* indicesPolygons = mesh->GetPolygonVertices();
		unsigned int uniqueVertices = 0;

		std::vector<unsigned int> indices; indices.reserve(numVertices);
		std::vector<glm::vec3> positions; positions.reserve(numVertices);
		std::vector<glm::vec3> normals; normals.reserve(numVertices);
		std::vector<glm::vec3> tangents; tangents.reserve(numVertices);
		std::vector<glm::vec3> biTangents; biTangents.reserve(numVertices);
		std::vector<glm::vec2> uvs; uvs.reserve(numVertices);
		std::vector<glm::vec4> colors; colors.reserve(numVertices);

		FbxVector4* cp = mesh->GetControlPoints();
		FbxLayerElementNormal* normalList = mesh->GetLayer(0)->GetNormals();
		FbxGeometryElementTangent* tangentList = mesh->GetElementTangent(0);
		FbxGeometryElementBinormal* biTangentList = mesh->GetElementBinormal(0);
		FbxGeometryElementUV* uvList = mesh->GetElementUV(0);

		cpToVertMap.resize(mesh->GetControlPointsCount());

	/*	glm::vec3 vertPosition[3] = { { 0,0,0 },{ 0,0,0 },{ 0,0,0 } };
		glm::vec3 vertNormal[3] = { { 0,0,0 },{ 0,0,0 },{ 0,0,0 } };
		glm::vec3 vertTangent[3] = { { 0,0,0 },{ 0,0,0 },{ 0,0,0 } };
		glm::vec3 vertBitangent[3] = { { 0,0,0 },{ 0,0,0 },{ 0,0,0 } };
		glm::vec2 vertTexCoord[3] = { { 0,0 },{ 0,0 } };*/
		glm::vec3 vertPosition	 = {0,0,0};
		glm::vec3 vertNormal	 = {0,0,0};
		glm::vec3 vertTangent	 = {0,0,0};
		glm::vec3 vertBitangent	 = {0,0,0};
		glm::vec2 vertUv	 = {0,0};
		glm::vec4 vertColor		 = {0,0,0,0};


		for (unsigned int vertexIndex = 0; vertexIndex < numVertices; vertexIndex++) {
			FBXtoGLM(vertPosition, cp[indicesPolygons[vertexIndex]]);


			if (normalList) {
				if (normalList->GetReferenceMode() == FbxLayerElement::eDirect) {
					FBXtoGLM(vertNormal, normalList->GetDirectArray()[vertexIndex]);
				}
				if (normalList->GetReferenceMode() == FbxLayerElement::eIndexToDirect) {
					FBXtoGLM(vertNormal, normalList->GetDirectArray()[normalList->GetIndexArray()[vertexIndex]]);
				}
			}


			int spot = -1;
			for (int i = positions.size() - 1; i >= 0; i--) {

				if (positions[i] == vertPosition) {
					if(normalList && normals[i] == vertNormal) {
						//if (uvList && uvs[i] == vertUv) {
						spot = i;
						break;
					}
			}
			}
			if (spot > -1) {
				indices.push_back(spot);
			}
			else {
				indices.push_back(positions.size());
				positions.push_back(vertPosition);
				colors.push_back({ vertNormal.x, vertNormal.y, vertNormal.z, 1 });
				if (normalList)		normals.push_back(vertNormal);
				//if (tangentList)	tangents.push_back(vertTangent);
				//if (biTangentList)	biTangents.push_back(vertBitangent);
				//if (uvList)			uvs.push_back(vertUv);
			}

		}


		//for (unsigned int polyIndex = 0; polyIndex < (unsigned int)mesh->GetPolygonCount(); polyIndex++) {
		//	unsigned int numVertices = mesh->GetPolygonSize(polyIndex);
		//	for (unsigned int vertIndex = 0; vertIndex < numVertices; vertIndex++) {
		//		unsigned int cpIndex = (unsigned int)indicesPolygons[vertIndex];

		//		FBXtoGLM(vertPosition, cp[indicesPolygons[vertIndex]]);

		//		if (normalList) {
		//			if (normalList->GetReferenceMode() == FbxLayerElement::eDirect) {
		//				FBXtoGLM(vertNormal, normalList->GetDirectArray()[vertIndex]);
		//			}
		//			if (normalList->GetReferenceMode() == FbxLayerElement::eIndexToDirect) {
		//				FBXtoGLM(vertNormal, normalList->GetDirectArray()[normalList->GetIndexArray()[vertIndex]]);
		//			}
		//		}
		//		if (tangentList) {
		//			if (tangentList->GetReferenceMode() == FbxLayerElement::eDirect) {
		//				FBXtoGLM(vertNormal, tangentList->GetDirectArray()[vertIndex]);
		//			}
		//			if (tangentList->GetReferenceMode() == FbxLayerElement::eIndexToDirect) {
		//				FBXtoGLM(vertTangent, tangentList->GetDirectArray()[tangentList->GetIndexArray()[vertIndex]]);
		//			}
		//		}
		//		if (biTangentList) {
		//			if (biTangentList->GetReferenceMode() == FbxLayerElement::eDirect) {
		//				FBXtoGLM(vertBitangent, biTangentList->GetDirectArray()[vertIndex]);
		//			}
		//			if (biTangentList->GetReferenceMode() == FbxLayerElement::eIndexToDirect) {
		//				FBXtoGLM(vertBitangent, biTangentList->GetDirectArray()[biTangentList->GetIndexArray()[vertIndex]]);
		//			}
		//		}
		//		if (uvList) {
		//			//texCoord = getTexCoord(cpIndex, geUV, mesh, polyIndex, vertIndex);
		//			//unsigned int cpIndex = mesh->GetPolygonVertex(polyIndex, vertIndex);
		//			if (uvList->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
		//				if (uvList->GetReferenceMode() == FbxLayerElement::eDirect) {
		//					FBXtoGLM(vertUv, uvList->GetDirectArray()[vertIndex]);
		//				}
		//				if (uvList->GetReferenceMode() == FbxLayerElement::eIndexToDirect) {
		//					FBXtoGLM(vertUv, uvList->GetDirectArray()[uvList->GetIndexArray().GetAt(vertIndex)]);
		//				}
		//			}
		//			else {
		//				if (uvList->GetReferenceMode() == FbxLayerElement::eIndexToDirect) {
		//					FBXtoGLM(vertUv, uvList->GetDirectArray()[mesh->GetTextureUVIndex(polyIndex, vertIndex)]);
		//				}
		//			}
		//		}
			/*	unsigned int spot = -1;
				for (int i = positions.size() - 1; i >= 0; i--) {
					
					if (positions[i] == vertPosition)
						if (normalList && normals[i] == vertNormal)
							if (uvList && uvs[i] == vertUv) {
								spot = i;
								break;
							}
				}
				if (spot > -1) {
					indices.push_back(spot);
				}
				else {
					indices.push_back(positions.size());
					positions.push_back(vertPosition);
					if (normalList)		normals.push_back(vertNormal);
					if (tangentList)	tangents.push_back(vertTangent);
					if (biTangentList)	biTangents.push_back(vertBitangent);
					if (uvList)			uvs.push_back(vertUv);
				}*/

		//		//if (normalList)
		//		//	if (normals[i] == vertNormal)
		//		//		if (uvList)
		//		//			if (uvs[i] == vertUv)
		//		//				unique = false;


		//	}
		//}
		



		
		return new Mesh(
			name,
			positions.size(),
			positions.data(),
			normalList ? normals.data() : nullptr,
			colors.data(),
			/*tangentList ? tangents.data() : nullptr,
			biTangentList ? biTangents.data() : nullptr,
			uvList ? uvs.data() : nullptr,*/
			indices.data(),
			indices.size()
		);
	}
	void FbxImporter::traverseNodesForMesh(fbxsdk::FbxNode* node, std::vector<fbxsdk::FbxNode*>& nodes) {
		unsigned int numAttributes = (unsigned int)node->GetNodeAttributeCount();
		for (unsigned int attributeIndex = 0; attributeIndex < numAttributes; attributeIndex++) {

			FbxNodeAttribute* nodeAttribute = node->GetNodeAttributeByIndex(attributeIndex);
			FbxNodeAttribute::EType attributeType = nodeAttribute->GetAttributeType();

			if (attributeType == FbxNodeAttribute::eMesh) {
				nodes.push_back(node);
			}
		}
		for (int i = 0; i < node->GetChildCount(); i++) {
			traverseNodesForMesh(node->GetChild(i), nodes);
		}
	}

	Skeleton* FbxImporter::getSkeleton(fbxsdk::FbxScene* scene) {





		return nullptr;
	}
	void FbxImporter::traverseNodesForSkeleton(fbxsdk::FbxNode* node, Skeleton* skeleton) {
		if (node->GetNodeAttribute() &&
			node->GetNodeAttribute()->GetAttributeType() &&
			node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton) {

			//AnimationStack::Bone limb;
		/*	limb.parentIndex = inParentIndex;
			limb.uniqueID = inNode->GetUniqueID();
			limb.name = inNode->GetName();

			if (stack->boneCount() > 0) {
				stack->getBone(limb.parentIndex).childIndexes.emplace_back(stack->boneCount());
			}
			stack->addBone(limb);

		}
		for (int i = 0; i < inNode->GetChildCount(); i++) {
			traverseNodesForSkeleton(inNode->GetChild(i), stack->boneCount(), myIndex, stack);
		}*/
		}
	}

	AnimationStack* FbxImporter::getAnimationStack(fbxsdk::FbxScene* scene) {
		return nullptr;
	}



	void FbxImporter::collectMeshData(fbxsdk::FbxNode* node) {
	}


#pragma region _DEBUG
	//DEBUG
	
	std::string FbxImporter::GetAttributeTypeName(FbxNodeAttribute::EType type) {
		switch (type) {
		case FbxNodeAttribute::eUnknown: return "unidentified";
		case FbxNodeAttribute::eNull: return "null";
		case FbxNodeAttribute::eMarker: return "marker";
		case FbxNodeAttribute::eSkeleton: return "skeleton";
		case FbxNodeAttribute::eMesh: return "mesh";
		case FbxNodeAttribute::eNurbs: return "nurbs";
		case FbxNodeAttribute::ePatch: return "patch";
		case FbxNodeAttribute::eCamera: return "camera";
		case FbxNodeAttribute::eCameraStereo: return "stereo";
		case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
		case FbxNodeAttribute::eLight: return "light";
		case FbxNodeAttribute::eOpticalReference: return "optical reference";
		case FbxNodeAttribute::eOpticalMarker: return "marker";
		case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
		case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
		case FbxNodeAttribute::eBoundary: return "boundary";
		case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
		case FbxNodeAttribute::eShape: return "shape";
		case FbxNodeAttribute::eLODGroup: return "lodgroup";
		case FbxNodeAttribute::eSubDiv: return "subdiv";
		default: return "unknown";
		}
	}
	std::string FbxImporter::PrintAttribute(FbxNodeAttribute * pAttribute) {
		if (!pAttribute) {
			return "x";
		}

		std::string typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
		std::string attrName = pAttribute->GetName();

		return "(" + typeName + " " + attrName + ")";
	}
	void FbxImporter::printNodeTree(FbxNode * node, const std::string & indent) {
		std::string name = node->GetName();
		std::string attributes = "";

		for (int i = 0; i < node->GetNodeAttributeCount(); i++) {
			attributes += ": " + PrintAttribute(node->GetNodeAttributeByIndex(i));
		}

		DEVLOG(indent + name + ":" + attributes);
		for (int i = 0; i < node->GetChildCount(); i++) {
			printNodeTree(node->GetChild(i), indent + "|");
		}
	}
	/*
	void FbxImporter::printAnimationStack(const FbxNode * node) {
	}

	unsigned int FbxImporter::getByteSize() const {
		unsigned int size = 0;

		size += sizeof(*this);

		size += sizeof(FbxManager);
		size += sizeof(FbxIOSettings);

		size += sizeof(std::pair<std::string, FbxScene*>) * m_scenes.size();
		for (auto& [key, val] : m_scenes) {
			size += sizeof(unsigned char) * key.capacity();
			size += sizeof(FbxScene);
			// TODO: Maybe investigate FbxScene further
		}

		size += sizeof(std::pair<std::string, SceneData>) * m_sceneData.size();
		for (auto& [key, val] : m_sceneData) {
			size += sizeof(unsigned char) * key.capacity();

			if (val.model) {
				size += val.model->getByteSize();
			}
			if (val.stack) {
				size += val.stack->getByteSize();
			}

			size += sizeof(std::vector<unsigned long>) * val.cpToVertMap.capacity();
			for (auto& vec : val.cpToVertMap) {
				size += sizeof(unsigned long) * vec.capacity();
			}
		}

		return size;
	}
	*/
#pragma endregion































}


