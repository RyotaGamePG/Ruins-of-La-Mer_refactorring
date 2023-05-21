#include "FilterFunctions.hlsli"

struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

//	�X�v���C�g�`��P�ʂŎw�肷��p�����[�^�[
cbuffer CBColorGrading : register(b0)
{
    float hueShift; // �F������
    float saturation; // �ʓx����
    float brightness; // ���x����
    float dummy;
};
