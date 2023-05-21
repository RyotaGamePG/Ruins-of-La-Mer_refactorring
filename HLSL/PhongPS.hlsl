#include "Phong.hlsli"

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1); // �@���}�b�v
Texture2D shadowMap : register(t2); // �V���h�E�}�b�v

SamplerState diffuseMapSamplerState : register(s0);
SamplerState shadowMapSamplerState : register(s1);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 diffuseColor = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;

	//float3 N = normalize(pin.normal);
		// �@���}�b�v����xyz�������擾����( -1 �` +1 )�̊ԂɃX�P�[�����O
    float3 normal = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).xyz * 2 - 1;
    float3x3 CM = { normalize(pin.tangent), normalize(pin.binormal), normalize(pin.normal) };
    float3 N = normalize(mul(normal, CM));

    float3 L = normalize(directionalLightData.direction.xyz);
    float3 E = normalize(viewPosition.xyz - pin.world_position.xyz);

	// �}�e���A���萔
    float3 ka = float3(1, 1, 1);
    float3 kd = float3(1, 1, 1);
//float3 ks = float3(0.25f, 0.25f, 0.25f);//1,1,1�Ńe�J��
	//float3 ks = float3(0, 0, 0);//1,1,1�Ńe�J��
    float3 ks = float3(1, 1, 1); //1,1,1�Ńe�J��

    float shiness = 128;

	// �����̌v�Z
    float3 ambient = ka * ambientLightColor;


	// ���s�����̃��C�e�B���O�v�Z
	//float3 directionalDiffuse = CalcLambertDiffuse(N, L, directionalLightData.color.rgb, kd);
	//float3 directionalSpecular = CalcPhongSpecular(N, L, directionalLightData.color.rgb, E, shiness, ks);
    float3 directionalDiffuse = CalcLambertDiffuse(N, L, directionalLightData.color.rgb, kd);
	//float3 directionalDiffuse = ClacHalfLambert(N, L, directionalLightData.color.rgb, kd);//�n�[�t�����o�[�g��
    float3 directionalSpecular = CalcPhongSpecular(N, L, directionalLightData.color.rgb, E, shiness, ks);

	// ���s�����̉e�Ȃ̂ŁA���s�����ɑ΂��ĉe��K��
    float3 shadow = CalcShadowColorPCFFilter(shadowMap, shadowMapSamplerState, pin.shadowTexcoord, shadowColor, shadowBias);
    directionalDiffuse *= shadow;
    directionalSpecular *= shadow;

	// �_�����̏���
    float3 pointDiffuse = (float3) 0;
    float3 pointSpecular = (float3) 0;
    int i;
    for (i = 0; i < pointLightCount; ++i)
    {
		// ���C�g�x�N�g�����Z�o
        float3 lightVector = pin.world_position - pointLightData[i].position.xyz;

		// ���C�g�x�N�g���̒������Z�o
        float lightLength = length(lightVector);

		// ���C�g�̉e���͈͊O�Ȃ��̌v�Z�����Ȃ��B
        if (lightLength >= pointLightData[i].range)
            continue;

		// �����������Z�o����
        float attenuate = saturate(1.0f - lightLength / pointLightData[i].range);

        lightVector = lightVector / lightLength;
        pointDiffuse += CalcLambertDiffuse(N, lightVector,
																			pointLightData[i].color.rgb, kd.rgb) * attenuate;
        pointSpecular += CalcPhongSpecular(N, lightVector,
																			pointLightData[i].color.rgb, E, shiness, ks.rgb) * attenuate;
    }

	// �X�|�b�g���C�g�̏���
    float3 spotDiffuse = (float3) 0;
    float3 spotSpecular = (float3) 0;
    for (i = 0; i < spotLightCount; ++i)
    {
		// ���C�g�x�N�g�����Z�o
        float3 lightVector = pin.world_position - spotLightData[i].position.xyz;

			// ���C�g�x�N�g���̒������Z�o
        float lightLength = length(lightVector);

        if (lightLength >= spotLightData[i].range)
            continue;

		// �����������Z�o����
        float attenuate = saturate(1.0f - lightLength / spotLightData[i].range);

        lightVector = normalize(lightVector);

		// �p�x�������Z�o����attenuate�ɏ�Z����
        float3 spotDirection = normalize(spotLightData[i].direction.xyz);
        float angle = dot(spotDirection, lightVector);
        float area = spotLightData[i].innerCorn - spotLightData[i].outerCorn;
        attenuate *= saturate(1.0f - (spotLightData[i].innerCorn - angle) / area);

        spotDiffuse += CalcLambertDiffuse(N, lightVector,
																				spotLightData[i].color.rgb, kd.rgb) * attenuate;
        spotSpecular += CalcPhongSpecular(N, lightVector,
																				spotLightData[i].color.rgb, E, shiness, ks.rgb) * attenuate;
    }

    float4 color = float4(ambient, diffuseColor.a);
    color.rgb += diffuseColor.rgb * (directionalDiffuse + pointDiffuse + spotDiffuse);
    color.rgb += directionalSpecular + pointSpecular + spotSpecular;

	//	�������C�e�B���O
	//color.rgb += CalcRimLight(N, E, L, directionalLightData.color.rgb);

    return color;
}
