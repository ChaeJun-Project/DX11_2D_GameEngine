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

//VertexColorTexture
//====================================================
struct VertexColorTexture
{
    float4 position : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

//Vertex Shader 반환형(VertexColorTexture)
struct VertexColorTextureOutputType
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};
//====================================================