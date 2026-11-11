#ifndef __GGP_SHADER_STRUCTS__
#define __GPP_SHADER_STRUCTS__


struct VertexShaderInput
{
    float3 localPosition : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};




struct VertexToPixel
{
    float4 screenPosition : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 worldPos : POSITION;
};

struct VertexToPixel_Sky
{
    float4 screenPosition : SV_POSITION;
    float3 sampleDir : DIRECTION;
};




#endif