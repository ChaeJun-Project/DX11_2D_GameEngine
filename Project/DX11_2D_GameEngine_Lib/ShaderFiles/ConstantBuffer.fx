#ifndef ConstantBuffer
#define ConstantBuffer

#define PI 3.14159265f
//GPU���� ����� Constant Buffer(cbuffer) ����ü ����
//CPU���� ����ϴ� Constant Buffer ����ü�� ������ ũ�� �� ��ġ�� �����ؾ� 
//��Ȯ�� ��� ���� ��µ�
//hlsl�� �⺻������ �����͸� �� �켱 ������� ä��µ�
//����ü ��� �տ� row_major Ű���带 ���̸� �ش� ����� �� �켱 ������� �����͸� ä��
//���� DirectX -> Shader ���α׷����� �����͸� ���� �� ����� 2������
//1. DirectX���� ����� ��ġ���� Shader ���α׷����� ������ �� �켱 ������� �����͸� ä�� => ����ġ => DirectX�� ��� ������ ���� �״�� ����
//2. DirectX���� ����� �״�� Shader ���α׷����� ������ �� �켱 ������� �����͸� ä�� => DirectX�� ��� ������ ���� �״�� ����
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
    float2 g_resolution;      //����Ÿ�� �ػ�
    float2 g_noise_resolution;//������ �ؽ�ó �ػ� ũ��
    
    float3 g_view_position; //ī�޶� ��ġ
    float g_delta_time;       //������ �� �ð�
    
    float g_accumulate_time;  //�����ð�     
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

//Texture2D: �ؽ�ó �ڿ�
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

//SamplerState: ������ ���̵� �۾��� �̷���� �� 
//��� �ؽ�ó�� �ȼ��� ���Ǵ� ���� ������ �� �ְ� ����
//ex) ��ü�� �ʹ� �ָ� �־� 8�ȼ���ŭ�� ������ �����ϴ� ��� 
//�� ��ü�� ����Ͽ� ���� �ؽ�ó�� � �ȼ� Ȥ�� � �ȼ� ������ ����ؾ� ���� ����
SamplerState g_sampler1 : register(s0);
SamplerState g_sampler2 : register(s1);
SamplerState g_sampler3 : register(s2); //Mirror

#endif
