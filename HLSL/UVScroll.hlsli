struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

//	スプライト描画単位で指定するパラメーター
cbuffer CbScene : register(b0)
{
    float2 uvScrollValue; // UVスクロール値
    float2 dummy; // ダミー
};
