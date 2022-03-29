#ifndef Light2DStruct
#define Light2DStruct

struct Light2D_Info
{
    float4 color;

    int type;
    float3 position;

    float range;
    float3 padding;
};

#endif