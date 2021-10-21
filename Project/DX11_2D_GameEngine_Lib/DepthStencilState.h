#pragma once

class DepthStencilState final
{
public:
	DepthStencilState() = default;
	~DepthStencilState() = default;

	void Create
	(
       const bool& is_depth_enable = true,
	   const D3D11_COMPARISON_FUNC& comparison_func = D3D11_COMPARISON_LESS,
	   const D3D11_DEPTH_WRITE_MASK& depth_write_mask = D3D11_DEPTH_WRITE_MASK_ALL
	);

public:
	ID3D11DepthStencilState* GetDepthStencilState() const { SAFE_GET_POINTER(this->m_p_depth_stencil_state.Get()); }

private:
    ComPtr<ID3D11DepthStencilState> m_p_depth_stencil_state = nullptr; 
};

