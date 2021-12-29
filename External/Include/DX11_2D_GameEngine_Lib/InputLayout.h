#pragma once

//GPU에게 Vertex정보를 보관한 VertexBuffer를 전송할때 보통 메모리 덩어리로 전송하기 때문에
//GPU에서 Buffer의 정확한 속성을 추출해내기 위해서는 Vertex Layout을 알아야함.
//이를 위해서 Input Layout을 사용해야 함.

//Input Layout은 GPU에다가 Vertex의 구조체의 레이아웃을 설명
class InputLayout final
{
public: 
    InputLayout() = default;
    ~InputLayout() = default;

    const bool Create(ID3DBlob* blob);

public:
    ID3D11InputLayout* GetInputLayout() const { SAFE_GET_POINTER(m_p_input_layout.Get()); }

private:
    ComPtr<ID3D11InputLayout> m_p_input_layout = nullptr;
};

