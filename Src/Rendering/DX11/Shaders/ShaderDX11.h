#pragma once
#include "Rendering/API/Shader/Shader.h"
#include <d3d11.h>

namespace scuff3d {

	class ShaderDX11 : public Shader {
	public:
		ShaderDX11(ID3D11DeviceChild* shaderData) { m_shaderData = shaderData; };
		~ShaderDX11() {};
		ID3D11DeviceChild* getData() { return m_shaderData; }
	protected:
		ID3D11DeviceChild* m_shaderData;
	};


}

