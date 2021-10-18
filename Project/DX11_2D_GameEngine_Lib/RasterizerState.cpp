#include "stdafx.h"
#include "RasterizerState.h"

void RasterizerState::Create(const D3D11_CULL_MODE& cull_mode, const D3D11_FILL_MODE& fill_mode)
{
    D3D11_RASTERIZER_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

    desc.CullMode = cull_mode;
    desc.FillMode = fill_mode;

    //Rasterizer State »ý¼º
    auto device = GraphicsManager::GetInstance()->GetDevice();
    auto hResult = device->CreateRasterizerState(&desc, this->m_p_rasterizer_state.GetAddressOf());
    assert(SUCCEEDED(hResult));
}
