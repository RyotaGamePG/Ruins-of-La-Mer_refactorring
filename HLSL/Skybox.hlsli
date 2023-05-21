struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 worldPosition : POSITION;
};

cbuffer CbScene : register(b0)
{
    float4 viewPosition;
    row_major float4x4 inverseView;
    row_major float4x4 inverseProjection;
};
