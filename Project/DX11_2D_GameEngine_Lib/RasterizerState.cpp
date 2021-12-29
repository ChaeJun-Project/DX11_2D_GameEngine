#include "stdafx.h"
#include "RasterizerState.h"

void RasterizerState::Create(const D3D11_CULL_MODE& cull_mode, const D3D11_FILL_MODE& fill_mode, const bool& is_front_counter_clockwise)
{
    D3D11_RASTERIZER_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

    //지정된 방향을 향한 삼각형이 그려지지 않도록 설정
    //D3D11_CULL_NONE: 전면 및 후면 삼각형 모두 그림
    //D3D11_CULL_FRONT: 전면 삼각형을 그리지 않음
    //D3D11_CULL_BACK(Default): 후면 삼각형을 그리지 않음
    desc.CullMode = cull_mode;
    //2차원으로 그려진 도형에 색을 채울 방식을 선택 
    //Solid(Default): 도형의 색을 모두 채움
    //WireFrame: 도형의 정점을 이은 라인만 색을 채움
    desc.FillMode = fill_mode; 
    //전면 삼각형으로 인식하는 방식 설정
    //false(Default): 시계방향으로 그린 삼각형을 전면, 반시계방향으로 그린 삼각형을 후면으로 간주
    //true: 시계방향으로 그린 삼각형을 후면, 반시계방향으로 그린 삼각형을 전면으로 간주
    desc.FrontCounterClockwise = is_front_counter_clockwise;

    //Rasterizer State 생성
    auto device = GraphicsManager::GetInstance()->GetDevice();
    auto hResult = device->CreateRasterizerState(&desc, m_p_rasterizer_state.GetAddressOf());
    assert(SUCCEEDED(hResult));
}
