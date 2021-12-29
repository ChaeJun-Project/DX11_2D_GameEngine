#include "stdafx.h"
#include "RasterizerState.h"

void RasterizerState::Create(const D3D11_CULL_MODE& cull_mode, const D3D11_FILL_MODE& fill_mode, const bool& is_front_counter_clockwise)
{
    D3D11_RASTERIZER_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

    //������ ������ ���� �ﰢ���� �׷����� �ʵ��� ����
    //D3D11_CULL_NONE: ���� �� �ĸ� �ﰢ�� ��� �׸�
    //D3D11_CULL_FRONT: ���� �ﰢ���� �׸��� ����
    //D3D11_CULL_BACK(Default): �ĸ� �ﰢ���� �׸��� ����
    desc.CullMode = cull_mode;
    //2�������� �׷��� ������ ���� ä�� ����� ���� 
    //Solid(Default): ������ ���� ��� ä��
    //WireFrame: ������ ������ ���� ���θ� ���� ä��
    desc.FillMode = fill_mode; 
    //���� �ﰢ������ �ν��ϴ� ��� ����
    //false(Default): �ð�������� �׸� �ﰢ���� ����, �ݽð�������� �׸� �ﰢ���� �ĸ����� ����
    //true: �ð�������� �׸� �ﰢ���� �ĸ�, �ݽð�������� �׸� �ﰢ���� �������� ����
    desc.FrontCounterClockwise = is_front_counter_clockwise;

    //Rasterizer State ����
    auto device = GraphicsManager::GetInstance()->GetDevice();
    auto hResult = device->CreateRasterizerState(&desc, m_p_rasterizer_state.GetAddressOf());
    assert(SUCCEEDED(hResult));
}
