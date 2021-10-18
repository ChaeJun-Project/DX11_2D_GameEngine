#include "stdafx.h"
#include "BlendState.h"

void BlendState::Create(const bool& is_blend_enable, const D3D11_BLEND_OP& blend_op, const D3D11_BLEND& src_blend, const D3D11_BLEND& dst_blend, const D3D11_BLEND_OP& blend_op_alpha, const D3D11_BLEND& src_blend_alpha, const D3D11_BLEND& dst_blend_alpha, const D3D11_COLOR_WRITE_ENABLE& color_mask)
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

	desc.AlphaToCoverageEnable = false;
	//�������� ���� ���
	desc.IndependentBlendEnable = false; //�ش� �ɼ��� false�� �����ϸ� ����Ÿ�� 0�� ������ �ٸ� ����Ÿ�ٿ����� ���

	desc.RenderTarget[0].BlendEnable = is_blend_enable;

	desc.RenderTarget[0].BlendOp = blend_op;
	desc.RenderTarget[0].SrcBlend = src_blend;
	desc.RenderTarget[0].DestBlend = dst_blend;

	desc.RenderTarget[0].BlendOpAlpha = blend_op_alpha;
	desc.RenderTarget[0].SrcBlendAlpha = src_blend_alpha;
	desc.RenderTarget[0].DestBlendAlpha = dst_blend_alpha;

	desc.RenderTarget[0].RenderTargetWriteMask = color_mask;

	//Blend State ����
	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateBlendState(&desc, this->m_p_blend_state.GetAddressOf());
	assert(SUCCEEDED(hResult));
}
