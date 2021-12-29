#pragma once

//GPU���� Vertex������ ������ VertexBuffer�� �����Ҷ� ���� �޸� ����� �����ϱ� ������
//GPU���� Buffer�� ��Ȯ�� �Ӽ��� �����س��� ���ؼ��� Vertex Layout�� �˾ƾ���.
//�̸� ���ؼ� Input Layout�� ����ؾ� ��.

//Input Layout�� GPU���ٰ� Vertex�� ����ü�� ���̾ƿ��� ����
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

