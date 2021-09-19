
//Vertex 정보에 관한 구조체 정의

//VertexColor
//====================================================
struct VertexColor
{
    float3 position : POSITION;
    float4 color : COLOR;
};

//Vertex Shader 반환형(VertexColor)
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

//Vertex Shader 반환형(VertexTexture)
struct VertexTextureOutputType
{
    float3 position : SV_Position;
    float2 uv : TEXCOORD;
};
//====================================================