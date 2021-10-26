//GPU���� ����� Constant Buffer(cbuffer) ����ü ����
//CPU���� ����ϴ� Constant Buffer ����ü�� ������ ũ�� �� ��ġ�� �����ؾ� 
//��Ȯ�� ��� ���� ��µ�
//hlsl�� �⺻������ �����͸� �� �켱 ������� ä��µ�
//����ü ��� �տ� row_major Ű���带 ���̸� �ش� ����� �� �켱 ������� �����͸� ä��
//���� DirectX -> Shader ���α׷����� �����͸� ���� �� ����� 2������
//1. DirectX���� ����� ��ġ���� Shader ���α׷����� ������ �� �켱 ������� �����͸� ä�� => ����ġ => DirectX�� ��� ������ ���� �״�� ����
//2. DirectX���� ����� �״�� Shader ���α׷����� ������ �� �켱 ������� �����͸� ä�� => DirectX�� ��� ������ ���� �״�� ����
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

//b2�� ���߿� �ٸ� ���۸� ���� ����

#include "LightStruct.fx"

cbuffer Light : register(b3)
{
    LightInfo g_light2D_array[50];
    uint g_light2D_count;
    uint3 padding;
}

