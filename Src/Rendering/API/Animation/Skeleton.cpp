#include "pch.h"
#include "Skeleton.h"
#include <stack>

namespace scuff3d {
	Skeleton::Skeleton() {
		
	}

	Skeleton::~Skeleton() {

	}

	void Skeleton::addBone(const unsigned int uniqueID, const unsigned int parentID, const std::string& name) {
		m_uniqueIDtoIndex[uniqueID] = m_bones.size();
		m_bones.push_back({ uniqueID, parentID, std::vector<unsigned int>() });
		m_names.emplace_back(name);
	}

	const unsigned int Skeleton::getUniqueID(const unsigned int index) const {
		return m_bones[index].uniqueID;
	}

	const unsigned int Skeleton::getIndexFromUniqueID(const unsigned int uniqueID) {
		return m_uniqueIDtoIndex[uniqueID];
	}

	const std::string Skeleton::printSkeleton() const {
		std::string output = "";
		for (size_t i = 0; i < m_bones.size(); i++) {
			output += std::to_string(i) + "(" + std::to_string(m_bones[i].uniqueID) + ")" + m_names[i];
		}
		return output;
	}








}