#include "Skybox.hlsli"

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
	//	カメラからワールド座標へのベクトル
    float3 E = normalize(pin.worldPosition.xyz - viewPosition.xyz);

	//	視線ベクトルを正距円筒図法に則ったUV座標へ変換する
    float2 texcoord;
	{
        texcoord = float2(1.0f - atan2(E.z, E.x) / 2.0f, -atan2(E.y, length(E.xz)));
        texcoord = texcoord / 3.14f + 0.5f;
    }
	//	mipmap使うと崩れるのでmipmapなしになるようにしておく
    return texture0.SampleLevel(sampler0, texcoord, 0) * pin.color;
}
