
//Vertex ������ ���� ����ü ����

//VertexColor
//====================================================
struct VertexColor
{
    float4 position : POSITION;
    float4 color : COLOR;
};

//Vertex Shader ��ȯ��(VertexColor)
struct VertexColorOuputType
{
    float4 position : SV_Position;
    float4 color : COLOR;
};
//====================================================

//VertexTexture
//====================================================
struct VertexTexture
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

//Vertex Shader ��ȯ��(VertexTexture)
struct VertexTextureOutputType
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
};
//====================================================