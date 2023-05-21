#include"Light.hlsli"
#include"ShadowmapFunctions.hlsli"

struct VS_OUT
{
    float4 position : SV_POSITION;
    float3 world_position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
    float3 shadowTexcoord : TEXCOORD1; //	シャドウマップから情報を取得するためのUV座標＋深度値
};

cbuffer CbScene : register(b0)
{
    float4 viewPosition;
    row_major float4x4 viewProjection;
    float4 ambientLightColor;
    DirectionalLightData directionalLightData;
    PointLightData pointLightData[PointLightMax]; // 点光源情報
    SpotLightData spotLightData[SpotLightMax]; // スポットライト情報
    int pointLightCount; // 点光源数
    int spotLightCount; // スポットライト数
    float2 dummy2;
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

cbuffer CbShadowmap : register(b3)
{
    row_major float4x4 lightViewProjection; // ライトビュープロジェクション行列
    float3 shadowColor; // 影の色
    float shadowBias; // 深度値比較時のオフセット
};