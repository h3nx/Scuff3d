#pragma once
#include "Rendering/DX11/Shaders/ShaderDX11.h"

namespace scuff3d {

	class VertexShaderDX11 : public ShaderDX11
	{
	public:
		VertexShaderDX11(ID3D11DeviceChild* shaderData);
		~VertexShaderDX11();

	private:

	};


}

