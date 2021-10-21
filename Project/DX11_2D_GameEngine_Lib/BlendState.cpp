#include "stdafx.h"
#include "BlendState.h"

void BlendState::Create(const bool& is_blend_enable, const D3D11_BLEND_OP& blend_op, const D3D11_BLEND& src_blend, const D3D11_BLEND& dst_blend, const D3D11_BLEND_OP& blend_op_alpha, const D3D11_BLEND& src_blend_alpha, const D3D11_BLEND& dst_blend_alpha, const D3D11_COLOR_WRITE_ENABLE& color_mask)
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

	//���� �񱳸� �� �� ���İ����� ����ؼ� ���� => ���İ��� �������� ������ ���̰��� ������� ����
	//�������� ������ ������ ������ ���ؼ� ���� �� ������ �ذ�����
	desc.AlphaToCoverageEnable = false; 
	//�������� ���� ���
	//�ش� �ɼ��� false�� �����ϸ� ����Ÿ�� 0�� ������ �ٸ� ����Ÿ�ٿ����� ���
	desc.IndependentBlendEnable = false; 

	//���� �� ���� ȥ�� Ȱ��ȭ ���� ����
	desc.RenderTarget[0].BlendEnable = is_blend_enable;

	//���� ȥ�� ��� ����
	desc.RenderTarget[0].BlendOp = blend_op;
	//���� Back Buffer�� �׷��� �ȼ��� ������ ���� ȥ�� ���
	desc.RenderTarget[0].SrcBlend = src_blend;
	//�̹� Back Buffer�� �׷����ִ� �ȼ��� ������ ��� ȥ�� ���
	desc.RenderTarget[0].DestBlend = dst_blend;

	//���� ȥ�� ��� ����
	desc.RenderTarget[0].BlendOpAlpha = blend_op_alpha;
	//���� Back Buffer�� �׷��� �ȼ��� ���İ��� ���� ȥ�� ���
	desc.RenderTarget[0].SrcBlendAlpha = src_blend_alpha;
	//�̹� Back Buffer�� �׷����ִ� �ȼ��� ���İ��� ��� ȥ�� ���
	desc.RenderTarget[0].DestBlendAlpha = dst_blend_alpha;

	//ȥ�� ����� Back Buffer�� � ���� ä�ε鿡 ����� ������ ����
	//D3D11_COLOR_WRITE_ENABLE_ALPHA: ȥ�� ����� ���� ä�ε鿡�� ��ϵ��� �ʰ� ���� ä�ο��� ���(A�� ���)
	//D3D11_COLOR_WRITE_ENABLE_ALL: ȥ�� ����� ��� ä�ο� ���(R, G, B, A ��� ���)
	desc.RenderTarget[0].RenderTargetWriteMask = color_mask;

	//Blend State ����
	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateBlendState(&desc, this->m_p_blend_state.GetAddressOf());
	assert(SUCCEEDED(hResult));
}
