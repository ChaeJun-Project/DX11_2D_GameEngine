#pragma once

#include "stdafx.h"

class VertexBuffer final
{
public:
	VertexBuffer() = default;
	~VertexBuffer() = default;

	//Vertex Buffer ����
	//���� ����ü�� ���������� ���� �� �����Ƿ� template ��� �Լ��� ����
	template<typename T>
	void Create(const std::vector<T>& vertex_vector, const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT);

private:
	void Clear()
	{
		m_p_buffer.Reset();
		m_p_buffer = nullptr;
	}

public:
	ID3D11Buffer* GetBuffer() const { SAFE_GET_POINTER(m_p_buffer.Get()); }
	const UINT& GetStride() const { return stride; }
	const UINT& GetOffset() const { return offset; }
	const UINT& GetCount() const { return count; }

private:
	ComPtr<ID3D11Buffer> m_p_buffer = nullptr;
	UINT stride = 0; //���� ������ Ÿ���� ũ��
	UINT offset = 0; //�ش� ���� ������ ���� ������
	UINT count = 0;  //������ ����
};

template<typename T>
void VertexBuffer::Create(const std::vector<T>& vertex_vector, const D3D11_USAGE& usage)
{
    //���� ���Ϳ� ������ ���� ���
	if (vertex_vector.size() == 0)
		return;

	if(m_p_buffer)
		Clear();

	stride = sizeof(T);
	count = static_cast<UINT>(vertex_vector.size());

	//Vertex Buffer ����
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = usage;
	//������ ���ٿ� ���õ� flag
	switch (desc.Usage)
	{
		//GPU�� ���ؼ� �а� ���� ������ ����
	case D3D11_USAGE_DEFAULT:

		//GPU�� ���ؼ� �б� ����
		//CPU ���� �Ұ�
		//�ش� �ɼǿ����� �ڿ� ������ �Ұ���
	case D3D11_USAGE_IMMUTABLE:
		desc.CPUAccessFlags = 0; //CPU ���� �Ұ�
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
	desc.ByteWidth = stride * count; //Vertex Buffer�� �� ũ��

	//���� ���� ������ �����ͷ� ����Ŵ
	D3D11_SUBRESOURCE_DATA sub_data;
	ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));
	sub_data.pSysMem = vertex_vector.data();

	//Vertex Buffer ����
	auto hResult = DEVICE->CreateBuffer(&desc, &sub_data, m_p_buffer.GetAddressOf());
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return;
}
