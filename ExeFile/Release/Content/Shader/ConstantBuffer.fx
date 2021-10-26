//GPU에서 사용할 Constant Buffer(cbuffer) 구조체 정의
//CPU에서 사용하는 Constant Buffer 구조체와 변수의 크기 및 위치가 동일해야 
//정확한 결과 값이 출력됨
//hlsl은 기본적으로 데이터를 열 우선 방식으로 채우는데
//구조체 멤버 앞에 row_major 키워드를 붙이면 해당 멤버는 행 우선 방식으로 데이터를 채움
//따라서 DirectX -> Shader 프로그램으로 데이터를 보낼 때 방법이 2가지임
//1. DirectX에서 행렬을 전치시켜 Shader 프로그램으로 보내고 열 우선 방식으로 데이터를 채움 => 역전치 => DirectX의 행렬 데이터 값이 그대로 보존
//2. DirectX에서 행렬을 그대로 Shader 프로그램으로 보내고 행 우선 방식으로 데이터를 채움 => DirectX의 행렬 데이터 값이 그대로 보존
cbuffer VS_WVPMatrix : register(b0)
{
    row_major matrix world;
    row_major matrix view;
    row_major matrix projection;
}

cbuffer Material : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float2 g_vector2_0;
    float2 g_vector2_1;
    float2 g_vector2_2;
    float2 g_vector2_3;
    
    float4 g_vector4_0;
    float4 g_vector4_1;
    float4 g_vector4_2;
    float4 g_vector4_3;
    
    row_major matrix g_matrix_0;
    row_major matrix g_matrix_1;
    row_major matrix g_matrix_2;
    row_major matrix g_matrix_3;
}

//b2는 나중에 다른 버퍼를 만들 예정

#include "LightStruct.fx"

cbuffer Light : register(b3)
{
    LightInfo g_light2D_array[50];
    uint g_light2D_count;
    uint3 padding;
}

