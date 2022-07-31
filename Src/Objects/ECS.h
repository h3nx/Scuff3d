
#include "Objects/Components.h"


namespace scuff3d {

	class GameObject;

	class ECS {
		friend GameObject;
		
	public:
		ECS();
		~ECS();

		GameObject* createObject(const std::string& name, GameObject* parent = nullptr, const glm::vec3& position = { 0.0f, 0.0f, 0.0f });

		GameObject* find(const size_t& id);

		void findAll(const std::string& name, std::vector<GameObject*>& objects);


		void destroy(GameObject* object);
		void destroy(const size_t& id);
		





	protected:
		GameObject* addObject(GameObject* obj);


		void initSystems();

		void frameBegin();

		void updateSystems(const float& dt);
		void updateFixedSystems(const float& dt);
		void renderSystems(const float& dt);

		void frameEnd();









		std::map<size_t, GameObject*> m_gameObjects;
		std::map<int, std::map<size_t, GameObject*>> m_components;

		std::map<size_t, void*> m_systems;


		std::queue<size_t> m_addedGameObjects;
		std::queue<size_t> m_deletedGameObjects;


	private:


		void removeObject(const size_t& id);
		void removeObject(GameObject* object);


		void registerComponent(const int componentID, const size_t& objectID);
		void registerComponent(const int componentID, GameObject* object);



	};




}