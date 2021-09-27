#include "stdafx.h"
#include "IndexBuffer.h"

void IndexBuffer::Create(const std::vector<UINT>& index_vector, const D3D11_USAGE& usage)
{
	//�ε��� ���Ϳ� ������ ���� ���
	if (index_vector.size() == 0)
		return;

	this->stride = sizeof(UINT);
	this->count = static_cast<UINT>(index_vector.size());

	//Index Buffer ����
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
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER; //�ش� Buffer�� Index Buffer�� ���
	desc.ByteWidth = this->stride * this->count; //Index Buffer�� �� ũ��

	//���� �ε��� ������ �����ͷ� ����Ŵ
	D3D11_SUBRESOURCE_DATA sub_data;
	ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));
	sub_data.pSysMem = index_vector.data();

	//Index Buffer ����
	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateBuffer(&desc, &sub_data, m_p_buffer.GetAddressOf());
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return;
}
