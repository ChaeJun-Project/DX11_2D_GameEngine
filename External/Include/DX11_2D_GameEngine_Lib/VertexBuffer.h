#pragma once

#include "stdafx.h"

class VertexBuffer final : public DX11Obejct
{
public:
	VertexBuffer() = default;
	~VertexBuffer() = default;

	//Vertex Buffer ����
	//���� ����ü�� ���������� ���� �� �����Ƿ� template ��� �Լ��� ����
	template<typename T>
	void Create(const std::vector<T>& vertex_vector, const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT);

public:
	ID3D11Buffer* GetResource() const { SAFE_GET_POINTER(this->m_p_buffer.Get()); }
	const UINT& GetStride() const { return this->stride; }
	const UINT& GetOffset() const { return this->offset; }
	const UINT& GetCount() const { return this->count; }

private:
	ComPtr<ID3D11Buffer> m_p_buffer = nullptr;
	UINT stride = 0; //���� ������ Ÿ���� ũ��
	UINT offset = 0; //�ش� ���� ������ ���� ������
	UINT count = 0;  //������ ����
};

template<typename T>
inline void VertexBuffer::Create(const std::vector<T>& vertex_vector, const D3D11_USAGE& usage)
{
    //���� ���Ϳ� ������ ���� ���
	if (vertex_vector.size() == 0)
		return;

	this->stride = sizeof(T);
	this->count = static_cast<UINT>(vertex_vector.size());

	//Vertex Buffer ����
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = usage;
	//������ ���ٿ� ���õ� flag
	switch (desc.Usage)
	{
		//GPU�� ���ؼ� �а� ���� ������ ����
		//CPU ���� �Ұ�
	case D3D11_USAGE_DEFAULT:

		//GPU�� ���ؼ� �б� ����
		//CPU ���� �Ұ�
		//�ش� �ɼǿ����� �ڿ� ������ �Ұ���
	case D3D11_USAGE_IMMUTABLE:
		desc.CPUAccessFlags = 0;
		break;

		//GPU�� ���ؼ� �б� ����
		//CPU���� ���� ����
		//CPU���� �ڿ��� ������ �������� ������ ����
		//���� ConstantBuffer�� ���� �� ���
	case D3D11_USAGE_DYNAMIC:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;

		//GPU �޸𸮿��� CPU �޸𸮷� ���� ���
	case D3D11_USAGE_STAGING:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		break;
	}
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //�ش� Buffer�� Vertex Buffer�� ���
	desc.ByteWidth = this->stride * this->count; //Vertex Buffer�� �� ũ��

	//���� ���� ������ �����ͷ� ����Ŵ
	D3D11_SUBRESOURCE_DATA sub_data;
	ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));
	sub_data.pSysMem = vertex_vector.data();

	//Vertex Buffer ����
	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateBuffer(&desc, &sub_data, this->m_p_buffer.GetAddressOf());
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return;
}
