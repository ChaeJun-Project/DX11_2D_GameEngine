#pragma once

class BlendState final
{
public:
	BlendState(ID3D11Device* device);
	~BlendState();

	void Create
	(
		const bool& is_blend_enable,
		const D3D11_BLEND& src_blend = D3D11_BLEND_SRC_ALPHA,	  //���� �ؽ�ó�� ���İ� ���
		const D3D11_BLEND& dst_blend = D3D11_BLEND_INV_SRC_ALPHA, //���� �ؽ�ó�� 1 - ���İ� ���
		const D3D11_BLEND_OP& blend_op = D3D11_BLEND_OP_ADD,
		const D3D11_BLEND& src_blend_alpha = D3D11_BLEND_ONE,
		const D3D11_BLEND& dst_blend_alpha = D3D11_BLEND_ZERO,
		const D3D11_BLEND_OP& blend_op_alpha = D3D11_BLEND_OP_ADD,
		const D3D11_COLOR_WRITE_ENABLE& color_mask = D3D11_COLOR_WRITE_ENABLE_ALL
	);

public:
	ID3D11BlendState* GetBlendState() const { SAFE_GET_POINTER(this->m_p_blend_state.Get()); }

private:
	ID3D11Device* device;
	ComPtr<ID3D11BlendState> m_p_blend_state = nullptr;
};

