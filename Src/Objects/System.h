#pragma once





namespace scuff3d {
	class GameObject;
	class System {
	public:
		System();
		virtual ~System();

		virtual void update(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt) {};
		virtual void fixedUpdate(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt) {};
		virtual void render(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt) {};
		virtual void debugRender(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt) {};

		const size_t getID() const;

	protected:
		const size_t m_id;


	};





}






/*


#include "../System.h"


namespace scuff3d {

	class RenderSystem  : public System{
	public:
		RenderSystem();
		~RenderSystem();

		void update(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt);
		void fixedUpdate(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt);
		void render(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt);
		void debugRender(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt);


	private:


	};
}




#include "pch.h"
#include "RenderSystem.h"
#include "../Components/Model.h"
#include "../Components/Transform.h"
#include "Objects/Object.h"

namespace scuff3d {
	scuff3d::RenderSystem::RenderSystem() {
		
	}

	RenderSystem::~RenderSystem() {
	}

	void RenderSystem::update(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt) {

	}

	void RenderSystem::fixedUpdate(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt) {

	}

	void RenderSystem::render(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt) {
		std::map<size_t, GameObject*>& toRender = objects[Model::ID];

		for (auto& obj : toRender) {
			Transform* transform = obj.second->getComponent<Transform>();
			Model* model = obj.second->getComponent<Model>();
			ModelData* modelData = model->getData();
			m_renderer->renderSolid(modelData, transform->getRenderMatrix());
		}
	}

	void RenderSystem::debugRender(std::map<int, std::map<size_t, GameObject*>>& objects, const float& dt) {

	}

}








}








*/









