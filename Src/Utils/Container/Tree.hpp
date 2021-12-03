#pragma once
#include <unordered_map>


namespace scuff3d {


	template<class T>
	class Tree {
	public:
		template <class T>
		class Node {
		public:
			friend Tree<T>;
			Node(Node<T>* parent) { m_parent = parent; };
			Node(Node<T>* parent, const T& initialValue) : Node<T>(parent) { m_value = initialValue; };
			~Node() { 
				for (Node<T>* node : m_children) {
					if (node)
						delete node;
				}
				m_children.clear();
			};

		private:
			T m_value;
			std::vector<Node<T>*> m_children;
			Node<T>* m_parent;
		
		};
		Tree() { };
		Tree(const T& rootValue) {

		}
		~Tree() {

		}
		
		
		Node<T>* push(const T& value) {
			//m_current->m_children.push_back(new Node<T)
			return nullptr;
		};
		


	private:
		Node<T>* m_root;
		Node<T>* m_current;
	};


}
