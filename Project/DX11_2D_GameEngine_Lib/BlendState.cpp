#include "stdafx.h"
#include "BlendState.h"

void BlendState::Create(const bool& is_blend_enable, const D3D11_BLEND_OP& blend_op, const D3D11_BLEND& src_blend, const D3D11_BLEND& dst_blend, const D3D11_BLEND_OP& blend_op_alpha, const D3D11_BLEND& src_blend_alpha, const D3D11_BLEND& dst_blend_alpha, const D3D11_COLOR_WRITE_ENABLE& color_mask)
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

	desc.AlphaToCoverageEnable = false;
	//독립적인 블랜더 사용
	desc.IndependentBlendEnable = false; //해당 옵션을 false로 설정하면 렌더타겟 0의 설정이 다른 렌더타겟에서도 사용

	desc.RenderTarget[0].BlendEnable = is_blend_enable;

	desc.RenderTarget[0].BlendOp = blend_op;
	desc.RenderTarget[0].SrcBlend = src_blend;
	desc.RenderTarget[0].DestBlend = dst_blend;

	desc.RenderTarget[0].BlendOpAlpha = blend_op_alpha;
	desc.RenderTarget[0].SrcBlendAlpha = src_blend_alpha;
	desc.RenderTarget[0].DestBlendAlpha = dst_blend_alpha;

	desc.RenderTarget[0].RenderTargetWriteMask = color_mask;

	//Blend State 생성
	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateBlendState(&desc, this->m_p_blend_state.GetAddressOf());
	assert(SUCCEEDED(hResult));
}
