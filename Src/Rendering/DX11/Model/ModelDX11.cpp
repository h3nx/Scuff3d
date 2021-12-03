#include "pch.h"
#include "ModelDX11.h"

scuff3d::ModelDX11::ModelDX11(Mesh* mesh, ID3D11Device* device) : ModelData(mesh){
	m_device = device;
	init();
}

scuff3d::ModelDX11::~ModelDX11() {
	safeRelease(m_vertexBuffer);
	safeRelease(m_indexBuffer);
}

void scuff3d::ModelDX11::init() {

	/* VERTICES */
	void* modeldata = m_mesh->getData();
	int count = m_mesh->getCount();
	int size = m_mesh->vertexByteSize();
	m_stride = size;
	m_offset = 0;

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = size * count;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = modeldata;
	auto hr = m_device->CreateBuffer(&bufferDesc, &data, &m_vertexBuffer);

	delete modeldata;


	/* INDICES */

	if (m_mesh->getIndicesCount() > 0) {
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(unsigned int) * m_mesh->getIndicesCount();
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		// Define the resource data.
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = m_mesh->getIndices();
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		HRESULT hr;
		// Create the buffer with the device.
		hr = m_device->CreateBuffer(&bufferDesc, &InitData, &m_indexBuffer);
		if (FAILED(hr))
			m_indexBuffer = nullptr;
	}

	



}

ID3D11Buffer* scuff3d::ModelDX11::getVertBuffer()
{
	return m_vertexBuffer;
}

ID3D11Buffer* scuff3d::ModelDX11::getIndBuffer()
{
	return m_indexBuffer;
}
