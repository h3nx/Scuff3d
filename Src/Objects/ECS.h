
#include "Objects/Components.h"


namespace scuff3d {

	class GameObject;
	class System;

	class ECS {
		friend GameObject;
		
	public:
		ECS();
		virtual ~ECS();

		GameObject* createObject(const std::string& name, GameObject* parent = nullptr, const glm::vec3& position = { 0.0f, 0.0f, 0.0f });

		GameObject* find(const size_t& id);

		void findAll(const std::string& name, std::vector<GameObject*>& objects);
		GameObject* findByID(const size_t& id);




		void addSystem(System* system, const int order = 500);

	protected:

		void initSystems();

		void frameBegin();
		virtual void update(const float& dt);
		virtual void fixedUpdate(const float& dt);
		virtual void render(const float& dt);
		void frameEnd();



		
		std::map<size_t, GameObject*> m_gameObjects;
		std::map<int, std::map<size_t, GameObject*>> m_components;

		std::map<size_t, System*> m_systems;
		std::map<int, std::vector<size_t>> m_systemOrder;

		std::queue<size_t> m_addedGameObjects;
		std::queue<size_t> m_deletedGameObjects;


	private:


		void registerComponent(const int componentID, const size_t& objectID);
		void registerComponent(const int componentID, GameObject* object);

		void registerComponentRemoval(const int componentID, GameObject* object);
		void registerObjectRemoval(const size_t& id);

		void requestDestroy(GameObject* obj);
		void requestImmediateDestroy(GameObject* obj);

	};




}