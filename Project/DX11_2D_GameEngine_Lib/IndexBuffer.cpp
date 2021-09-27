#include "stdafx.h"
#include "IndexBuffer.h"

void IndexBuffer::Create(const std::vector<UINT>& index_vector, const D3D11_USAGE& usage)
{
	//인덱스 벡터에 정보가 없는 경우
	if (index_vector.size() == 0)
		return;

	this->stride = sizeof(UINT);
	this->count = static_cast<UINT>(index_vector.size());

	//Index Buffer 정의
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = usage;
	//버퍼의 접근에 관련된 flag
	switch (desc.Usage)
	{
		//GPU에 의해서 읽고 쓰기 접근이 가능
		//CPU 접근 불가
	case D3D11_USAGE_DEFAULT:

		//GPU에 의해서 읽기 가능
		//CPU 접근 불가
		//해당 옵션에서는 자원 수정이 불가능
	case D3D11_USAGE_IMMUTABLE:
		desc.CPUAccessFlags = 0;
		break;

		//GPU에 의해서 읽기 가능
		//CPU에서 쓰기 가능
		//CPU에서 자원을 읽지는 못하지만 수정은 가능
		//보통 ConstantBuffer를 만들 때 사용
	case D3D11_USAGE_DYNAMIC:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;

		//GPU 메모리에서 CPU 메모리로 복사 허용
	case D3D11_USAGE_STAGING:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		break;
	}
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER; //해당 Buffer를 Index Buffer로 사용
	desc.ByteWidth = this->stride * this->count; //Index Buffer의 총 크기

	//실제 인덱스 정보를 포인터로 가리킴
	D3D11_SUBRESOURCE_DATA sub_data;
	ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));
	sub_data.pSysMem = index_vector.data();

	//Index Buffer 생성
	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateBuffer(&desc, &sub_data, m_p_buffer.GetAddressOf());
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return;
}
