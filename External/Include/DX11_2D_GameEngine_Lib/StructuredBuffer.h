#pragma once

//����ȭ ����
class StructuredBuffer final
{
public:
	StructuredBuffer() = default;
	~StructuredBuffer() = default;

	//Structured Buffer�� ���� ������ �ֱ� ������ ����Լ� ���ø� ���
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
	UINT m_buffer_bind_slot = 0;	//t(texture) register ���ε� ���� ��ȣ
	UINT m_unordered_bind_slot = 0; //u(unordered) register ���ε� ���� ��ȣ
};

template<typename T>
void StructuredBuffer::Create(const UINT& element_count, const SBufferType& structured_buffer_type, const bool& is_cpu_access, void* p_data)
{
	m_element_size = sizeof(T);					    //������ ���� ��Ҵ� ������
	m_element_count = element_count;			    //������ ���� ��� ����
	m_buffer_size = m_element_size * element_count; //������ ���� �� ũ��
	m_sbuffer_type = structured_buffer_type;		//������ ���� ���� ����

	//=============================================
	//������ ���� ����
	//=============================================
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = m_buffer_size;

	switch (m_sbuffer_type)
	{
	case SBufferType::Read_Only: //�б� ����
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; //SRV �ڿ� �並 ���� ������ ���ο� ����
		break;
	case SBufferType::Read_Write: //�а� ���� ����
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS; //SRV, UAV �ڿ� �並 ���� ������ ���ο� ����
		break;
	}

	desc.Usage = D3D11_USAGE_DEFAULT; //GPU�� ���ؼ� �а� ���� ���� CPU ���� �Ұ�
	desc.CPUAccessFlags = 0; //CPU ���� �Ұ�

	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = m_element_size; //������ ���� ��Ҵ� ũ��

	auto device = DEVICE;
	HRESULT hResult;

	//�ʱ� �����Ͱ� �ִٸ�
	if (p_data != nullptr)
	{
		D3D11_SUBRESOURCE_DATA sub_data;
		ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));

		sub_data.pSysMem = p_data;

		//�ʱ� �����ͷ� �ʱ�ȭ�� ���·� ������ ���� ����
		hResult = device->CreateBuffer(&desc, &sub_data, m_p_buffer.GetAddressOf());
		assert(SUCCEEDED(hResult));
		if (!SUCCEEDED(hResult))
			return;
	}
	//�ʱ� �����Ͱ� ���ٸ�
	else
	{
		//�ʱ� �����Ͱ� ���� ���·� ������ ���� ����
		hResult = device->CreateBuffer(&desc, nullptr, m_p_buffer.GetAddressOf());
		assert(SUCCEEDED(hResult));
		if (!SUCCEEDED(hResult))
			return;
	}

	//=============================================
	//Create Shader Resource View(GPU �б⸸ ������ ���)
	//=============================================
	CreateSRV();

	//=============================================
	//Create Unordered Access View(GPU �б�, ���� ��� ������ ���)
	//=============================================
	if (m_sbuffer_type == SBufferType::Read_Write)
	{
		CreateUAV();
	}

	//=============================================
	//CPU���� ���� ������ Structured Buffer ����
	//=============================================
	if (is_cpu_access)
	{
		//CPU Read
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.ByteWidth = m_buffer_size;

		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; //CPU �б⸸ ���

		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = m_element_size;

		//�ʱ� �����Ͱ� ���� ���·� ������ ���� ����
		hResult = device->CreateBuffer(&desc, nullptr, m_p_cpu_read_buffer.GetAddressOf());
		assert(SUCCEEDED(hResult));
		if (!SUCCEEDED(hResult))
			return;

		//CPU Write
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.ByteWidth = m_buffer_size;

		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		desc.Usage = D3D11_USAGE_DYNAMIC; //GPU�� ���ؼ� �б� ����, CPU���� ���� ����
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //CPU���� ���� ���

		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = m_element_size;

		//�ʱ� �����Ͱ� ���� ���·� ������ ���� ����
		hResult = device->CreateBuffer(&desc, nullptr, m_p_cpu_write_buffer.GetAddressOf());
		assert(SUCCEEDED(hResult));
	}
}
