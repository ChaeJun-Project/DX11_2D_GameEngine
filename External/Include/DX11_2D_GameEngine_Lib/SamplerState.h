#pragma once

class SamplerState final
{
public:
	SamplerState() = default;
	~SamplerState() = default;

	void Create
	(
	    const UINT& sampler_bind_slot,
		const D3D11_FILTER& filter,
		const D3D11_TEXTURE_ADDRESS_MODE& address_mode_u,
		const D3D11_TEXTURE_ADDRESS_MODE& address_mode_v,
		const D3D11_TEXTURE_ADDRESS_MODE& address_mode_w,
		const D3D11_COMPARISON_FUNC& comparison_func = D3D11_COMPARISON_NEVER
	);

public:
	ID3D11SamplerState* GetSamplerState() const { SAFE_GET_POINTER(m_p_sampler_state.Get()); }
	const UINT& GetSamplerBindSlot() { return m_sampler_bind_slot; }

private:
	ComPtr<ID3D11SamplerState> m_p_sampler_state = nullptr;

	UINT m_sampler_bind_slot;
};

