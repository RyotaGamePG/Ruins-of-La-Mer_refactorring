#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

//	���s�������
struct DirectionalLightData
{
	DirectX::XMFLOAT4	direction;	// ����
	DirectX::XMFLOAT4	color;		// �F
};

// �_�������
struct PointLightData
{
	DirectX::XMFLOAT4	position;	// ���W
	DirectX::XMFLOAT4	color;		// �F
	float			range;		// �͈�
	DirectX::XMFLOAT3	dummy;
};
// �_�����̍ő吔
static	constexpr	int	PointLightMax = 8;


// �X�|�b�g���C�g���
struct SpotLightData
{
	DirectX::XMFLOAT4	position;	// ���W
	DirectX::XMFLOAT4	direction;	// ����
	DirectX::XMFLOAT4	color;		// �F
	float			range;		// �͈�
	float			innerCorn; 	// �C���i�[�p�x�͈�
	float			outerCorn; 	// �A�E�^�[�p�x�͈�
	float			dummy;
};
// �X�|�b�g���C�g�̍ő吔
static	constexpr	int	SpotLightMax = 8;



//	UV�X�N���[�����
struct UVScrollData
{
	DirectX::XMFLOAT2	uvScrollValue;	// UV�X�N���[���l
};

//�}�X�N�f�[�^
struct MaskData
{
	ID3D11ShaderResourceView* maskTexture;
	float					  dissolveThreshold;
	float					  edgeThreshold;		//����臒l
	DirectX::XMFLOAT4		  edgeColor;			//���̐F
};

// �F���␳���
struct ColorGradingData
{
	float	hueShift = 0;	// �F������
	float	saturation = 1.252f;	// �ʓx����
	float	brightness = 0.886f;	// ���x����
	float	dummy;
};

// �K�E�X�t�B���^�[�v�Z���
struct GaussianFilterData
{
	int					kernelSize = 15;		// �J�[�l���T�C�Y
	float				deviation = 9.451;		// �W���΍�
	DirectX::XMFLOAT2	textureSize;			// �򂷃e�N�X�`���̃T�C�Y
};
// �K�E�X�t�B���^�[�̍ő�J�[�l���T�C�Y
static const int MaxKernelSize = 16;

// ���P�x���o�p���
struct LuminanceExtractionData
{
	float				threshold = 0.737f;	// 臒l
	float				intensity = 3.846f;	// �u���[���̋��x
	DirectX::XMFLOAT2	dummy2;
};

// �|�X�g�G�t�F�N�g�̍ŏI�p�X�p���
struct FinalpassnData
{
	//	�u���[���e�N�X�`��
	ID3D11ShaderResourceView* bloomTexture;
};

//	�V���h�E�}�b�v�p���
struct ShadowMapData
{
	ID3D11ShaderResourceView* shadowMap;				//	�V���h�E�}�b�v�e�N�X�`��
	DirectX::XMFLOAT4X4			lightViewProjection;	//	���C�g�r���[�v���W�F�N�V�����s��
	DirectX::XMFLOAT3			shadowColor;			//	�e�̐F
	float						shadowBias;				//	�[�x��r�p�̃I�t�Z�b�g�l
};


// �����_�[�R���e�L�X�g
struct RenderContext
{
	ID3D11DeviceContext* deviceContext;

	//	�J�������
	DirectX::XMFLOAT4		viewPosition;
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;

	//	���C�g���
	DirectX::XMFLOAT4		ambientLightColor;
	DirectionalLightData	directionalLightData;
	PointLightData			pointLightData[PointLightMax];	// �_�������
	SpotLightData			spotLightData[SpotLightMax];	// �X�|�b�g���C�g���
	int						pointLightCount = 0;			// �_������
	int						spotLightCount = 0;				// �X�|�b�g���C�g��

	//	�X�N���[���f�[�^
	UVScrollData			uvScrollData;
	//�}�X�N�f�[�^
	MaskData				maskData;
	//	�F���␳���
	ColorGradingData		colorGradingData;
	//	�K�E�X�t�B���^�[���
	GaussianFilterData		gaussianFilterData;
	//	���P�x���o�p���
	LuminanceExtractionData	luminanceExtractionData;
	//	�ŏI�p�X���
	FinalpassnData			finalpassnData;
	//	�V���h�E�}�b�v���
	ShadowMapData			shadowMapData;

};