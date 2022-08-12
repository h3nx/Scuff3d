#include "pch.h"
#include "System.h"


namespace scuff3d {
	size_t systemIdCounter = 0;


	System::System() 
		: m_id(systemIdCounter++)
	{
	}
	System::~System() {
	}
	const size_t System::getID() const {
		return m_id;
	}
}







