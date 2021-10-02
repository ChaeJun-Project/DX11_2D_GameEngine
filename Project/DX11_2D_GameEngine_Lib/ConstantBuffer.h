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

	//Constant Buffer�� ���� ������ �ֱ� ������ ����Լ� ���ø� ���
	template<typename T>
	T* Map();
	void Unmap();

public:
    const UINT& GetBufferBindSlot() { return this->buffer_bind_slot; }

private:
	ComPtr<ID3D11Buffer> m_p_buffer = nullptr;
	UINT buffer_bind_slot;
};

template<typename T>
inline void ConstantBuffer::Create(const UINT& buffer_bind_slot, const D3D11_USAGE& usage)
{
    //Constant Buffer ���ε� ���� ����
    this->buffer_bind_slot = buffer_bind_slot;

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
	desc.ByteWidth = sizeof(T); //Constant Buffer�� �� ũ��

	//Constant Buffer ����
	//Constant Buffer�� �ٸ� buffer��� �ٸ��� D3D11_SUBRESOURCE_DATA�� �ʿ���������
	//�����Ǿ� �ִ� ���� �����Ͱ� �ƴϰ� ��� ����Ǵ� ���̱� ����
	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateBuffer(&desc, nullptr, this->m_p_buffer.GetAddressOf());
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return;
}

template<typename T>
inline T* ConstantBuffer::Map()
{
    D3D11_MAPPED_SUBRESOURCE mapped_sub_data;
	ZeroMemory(&mapped_sub_data, sizeof(D3D11_MAPPED_SUBRESOURCE));

	auto device_context = GraphicsManager::GetInstance()->GetDeviceContext();
	auto hResult = device_context->Map
	(
		m_p_buffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mapped_sub_data
	);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return nullptr;

	return reinterpret_cast<T*>(mapped_sub_data.pData);
}
