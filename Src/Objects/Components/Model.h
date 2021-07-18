#pragma once
#include "Objects/Component.h"

namespace scuff3d
{
	class Model :
		public Component<Model>
	{
	public:
		Model() : Component<Model>() {};
		~Model() {};

	private:
	};



}

