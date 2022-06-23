#pragma once
#include "Objects/Component.h"

namespace scuff3d
{
	class Animator :
		public Component<Animator>
	{
	public:
		Animator() : Component<Animator>() {};
		~Animator() {};

	private:
	};

}



/*

#pragma once
#include "Node.h"
#include "Transition.h"


class StateMachine
{


private:


	std::vector<std::unique_ptr<Node>> m_nodes;
	std::vector<std::unique_ptr<Transition>> m_transitions;

	std::map<std::string, Node*> m_nameToNode;
	std::map<std::string, Transition*> n_nameToTransition;



	std::map<std::string, bool> m_triggers;
	std::map<std::string, bool> m_triggersBool;
	std::map<std::string, int> m_triggersInt;
	std::map<std::string, float> m_triggersFloat;


	std::map<std::string, Transition*> m_connections;

	Node* m_current;
	Node* m_currentTransition;



	void addTrigger(const std::string& trigger						, Transition* transition); // , Transition* transition
	void addTrigger(const std::string& trigger, const bool value		, Transition* transition); // , Node* next
	void addTrigger(const std::string& trigger, const int value		, Transition* transition);
	void addTrigger(const std::string& trigger, const float value	, Transition* transition);




public:
	StateMachine();
	~StateMachine();


	void update(const float dt);

	void setTrigger(const std::string& trigger);
	void setTrigger(const std::string& trigger, bool value);
	void setTrigger(const std::string& trigger, int value);
	void setTrigger(const std::string& trigger, float value);




};






*/

