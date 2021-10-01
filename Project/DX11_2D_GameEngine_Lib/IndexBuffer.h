#pragma once

class IndexBuffer final : public DX11Obejct
{
public:
    IndexBuffer() = default;
    ~IndexBuffer() = default;

	//Index Buffer ����
	//Index Buffer�� ������ Ÿ���� ���� int �Ǵ� unsigned int Ÿ������ �����Ǿ� �����Ƿ�
	//Vertex Bufferó�� template ��� �Լ��� �������� ����
	void Create(const std::vector<UINT>& index_vector, const D3D11_USAGE& usage = D3D11_USAGE_IMMUTABLE);

public:
	ID3D11Buffer* GetBuffer() const { SAFE_GET_POINTER(this->m_p_buffer.Get()); }
	const UINT& GetStride() const { return this->stride; }
	const UINT& GetOffset() const { return this->offset; }
	const UINT& GetCount() const { return this->count; }

private:
	ComPtr<ID3D11Buffer> m_p_buffer = nullptr;
	UINT stride = 0; //�ε��� ������ Ÿ���� ũ��
	UINT offset = 0; //�ش� �ε��� ������ ���� ������
	UINT count = 0;  //�ε����� ����
};

