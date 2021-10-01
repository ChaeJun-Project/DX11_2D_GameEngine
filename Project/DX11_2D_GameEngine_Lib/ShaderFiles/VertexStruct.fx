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

//VertexColorTexture
//====================================================
struct VertexColorTexture
{
    float4 position : POSITION;
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