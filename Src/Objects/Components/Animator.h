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

