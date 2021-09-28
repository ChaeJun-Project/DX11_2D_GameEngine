#include "VertexStruct.fx"
#include "ConstantBuffer.fx"

//Vertex Shader
VertexTextureOutputType VS(VertexTexture vs_input)
{
    VertexTextureOutputType vs_output;
    
    //World -> View -> Projection 순서대로 곱해줘야 정상적인 위치 값이 나옴
    vs_output.position = mul(vs_input.position, world);
    vs_output.position = mul(vs_output.position, view);
    vs_output.position = mul(vs_output.position, projection);
    
    //UV값 복사
    vs_output.uv = vs_input.uv;
    
    return vs_output;
}

//SamplerState: 도형에 셰이딩 작업이 이루어질 때 
//어떻게 텍스처의 픽셀이 사용되는 지를 수정할 수 있게 해줌
//ex) 물체가 너무 멀리 있어 8픽셀만큼의 영역을 차지하는 경우 
//이 객체를 사용하여 원래 텍스처의 어떤 픽셀 혹은 어떤 픽셀 조합을 사용해야 할지 결정
SamplerState Sampler : register(s0);
//Texture2D: 텍스처 자원
Texture2D Texture : register(t0); 

//Pixel Shader
float4 PS(VertexTextureOutputType ps_input) : SV_Target
{
    return Texture.Sample(Sampler, ps_input.uv);
}
