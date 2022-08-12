#include "pch.h"
#include "RenderSystem.h"
#include "Rendering/API/Renderer.h"
#include "Objects/Object.h"
#include "../Components/Model.h"
#include "../Components/Transform.h"

namespace scuff3d {
	scuff3d::RenderSystem::RenderSystem(Renderer* renderer) {
		m_renderer = renderer;
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
