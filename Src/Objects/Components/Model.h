#pragma once
#include "Objects/Component.h"
#include "Rendering/API/Geometry/ModelData.h"

namespace scuff3d
{
	class Model :
		public Component<Model>
	{
	public:
		Model() : Component<Model>(), m_data(nullptr) {};
		Model(ModelData* data) : Component<Model>(), m_data(data) {};
		~Model() {};
		ModelData* getData() { return m_data; };

	private:
		ModelData* m_data;

	};



}

