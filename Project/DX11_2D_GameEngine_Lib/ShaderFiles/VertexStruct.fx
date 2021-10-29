//Vertex ������ ���� ����ü ����

//VertexColor
//====================================================
struct VertexColor
{
    float3 position : POSITION;
    float4 color : COLOR;
};

//Vertex Shader ��ȯ��(VertexColor)
struct VertexColorOuputType
{
    float4 position : SV_Position;
    float4 color : COLOR;
};
//====================================================

//VertexColorTexture
//====================================================
struct VertexColorTexture
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

//Vertex Shader ��ȯ��(VertexColorTexture)
struct VertexColorTextureOutputType
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};
//====================================================

//VertexColorTexture
//====================================================
//Vertex Shader ��ȯ��(VertexColorTexture)
struct VertexColorTextureLightOutputType
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float3 world_position : POSITION;
    float2 uv : TEXCOORD;
};
//====================================================