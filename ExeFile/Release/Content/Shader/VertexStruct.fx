
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
    float3 position : SV_Position;
    float4 color : COLOR;
};
//====================================================

//VertexTexture
//====================================================
struct VertexTexture
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
};

//Vertex Shader ��ȯ��(VertexTexture)
struct VertexTextureOutputType
{
    float3 position : SV_Position;
    float2 uv : TEXCOORD;
};
//====================================================