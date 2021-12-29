#pragma once

class BlendState final
{
public:
	BlendState() = default;
	~BlendState() = default;

	void Create
	(
		const bool& is_blend_enable = false,
		const D3D11_BLEND_OP& blend_op = D3D11_BLEND_OP_ADD, //���� ȥ�չ�� ���
		const D3D11_BLEND& src_blend = D3D11_BLEND_ONE,		 //�������� ���� ����
		const D3D11_BLEND& dst_blend = D3D11_BLEND_ZERO,	 //Back Buffer���� �׸� ��ġ�� ����
		const D3D11_BLEND_OP& blend_op_alpha = D3D11_BLEND_OP_ADD,
		const D3D11_BLEND& src_blend_alpha = D3D11_BLEND_ONE,
		const D3D11_BLEND& dst_blend_alpha = D3D11_BLEND_ZERO,
		const D3D11_COLOR_WRITE_ENABLE& color_mask = D3D11_COLOR_WRITE_ENABLE_ALL
	);

public:
	ID3D11BlendState* GetBlendState() const { SAFE_GET_POINTER(m_p_blend_state.Get()); }

private:
	ComPtr<ID3D11BlendState> m_p_blend_state = nullptr;
};

