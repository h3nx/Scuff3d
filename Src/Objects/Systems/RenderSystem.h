#pragma once
#include "../System.h"


namespace scuff3d {

	class Renderer;
	class RenderSystem : public scuff3d::System {
	public:
		RenderSystem(Renderer* renderer);
		~RenderSystem();

		void update(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt);
		void fixedUpdate(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt);
		void render(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt);
		void debugRender(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt);


	private:

		Renderer* m_renderer;

	};



}