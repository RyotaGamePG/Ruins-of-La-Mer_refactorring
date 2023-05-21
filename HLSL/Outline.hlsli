struct VS_OUT
{
    float4 position : SV_POSITION;
};

cbuffer CbScene : register(b3)
{
    row_major float4x4 viewProjection;
    float4 outlineColor;
    float outlineSize;
    float3 dummy;
};

#define MAX_BONES 128
cbuffer CbMesh : register(b1)
{
    row_major float4x4 boneTransforms[MAX_BONES];
};

cbuffer CbSubset : register(b2)
{
    float4 materialColor;
};
