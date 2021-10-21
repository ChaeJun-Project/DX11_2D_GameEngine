#include "stdafx.h"
#include "DepthStencilState.h"

void DepthStencilState::Create(const bool& is_depth_enable, const D3D11_COMPARISON_FUNC& comparison_func, const D3D11_DEPTH_WRITE_MASK& depth_write_mask)
{
	D3D11_DEPTH_STENCIL_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	//깊이
	//깊이값 비교 사용 여부
	//true: 깊이 판정으로 인해서 z값이 작은 오브젝트일수록 앞으로 그려짐
	//false: 깊이 판정이 일어나지 않으므로 그리기 순서가 중요 => 나중에 그려진 픽셀이 기존의 픽셀을 덮어씌우기 때문에 가려짐
	desc.DepthEnable = is_depth_enable;
	//비교 방식 설정
	desc.DepthFunc = comparison_func;
	//비교하여 결정된 픽셀의 깊이값을 사용할지 결정
	//D3D11_DEPTH_WRITE_MASK_ZERO: 픽셀의 깊이가 깊이 버퍼에 갱신되지 않음
    //D3D11_DEPTH_WRITE_MASK_ALL: 깊이 및 스텐실 판정을 모두 통과한 픽셀의 깊이가 깊이 버퍼에 갱신됨 => 여기서는 사용하지 않으므로 깊이 판정만 통과해도 깊이 버퍼에 갱신
	desc.DepthWriteMask = depth_write_mask;

	//스텐실
	desc.StencilEnable  = false;
	
	//Depth Stencil State 생성
	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateDepthStencilState(&desc, this->m_p_depth_stencil_state.GetAddressOf());
	assert(SUCCEEDED(hResult));
}
