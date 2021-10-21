#pragma once

class RasterizerState final
{
public:
	RasterizerState() = default;
	~RasterizerState() = default;

	void Create
	(
		const D3D11_CULL_MODE& cull_mode,
		const D3D11_FILL_MODE& fill_mode,
		const bool& is_front_counter_clockwise = false
	);

public:
	ID3D11RasterizerState* GetRasterizerState() const { SAFE_GET_POINTER(this->m_p_rasterizer_state.Get()); }

private:
    ComPtr<ID3D11RasterizerState> m_p_rasterizer_state = nullptr;
};

