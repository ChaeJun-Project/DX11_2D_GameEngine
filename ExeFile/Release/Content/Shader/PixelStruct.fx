#ifndef PixelStruct
#define PixelStruct

//Pixel ������ ���� ����ü ����
struct PixelOutputType
{
    float4 game_scene : SV_Target0;
    float4 editor_scene : SV_Target1;
};

#endif