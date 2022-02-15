#ifndef PixelStruct
#define PixelStruct

//Pixel 정보에 관한 구조체 정의
struct PixelOutputType
{
    float4 game_scene : SV_Target0;
    float4 editor_scene : SV_Target1;
};

#endif