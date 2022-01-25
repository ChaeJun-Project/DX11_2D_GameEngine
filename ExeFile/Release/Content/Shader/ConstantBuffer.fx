#ifndef ConstantBuffer
#define ConstantBuffer

#define PI 3.14159265f
//GPU에서 사용할 Constant Buffer(cbuffer) 구조체 정의
//CPU에서 사용하는 Constant Buffer 구조체와 변수의 크기 및 위치가 동일해야 
//정확한 결과 값이 출력됨
//hlsl은 기본적으로 데이터를 열 우선 방식으로 채우는데
//구조체 멤버 앞에 row_major 키워드를 붙이면 해당 멤버는 행 우선 방식으로 데이터를 채움
//따라서 DirectX -> Shader 프로그램으로 데이터를 보낼 때 방법이 2가지임
//1. DirectX에서 행렬을 전치시켜 Shader 프로그램으로 보내고 열 우선 방식으로 데이터를 채움 => 역전치 => DirectX의 행렬 데이터 값이 그대로 보존
//2. DirectX에서 행렬을 그대로 Shader 프로그램으로 보내고 행 우선 방식으로 데이터를 채움 => DirectX의 행렬 데이터 값이 그대로 보존
cbuffer WVPMatrix : register(b0)
{
    row_major matrix world;
    row_major matrix view;
    row_major matrix projection;
    
    row_major matrix reflection;
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
    float4 g_vector4_4;
    float4 g_vector4_5;
    float4 g_vector4_6;
    float4 g_vector4_7;
    
    row_major matrix g_matrix_0;
    row_major matrix g_matrix_1;
    row_major matrix g_matrix_2;
    row_major matrix g_matrix_3;
}

cbuffer Program : register(b2)
{
    float2 g_resolution;      //렌더타겟 해상도
    float2 g_noise_resolution;//노이즈 텍스처 해상도 크기
    
    float3 g_view_position; //카메라 위치
    float g_delta_time;       //프레임 당 시간
    
    float g_accumulate_time;  //누적시간     
    float3 g_program_padding;
}

#include "LightStruct.fx"

cbuffer Light2D : register(b3)
{
    LightInfo g_light2D_array[50];
    uint g_light2D_count;
    float3 g_light2D_padding;
}

#include "SpriteAnimationStruct.fx"

cbuffer SpriteAnimation : register(b4)
{
    SpriteAnimation_Data g_animation2D_data_array[50];
}

//Texture2D: 텍스처 자원
Texture2D g_texture_0 : register(t0);
Texture2D g_texture_1 : register(t1);
Texture2D g_texture_2 : register(t2);
Texture2D g_texture_3 : register(t3);
Texture2D g_texture_4 : register(t4);
Texture2D g_texture_5 : register(t5);
Texture2D g_texture_6 : register(t6);
Texture2D g_texture_7 : register(t7);

//Noise Texture
Texture2D g_noise_texture : register(t13);

//SamplerState: 도형에 셰이딩 작업이 이루어질 때 
//어떻게 텍스처의 픽셀이 사용되는 지를 수정할 수 있게 해줌
//ex) 물체가 너무 멀리 있어 8픽셀만큼의 영역을 차지하는 경우 
//이 객체를 사용하여 원래 텍스처의 어떤 픽셀 혹은 어떤 픽셀 조합을 사용해야 할지 결정
SamplerState g_sampler1 : register(s0);
SamplerState g_sampler2 : register(s1);
SamplerState g_sampler3 : register(s2); //Mirror

#endif
