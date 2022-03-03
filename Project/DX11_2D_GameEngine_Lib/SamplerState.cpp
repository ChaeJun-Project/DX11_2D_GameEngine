#include "stdafx.h"
#include "SamplerState.h"

void SamplerState::Create
(
	const UINT& sampler_bind_slot,
	const D3D11_FILTER& filter,
	const D3D11_TEXTURE_ADDRESS_MODE& address_mode_u,
	const D3D11_TEXTURE_ADDRESS_MODE& address_mode_v,
	const D3D11_TEXTURE_ADDRESS_MODE& address_mode_w,
	const D3D11_COMPARISON_FUNC& comparison_func
)
{
	m_sampler_bind_slot = sampler_bind_slot;

	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));
	desc.Filter = filter;
	desc.AddressU = address_mode_u;
	desc.AddressV = address_mode_v;
	desc.AddressW = address_mode_w;
	desc.ComparisonFunc = comparison_func;

	//Sampler State »ý¼º
	auto device = GRAPHICS_MANAGER->GetDevice();
	auto hResult = device->CreateSamplerState(&desc, m_p_sampler_state.GetAddressOf());
	assert(SUCCEEDED(hResult));
}
