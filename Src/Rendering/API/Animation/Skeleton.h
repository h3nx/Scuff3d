#include <vector>
#include <glm/common.hpp>
namespace scuff3d {

	class Skeleton {
	public:
		Skeleton();
		~Skeleton();

		void addBone(const unsigned int uniqueID, const unsigned int parentID, const std::string& name = "");

		struct Bone {
			unsigned int uniqueID = 0;
			unsigned int parentID = 0;
			std::vector<unsigned int> children;
		};


		const unsigned int getUniqueID(const unsigned int index) const;
		const unsigned int getIndexFromUniqueID(const unsigned int uniqueID);

		const std::string printSkeleton() const;

	private:
		std::vector<Bone> m_bones;
		std::map<unsigned int, unsigned int> m_uniqueIDtoIndex;
		std::vector<std::string> m_names;
		std::vector<glm::mat4> m_inverseBindPoseMatrixes;

	};





}





