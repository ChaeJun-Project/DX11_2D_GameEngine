#pragma once

class IndexBuffer final
{
public:
    IndexBuffer() = default;
    ~IndexBuffer() = default;

	//Index Buffer ����
	//Index Buffer�� ������ Ÿ���� ���� int �Ǵ� unsigned int Ÿ������ �����Ǿ� �����Ƿ�
	//Vertex Bufferó�� template ��� �Լ��� �������� ����
	void Create(const std::vector<UINT>& index_vector, const D3D11_USAGE& usage);

public:
	ID3D11Buffer* GetResource() const { SAFE_GET_POINT(this->m_p_buffer.Get()); }
	const UINT& GetStride() const { return this->stride; }
	const UINT& GetOffset() const { return this->offset; }
	const UINT& GetCount() const { return this->count; }

private:
	ComPtr<ID3D11Buffer> m_p_buffer = nullptr;
	UINT stride = 0; //�ε��� ������ Ÿ���� ũ��
	UINT offset = 0; //�ش� �ε��� ������ ���� ������
	UINT count = 0;  //�ε����� ����
};

