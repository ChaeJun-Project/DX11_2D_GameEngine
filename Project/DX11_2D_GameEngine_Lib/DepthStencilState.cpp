#include "stdafx.h"
#include "DepthStencilState.h"

void DepthStencilState::Create(const bool& is_depth_enable, const D3D11_COMPARISON_FUNC& comparison_func)
{
	D3D11_DEPTH_STENCIL_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	//����
	desc.DepthEnable = is_depth_enable;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = comparison_func;

	//���ٽ�
	desc.StencilEnable  = false;
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	//���� ���� �������� ��� ���ٽ� �׽�Ʈ ����
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//���� �ڸ� �������� ��� ���ٽ� �׽�Ʈ ����
	//���� ���� �������� ���� ����
	desc.BackFace = desc.FrontFace;

	//Depth Stencil State ����
	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateDepthStencilState(&desc, this->m_p_depth_stencil_state.GetAddressOf());
	assert(SUCCEEDED(hResult));
}
