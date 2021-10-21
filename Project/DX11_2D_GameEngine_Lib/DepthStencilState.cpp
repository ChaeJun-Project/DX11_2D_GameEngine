#include "stdafx.h"
#include "DepthStencilState.h"

void DepthStencilState::Create(const bool& is_depth_enable, const D3D11_COMPARISON_FUNC& comparison_func, const D3D11_DEPTH_WRITE_MASK& depth_write_mask)
{
	D3D11_DEPTH_STENCIL_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	//����
	//���̰� �� ��� ����
	//true: ���� �������� ���ؼ� z���� ���� ������Ʈ�ϼ��� ������ �׷���
	//false: ���� ������ �Ͼ�� �����Ƿ� �׸��� ������ �߿� => ���߿� �׷��� �ȼ��� ������ �ȼ��� ������ ������ ������
	desc.DepthEnable = is_depth_enable;
	//�� ��� ����
	desc.DepthFunc = comparison_func;
	//���Ͽ� ������ �ȼ��� ���̰��� ������� ����
	//D3D11_DEPTH_WRITE_MASK_ZERO: �ȼ��� ���̰� ���� ���ۿ� ���ŵ��� ����
    //D3D11_DEPTH_WRITE_MASK_ALL: ���� �� ���ٽ� ������ ��� ����� �ȼ��� ���̰� ���� ���ۿ� ���ŵ� => ���⼭�� ������� �����Ƿ� ���� ������ ����ص� ���� ���ۿ� ����
	desc.DepthWriteMask = depth_write_mask;

	//���ٽ�
	desc.StencilEnable  = false;
	
	//Depth Stencil State ����
	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateDepthStencilState(&desc, this->m_p_depth_stencil_state.GetAddressOf());
	assert(SUCCEEDED(hResult));
}
