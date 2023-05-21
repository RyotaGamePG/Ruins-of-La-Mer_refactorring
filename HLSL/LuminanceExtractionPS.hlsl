#include "LuminanceExtraction.hlsli"

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = texture0.Sample(sampler0, pin.texcoord) * pin.color;

	// RGB > �P�x�l�ɕϊ�
    float luminance = RGB2Luminance(color.rgb);

	// 臒l�Ƃ̍����Z�o
    float contribution = max(0, luminance - threshold);

	// �o�͂���F��␳����
    contribution /= luminance;
    color.rgb *= contribution * intensity;
    return color;
}
