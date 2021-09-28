
//Vertex 정보에 관한 구조체 정의

//VertexColor
//====================================================
struct VertexColor
{
    float4 position : POSITION;
    float4 color : COLOR;
};

//Vertex Shader 반환형(VertexColor)
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

//Vertex Shader 반환형(VertexTexture)
struct VertexTextureOutputType
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
};
//====================================================