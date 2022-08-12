#pragma once
#include "../System.h"

namespace scuff3d {

	class BasicPhysicsSystem : public System {
	public:
		BasicPhysicsSystem();
		~BasicPhysicsSystem();

		void update(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt);
		void fixedUpdate(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt);
		void render(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt);
		void debugRender(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt);


	private:


	};
}

