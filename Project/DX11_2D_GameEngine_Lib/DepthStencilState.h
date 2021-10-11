#pragma once

class DepthStencilState final : public DX11Obejct
{
public:
	DepthStencilState() = default;
	~DepthStencilState() = default;

	void Create
	(
       const bool& is_depth_enable,
	   const D3D11_COMPARISON_FUNC& comparison_func = D3D11_COMPARISON_LESS
	);

public:
	ID3D11DepthStencilState* GetDepthStencilState() const { SAFE_GET_POINTER(this->m_p_depth_stencil_state.Get()); }

private:
    ComPtr<ID3D11DepthStencilState> m_p_depth_stencil_state = nullptr; 
};

