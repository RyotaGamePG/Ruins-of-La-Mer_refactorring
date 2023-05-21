#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Sprite.h"
#include "Graphics/RenderTarget.h"

// �V�F�[�_�[���\�[�X�r���[�̏��
struct ShaderResourceViewData
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	srv;
	int	width = 0;
	int height = 0;
};

// �|�X�g�v���Z�X�p�̃����_���[
class PostprocessingRenderer
{
	// �u���[�����
	struct BloomData
	{
		//	���P�x���o�p���
		LuminanceExtractionData	luminanceExtractionData;
		//	�K�E�X�u���[�p���
		GaussianFilterData		gaussianFilterData;
	};

public:
	PostprocessingRenderer();
	~PostprocessingRenderer();

	//	�`��
	void Render(ID3D11DeviceContext* deviceContext);

	//	�V�[�����̐ݒ�
	void SetSceneData(ShaderResourceViewData srvData);

	//	�f�o�b�O���̕\��
	void DrawDebugGUI();

private:
	// �V�[���`��f�[�^
	ShaderResourceViewData			sceneData;

	// �`��p�X�v���C�g
	std::unique_ptr<Sprite>			renderSprite;

	// �u���[���f�[�^
	BloomData						bloomData;

	// �F���␳�f�[�^
	ColorGradingData				colorGradingData;

	//	���P�x���o�e�N�X�`��
	std::unique_ptr<RenderTarget>	luminanceExtractRenderTarget;

	//	���P�x���o�ڂ����e�N�X�`��
	std::unique_ptr<RenderTarget>	luminanceExtractBokehRenderTarget;
};
