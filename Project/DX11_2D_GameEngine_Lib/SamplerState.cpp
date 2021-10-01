#include "stdafx.h"
#include "SamplerState.h"

void SamplerState::Create(const UINT& sampler_bind_slot, const D3D11_FILTER& filter, const D3D11_TEXTURE_ADDRESS_MODE& address_mode, const D3D11_COMPARISON_FUNC& comparison_func)
{
   this->m_sampler_bind_slot = sampler_bind_slot;

   D3D11_SAMPLER_DESC desc;
   ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));
   desc.Filter = filter;
   desc.AddressU = address_mode;
   desc.AddressV = address_mode;
   desc.AddressW = address_mode;
   desc.ComparisonFunc = comparison_func;

   //Sampler State »ý¼º
   auto device = GraphicsManager::GetInstance()->GetDevice();
   auto hResult = device->CreateSamplerState(&desc, this->m_p_sampler_state.GetAddressOf());
   assert(SUCCEEDED(hResult));
}
