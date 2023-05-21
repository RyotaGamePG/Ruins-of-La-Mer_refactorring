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
    float3 shadowTexcoord : TEXCOORD1; //	�V���h�E�}�b�v��������擾���邽�߂�UV���W�{�[�x�l
};

cbuffer CbScene : register(b0)
{
    float4 viewPosition;
    row_major float4x4 viewProjection;
    float4 ambientLightColor;
    DirectionalLightData directionalLightData;
    PointLightData pointLightData[PointLightMax]; // �_�������
    SpotLightData spotLightData[SpotLightMax]; // �X�|�b�g���C�g���
    int pointLightCount; // �_������
    int spotLightCount; // �X�|�b�g���C�g��
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
    row_major float4x4 lightViewProjection; // ���C�g�r���[�v���W�F�N�V�����s��
    float3 shadowColor; // �e�̐F
    float shadowBias; // �[�x�l��r���̃I�t�Z�b�g
};