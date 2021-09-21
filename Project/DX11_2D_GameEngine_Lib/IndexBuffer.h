#pragma once

class IndexBuffer final
{
public:
    IndexBuffer() = default;
    ~IndexBuffer() = default;

	//Index Buffer 생성
	//Index Buffer의 데이터 타입은 보통 int 또는 unsigned int 타입으로 고정되어 있으므로
	//Vertex Buffer처럼 template 멤버 함수로 정의하지 않음
	void Create(const std::vector<UINT>& index_vector, const D3D11_USAGE& usage);

public:
	ID3D11Buffer* GetResource() const { SAFE_GET_POINT(this->m_p_buffer.Get()); }
	const UINT& GetStride() const { return this->stride; }
	const UINT& GetOffset() const { return this->offset; }
	const UINT& GetCount() const { return this->count; }

private:
	ComPtr<ID3D11Buffer> m_p_buffer = nullptr;
	UINT stride = 0; //인덱스 데이터 타입의 크기
	UINT offset = 0; //해당 인덱스 버퍼의 시작 오프셋
	UINT count = 0;  //인덱스의 개수
};

