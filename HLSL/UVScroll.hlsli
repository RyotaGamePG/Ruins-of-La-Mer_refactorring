struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

//	�X�v���C�g�`��P�ʂŎw�肷��p�����[�^�[
cbuffer CbScene : register(b0)
{
    float2 uvScrollValue; // UV�X�N���[���l
    float2 dummy; // �_�~�[
};
