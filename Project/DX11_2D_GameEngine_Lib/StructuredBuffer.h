#pragma once

//구조화 버퍼
class StructuredBuffer final
{
public:
	StructuredBuffer() = default;
	~StructuredBuffer() = default;

	//Structured Buffer는 여러 종류가 있기 때문에 멤버함수 템플릿 사용
	template<typename T>
	void Create
	(
		const UINT& element_count,
		const SBufferType& structured_buffer_type,
		const bool& is_cpu_access,
		void* p_data = nullptr
	);

	void SetStructuredBufferData(const void* buffer_data, const UINT& buffer_size);
	void GetStructuredBufferData(void* buffer_data, const UINT& buffer_size);

	void BindPipeline();
	void BindPipelineRW(const UINT& unordered_bind_slot);

	void Clear();
	void ClearRW();

private:
	void CreateSRV();
	void CreateUAV();

public:
	UINT GetBufferBindSlot() const { return m_buffer_bind_slot; }
	UINT GetBufferBindStage() const { return m_buffer_bind_stage; }

	void SetBufferBindStage(const UINT& buffer_bind_stage) { m_buffer_bind_stage = buffer_bind_stage; }
	void SetBufferBindSlot(const UINT& buffer_bind_slot) { m_buffer_bind_slot = buffer_bind_slot; }

	UINT GetElementCount() const { return m_element_count; }

private:
	//=============================================
	//GPU Read_Only or Read_Write StructuredBuffer
	//=============================================
	ComPtr<ID3D11Buffer> m_p_buffer = nullptr;

	ComPtr<ID3D11ShaderResourceView> m_p_shader_resource_view = nullptr; //Read_Only
	ComPtr<ID3D11UnorderedAccessView> m_p_unordered_access_view = nullptr; //Read_Write

	SBufferType m_sbuffer_type;

	//=============================================
    //CPU StructuredBuffer
	//=============================================
	ComPtr<ID3D11Buffer> m_p_cpu_read_buffer = nullptr;
	ComPtr<ID3D11Buffer> m_p_cpu_write_buffer = nullptr;
	bool is_cpu_access = false;

	UINT m_element_size = 0;
	UINT m_element_count = 0;
	UINT m_buffer_size = 0;	  //m_element_size * m_element_count

	UINT m_buffer_bind_stage = 0;   //Bind Stage
	UINT m_buffer_bind_slot = 0;	//t(texture) register 바인드 슬롯 번호
	UINT m_unordered_bind_slot = 0; //u(unordered) register 바인드 슬롯 번호
};

template<typename T>
void StructuredBuffer::Create(const UINT& element_count, const SBufferType& structured_buffer_type, const bool& is_cpu_access, void* p_data)
{
	m_element_size = sizeof(T);					    //구조적 버퍼 요소당 사이즈
	m_element_count = element_count;			    //구조적 버퍼 요소 개수
	m_buffer_size = m_element_size * element_count; //구조적 버퍼 총 크기
	m_sbuffer_type = structured_buffer_type;		//구조적 버퍼 접근 정도

	//=============================================
	//구조적 버퍼 정의
	//=============================================
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = m_buffer_size;

	switch (m_sbuffer_type)
	{
	case SBufferType::Read_Only: //읽기 전용
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; //SRV 자원 뷰를 통해 파이프 라인에 연결
		break;
	case SBufferType::Read_Write: //읽고 쓰기 전용
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS; //SRV, UAV 자원 뷰를 통해 파이프 라인에 연결
		break;
	}

	desc.Usage = D3D11_USAGE_DEFAULT; //GPU에 의해서 읽고 쓰기 가능 CPU 접근 불가
	desc.CPUAccessFlags = 0; //CPU 접근 불가

	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = m_element_size; //구조적 버퍼 요소당 크기

	auto device = DEVICE;
	HRESULT hResult;

	//초기 데이터가 있다면
	if (p_data != nullptr)
	{
		D3D11_SUBRESOURCE_DATA sub_data;
		ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));

		sub_data.pSysMem = p_data;

		//초기 데이터로 초기화한 상태로 구조적 버퍼 생성
		hResult = device->CreateBuffer(&desc, &sub_data, m_p_buffer.GetAddressOf());
		assert(SUCCEEDED(hResult));
		if (!SUCCEEDED(hResult))
			return;
	}
	//초기 데이터가 없다면
	else
	{
		//초기 데이터가 없는 상태로 구조적 버퍼 생성
		hResult = device->CreateBuffer(&desc, nullptr, m_p_buffer.GetAddressOf());
		assert(SUCCEEDED(hResult));
		if (!SUCCEEDED(hResult))
			return;
	}

	//=============================================
	//Create Shader Resource View(GPU 읽기만 가능한 경우)
	//=============================================
	CreateSRV();

	//=============================================
	//Create Unordered Access View(GPU 읽기, 쓰기 모두 가능한 경우)
	//=============================================
	if (m_sbuffer_type == SBufferType::Read_Write)
	{
		CreateUAV();
	}

	//=============================================
	//CPU에서 접근 가능한 Structured Buffer 생성
	//=============================================
	if (is_cpu_access)
	{
		//CPU Read
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.ByteWidth = m_buffer_size;

		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; //CPU 읽기만 허용

		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = m_element_size;

		//초기 데이터가 없는 상태로 구조적 버퍼 생성
		hResult = device->CreateBuffer(&desc, nullptr, m_p_cpu_read_buffer.GetAddressOf());
		assert(SUCCEEDED(hResult));
		if (!SUCCEEDED(hResult))
			return;

		//CPU Write
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.ByteWidth = m_buffer_size;

		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		desc.Usage = D3D11_USAGE_DYNAMIC; //GPU에 의해서 읽기 가능, CPU에서 쓰기 가능
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //CPU에서 쓰기 허용

		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = m_element_size;

		//초기 데이터가 없는 상태로 구조적 버퍼 생성
		hResult = device->CreateBuffer(&desc, nullptr, m_p_cpu_write_buffer.GetAddressOf());
		assert(SUCCEEDED(hResult));
	}
}
