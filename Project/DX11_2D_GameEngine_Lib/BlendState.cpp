#include "stdafx.h"
#include "BlendState.h"

void BlendState::Create(const bool& is_blend_enable, const D3D11_BLEND_OP& blend_op, const D3D11_BLEND& src_blend, const D3D11_BLEND& dst_blend, const D3D11_BLEND_OP& blend_op_alpha, const D3D11_BLEND& src_blend_alpha, const D3D11_BLEND& dst_blend_alpha, const D3D11_COLOR_WRITE_ENABLE& color_mask)
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

	//깊이 비교를 할 때 알파값까지 고려해서 남김 => 알파값이 존재하지 않으면 깊이값에 기록하지 않음
	//불투명한 영역과 투명한 영역에 대해서 깊이 비교 문제를 해결해줌
	desc.AlphaToCoverageEnable = false; 
	//독립적인 블랜더 사용
	//해당 옵션을 false로 설정하면 렌더타겟 0의 설정이 다른 렌더타겟에서도 사용
	desc.IndependentBlendEnable = false; 

	//색상 및 알파 혼합 활성화 여부 설정
	desc.RenderTarget[0].BlendEnable = is_blend_enable;

	//색상 혼합 방식 설정
	desc.RenderTarget[0].BlendOp = blend_op;
	//새로 Back Buffer에 그려질 픽셀의 색상의 원본 혼합 계수
	desc.RenderTarget[0].SrcBlend = src_blend;
	//이미 Back Buffer에 그려져있는 픽셀의 색상의 대상 혼합 계수
	desc.RenderTarget[0].DestBlend = dst_blend;

	//알파 혼합 방식 설정
	desc.RenderTarget[0].BlendOpAlpha = blend_op_alpha;
	//새로 Back Buffer에 그려질 픽셀의 알파값의 원본 혼합 계수
	desc.RenderTarget[0].SrcBlendAlpha = src_blend_alpha;
	//이미 Back Buffer에 그려져있는 픽셀의 알파값의 대상 혼합 계수
	desc.RenderTarget[0].DestBlendAlpha = dst_blend_alpha;

	//혼합 결과를 Back Buffer의 어떤 색상 채널들에 기록할 것인지 결정
	//D3D11_COLOR_WRITE_ENABLE_ALPHA: 혼합 결과가 색상 채널들에는 기록되지 않고 알파 채널에만 기록(A만 기록)
	//D3D11_COLOR_WRITE_ENABLE_ALL: 혼합 결과가 모든 채널에 기록(R, G, B, A 모두 기록)
	desc.RenderTarget[0].RenderTargetWriteMask = color_mask;

	//Blend State 생성
	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateBlendState(&desc, this->m_p_blend_state.GetAddressOf());
	assert(SUCCEEDED(hResult));
}
