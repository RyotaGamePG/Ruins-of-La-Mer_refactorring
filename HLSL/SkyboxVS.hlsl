#include "Skybox.hlsli"

VS_OUT main(float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD)
{
    VS_OUT vout;
    vout.position = position;
    vout.color = color;

	//	ÉèÅ[ÉãÉhïœä∑
    float4 p = mul(position, inverseProjection);
    p /= p.w;
    p = mul(p, inverseView);
    vout.worldPosition = p.xyz;
    return vout;
}
