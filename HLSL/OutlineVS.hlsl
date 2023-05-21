#include "Outline.hlsli"

VS_OUT main(
	float4 position : POSITION,
	float3 normal : NORMAL,
	float3 tangent : TANGENT,
	float2 texcoord : TEXCOORD,
	float4 color : COLOR,
	float4 boneWeights : WEIGHTS,
	uint4 boneIndices : BONES
)
{
    float3 p = { 0, 0, 0 };
    float3 n = { 0, 0, 0 };
    for (int i = 0; i < 4; i++)
    {
        p += (boneWeights[i] * mul(position, boneTransforms[boneIndices[i]])).xyz;
        n += (boneWeights[i] * mul(float4(normal.xyz, 0), boneTransforms[boneIndices[i]])).xyz;
    }
    n = normalize(n);

    VS_OUT vout;
	//viewProjection,boneTransforms[],p,outlineColor
	//	法線方向に拡大(各法線方向にmodelの座標を+)
    vout.position = mul(float4(p.xyz + (n.xyz / 10), 1.0f), viewProjection);
    return vout;
}
