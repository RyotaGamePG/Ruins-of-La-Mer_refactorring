#include "FilterFunctions.hlsli"

struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

cbuffer CBFinalpass : register(b0)
{
	//	�F���␳
    float hueShift; // �F������
    float saturation; // �ʓx����
    float brightness; // ���x����
    float dummy;
};
