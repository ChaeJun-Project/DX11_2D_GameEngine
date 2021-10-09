#pragma once

class ConstantBuffer final : public DX11Obejct
{
public:
	ConstantBuffer() = default;
	~ConstantBuffer() = default;

	//Constant Buffer는 여러 종류가 있기 때문에 멤버함수 템플릿 사용
	template<typename T>
	void Create
	(
		const UINT& buffer_bind_slot, //enum CBuffer_BindSlot
		const D3D11_USAGE& usage = D3D11_USAGE_DYNAMIC
	);

	void SetConstantBufferData(const void* buffer_data, const UINT& buffer_size);

	void BindPipeline();

public:
    const UINT& GetBufferBindSlot() { return this->m_buffer_bind_slot; }

	const UINT& GetBufferBindStage() { return this->m_buffer_bind_stage; }
	void SetBufferBindStage(const PipelineStage& buffer_bind_stage) { this->m_buffer_bind_stage = buffer_bind_stage; }

private:
	ComPtr<ID3D11Buffer> m_p_buffer = nullptr;
	UINT m_buffer_bind_stage;
	UINT m_buffer_bind_slot;
	UINT m_buffer_size;
};

template<typename T>
inline void ConstantBuffer::Create(const UINT& buffer_bind_slot, const D3D11_USAGE& usage)
{
    //Constant Buffer 바인드 슬롯 설정
    this->m_buffer_bind_slot = buffer_bind_slot;

    //Constant Buffer 정의
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
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; //해당 Buffer를 Constant Buffer로 사용
	desc.ByteWidth = this->m_buffer_size = sizeof(T); //Constant Buffer의 총 크기

	//Constant Buffer 생성
	//Constant Buffer는 다른 buffer들과 다르게 D3D11_SUBRESOURCE_DATA가 필요하지않음
	//고정되어 있는 실제 데이터가 아니고 계속 변경되는 값이기 때문
	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateBuffer(&desc, nullptr, this->m_p_buffer.GetAddressOf());
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return;
}