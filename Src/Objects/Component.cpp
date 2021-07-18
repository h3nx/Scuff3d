#include "pch.h"
#include "Component.h"


namespace scuff3d
{
	static int componentID;

	const int ComponentBase::createID() {
		return componentID++;
	}

	void ComponentBase::init() {
	}
	void ComponentBase::destroy() {
	}
	void ComponentBase::preUpdate(const float& dt) {
	}
	void ComponentBase::update(const float& dt) {
	}
	void ComponentBase::postUpdate(const float& dt) {
	}
	void ComponentBase::preFixedUpdate(const float& dt) {
	}
	void ComponentBase::fixedUpdate(const float& dt) {
	}
	void ComponentBase::postFixedUpdate(const float& dt) {
	}
	const bool ComponentBase::isActive() const {
		return m_active;
	}
	const void ComponentBase::setActive(const bool status) {
		m_active = status;
	}
	void ComponentBase::setObject(GameObject* obj) {
		m_obj = obj;
	}
}





