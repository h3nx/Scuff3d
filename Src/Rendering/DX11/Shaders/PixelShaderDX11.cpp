#include "pch.h"
#include "PixelShaderDX11.h"


namespace scuff3d {






	PixelShaderDX11::PixelShaderDX11(ID3D11DeviceChild* shaderData) :
		ShaderDX11(shaderData)
	{ }

	PixelShaderDX11::~PixelShaderDX11()
	{ }

}