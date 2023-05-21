#include "FilterFunctions.hlsli"

struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

cbuffer CBLuminanceExtraction : register(b0)
{
    float threshold; // ���P�x���o�̂��߂�臒l
    float intensity; // �u���[���̋��x
    float2 dummy;
};
