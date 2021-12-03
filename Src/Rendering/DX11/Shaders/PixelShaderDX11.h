#pragma once
#include "Rendering/DX11/Shaders/ShaderDX11.h"

namespace scuff3d {

	class PixelShaderDX11 : public ShaderDX11
	{
	public:
		PixelShaderDX11(ID3D11DeviceChild* shaderData);
		~PixelShaderDX11();

	private:

	};


}
