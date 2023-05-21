//--------------------------------------------
//	�V���h�E�}�b�v�Q�Ɨp���̌v�Z
//--------------------------------------------
// worldPosition		: ���[���h���W
// lightViewProjection	: ���C�g�r���[�v���W�F�N�V�����s��
// �Ԃ��l				: �V���h�E�}�b�v�Q�Ɨp��UV���W�y�ѐ[�x���
float3 CalcShadowTexcoord(float3 worldPosition, matrix lightViewProjection)
{
	// �N���b�v��Ԃ̍��W�ɕϊ�
    float4 worldViewProjectionPosition = mul(float4(worldPosition, 1), lightViewProjection);

	// W���Z����NDC���W�ɕϊ�
    worldViewProjectionPosition /= worldViewProjectionPosition.w;

	// NDC���W����UV���W�ɕϊ�
	// �������AZ�l�͂��ł�0�`1�͈̔͂ɕϊ�����Ă���̂ŁA�������Ȃ��ėǂ�
    worldViewProjectionPosition.y = -worldViewProjectionPosition.y;
    worldViewProjectionPosition.xy = 0.5f * worldViewProjectionPosition.xy + 0.5f;
    return worldViewProjectionPosition.xyz;
}

//--------------------------------------------
//	�V���h�E�}�b�v����[�x�l���擾���ĉe���ǂ�����n��
//--------------------------------------------
// tex				: �V���h�E�}�b�v
// samplerState		: �T���v���X�e�[�g
// shadowTexcoord	: �V���h�E�}�b�v�Q�Ɨp���
// shadowBias		: �[�x��r�p�̃I�t�Z�b�g�l
// shadowColor		: �e�̐F
// �Ԃ��l			: �e�̐F
float3 CalcShadowColor(Texture2D tex, SamplerState samplerState, float3 shadowTexcoord, float3 shadowColor, float shadowBias)
{
	// �V���h�E�}�b�v����[�x�l�擾
    float depth = tex.Sample(samplerState, shadowTexcoord.xy).r;
	// �[�x�l���r���ĉe���ǂ����𔻒肷��
    return lerp(shadowColor, 1, step(shadowTexcoord.z - depth, shadowBias));
}

//--------------------------------------------
//	PCF�t�B���^�[�t���\�t�g�V���h�E�}�b�v
//--------------------------------------------
// tex				: �V���h�E�}�b�v
// samplerState		: �T���v���X�e�[�g
// shadowTexcoord	: �V���h�E�}�b�v�Q�Ɨp���
// shadowBias		: �[�x��r�p�̃I�t�Z�b�g�l
// shadowColor		: �e�̐F
// �Ԃ��l			: �e�̐F
float3 CalcShadowColorPCFFilter(Texture2D tex, SamplerState samplerState, float3 shadowTexcoord, float3 shadowColor, float shadowBias)
{
	// �e�N�Z���T�C�Y�̌v�Z
    float2 texelSize;
	{
		// �e�N�X�`���̏c���������擾����
        uint width, height;
        tex.GetDimensions(width, height);

		// �Z�o
        texelSize = float2(1.0f / width, 1.0f / height);
    }

    float factor = 0;
    static const int PCFKernelSize = 5; //	�w��͊�ɂ��邱��
    for (int x = -PCFKernelSize / 2; x <= PCFKernelSize / 2; ++x)
    {
        for (int y = -PCFKernelSize / 2; y <= PCFKernelSize / 2; ++y)
        {
			// �V���h�E�}�b�v����[�x�l�擾
            float depth = tex.Sample(samplerState, shadowTexcoord.xy + texelSize * float2(x, y)).r;
            factor += step(shadowTexcoord.z - depth, shadowBias);
        }
    }
	// �[�x�l���r���ĉe���ǂ����𔻒肷��
    return lerp(shadowColor, 1, factor / (PCFKernelSize * PCFKernelSize));
}
