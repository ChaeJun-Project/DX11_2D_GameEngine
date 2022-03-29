#ifndef LightStruct
#define LightStruct

struct LightColor
{
    float4 color;
    float4 specular;
    float4 ambient;
};

struct LightInfo
{
    LightColor ligth_color;

    int light_type;
    float3 light_direction;
    float3 light_position;

    float light_range;
    float light_angle;
    float3 padding;
};

#endif