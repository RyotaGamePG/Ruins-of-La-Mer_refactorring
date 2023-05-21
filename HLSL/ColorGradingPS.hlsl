#include "ColorGrading.hlsli"

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = texture0.Sample(sampler0, pin.texcoord) * pin.color;
	// RGB > HSV�ɕϊ�
    color.rgb = RGB2HSV(color.rgb);

	// �F������
    color.r += hueShift;

	// �ʓx����
    color.g *= saturation;

	// ���x����
    color.b *= brightness;

	// HSV > RGB�ɕϊ�
    color.rgb = HSV2RGB(color.rgb);
    return color;
}
