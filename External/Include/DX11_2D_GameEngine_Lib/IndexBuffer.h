#pragma once

class IndexBuffer final
{
public:
    IndexBuffer() = default;
    ~IndexBuffer() = default;

	//Index Buffer 생성
	//Index Buffer의 데이터 타입은 보통 int 또는 unsigned int 타입으로 고정되어 있으므로
	//Vertex Buffer처럼 template 멤버 함수로 정의하지 않음
	void Create(const std::vector<UINT>& index_vector, const D3D11_USAGE& usage = D3D11_USAGE_IMMUTABLE);

private:
	void Clear();

public:
	ID3D11Buffer* GetBuffer() const { SAFE_GET_POINTER(m_p_buffer.Get()); }
	const UINT& GetStride() const { return stride; }
	const UINT& GetOffset() const { return offset; }
	const UINT& GetCount() const { return count; }

private:
	ComPtr<ID3D11Buffer> m_p_buffer = nullptr;
	UINT stride = 0; //인덱스 데이터 타입의 크기
	UINT offset = 0; //해당 인덱스 버퍼의 시작 오프셋
	UINT count = 0;  //인덱스의 개수
};

