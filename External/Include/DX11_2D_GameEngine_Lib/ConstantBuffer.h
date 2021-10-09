#pragma once

class ConstantBuffer final : public DX11Obejct
{
public:
	ConstantBuffer() = default;
	~ConstantBuffer() = default;

	//Constant Buffer�� ���� ������ �ֱ� ������ ����Լ� ���ø� ���
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
    //Constant Buffer ���ε� ���� ����
    this->m_buffer_bind_slot = buffer_bind_slot;

    //Constant Buffer ����
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
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; //�ش� Buffer�� Constant Buffer�� ���
	desc.ByteWidth = this->m_buffer_size = sizeof(T); //Constant Buffer�� �� ũ��

	//Constant Buffer ����
	//Constant Buffer�� �ٸ� buffer��� �ٸ��� D3D11_SUBRESOURCE_DATA�� �ʿ���������
	//�����Ǿ� �ִ� ���� �����Ͱ� �ƴϰ� ��� ����Ǵ� ���̱� ����
	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateBuffer(&desc, nullptr, this->m_p_buffer.GetAddressOf());
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return;
}