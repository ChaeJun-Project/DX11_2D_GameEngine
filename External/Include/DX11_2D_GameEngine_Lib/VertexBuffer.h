#pragma once

#include "stdafx.h"

class VertexBuffer final : public DX11Obejct
{
public:
	VertexBuffer() = default;
	~VertexBuffer() = default;

	//Vertex Buffer 생성
	//정점 구조체는 여러가지가 사용될 수 있으므로 template 멤버 함수로 정의
	template<typename T>
	void Create(const std::vector<T>& vertex_vector, const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT);

public:
	ID3D11Buffer* GetResource() const { SAFE_GET_POINTER(this->m_p_buffer.Get()); }
	const UINT& GetStride() const { return this->stride; }
	const UINT& GetOffset() const { return this->offset; }
	const UINT& GetCount() const { return this->count; }

private:
	ComPtr<ID3D11Buffer> m_p_buffer = nullptr;
	UINT stride = 0; //정점 데이터 타입의 크기
	UINT offset = 0; //해당 정점 버퍼의 시작 오프셋
	UINT count = 0;  //정점의 개수
};

template<typename T>
inline void VertexBuffer::Create(const std::vector<T>& vertex_vector, const D3D11_USAGE& usage)
{
    //정점 벡터에 정보가 없는 경우
	if (vertex_vector.size() == 0)
		return;

	this->stride = sizeof(T);
	this->count = static_cast<UINT>(vertex_vector.size());

	//Vertex Buffer 정의
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
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //해당 Buffer를 Vertex Buffer로 사용
	desc.ByteWidth = this->stride * this->count; //Vertex Buffer의 총 크기

	//실제 정점 정보를 포인터로 가리킴
	D3D11_SUBRESOURCE_DATA sub_data;
	ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));
	sub_data.pSysMem = vertex_vector.data();

	//Vertex Buffer 생성
	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateBuffer(&desc, &sub_data, this->m_p_buffer.GetAddressOf());
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return;
}
