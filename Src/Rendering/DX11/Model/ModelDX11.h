#pragma once
#include "Rendering/API/Geometry/ModelData.h"
#include <d3d11.h>

namespace scuff3d {

	class ModelDX11 : public ModelData
	{
	public:
		ModelDX11(Mesh*mesh, ID3D11Device* device);
		~ModelDX11();
		void init();

		ID3D11Buffer* getVertBuffer();
		ID3D11Buffer* getIndBuffer();

	private:
		ID3D11Buffer* m_vertexBuffer;
		ID3D11Buffer* m_indexBuffer;
		ID3D11Device* m_device;
	};


}


