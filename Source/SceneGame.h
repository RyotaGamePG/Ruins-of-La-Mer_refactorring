#pragma once

#include "Stage.h"
#include "Wall.h"
#include "Player.h"
#include "EnemyBoss.h"
#include "CameraController.h"
#include "Scene.h"

#include "Audio/Audio.h"

//����
#include "Graphics/Sprite.h"

#include"Graphics/RenderTarget.h"
#include"Graphics/DepthStencil.h"
//#include"Graphics/LightManager.h"
#include"Graphics/Light.h"
#include "PostprocessingRenderer.h"

#include "Graphics/Model.h"
#include "Graphics/Sprite.h"
#include "Graphics/Texture.h"

// �Q�[���V�[��
class SceneGame:public Scene
{
public:
	SceneGame() {}
	~SceneGame() override{}

	// ������
	void Initialize() override;

	// �I����
	void Finalize()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render()override;

	//�Q�[���ʏ폈��
	void GameUpdate(float elapsedTime);
	//�Q�[���N���A����
	void GameClearUpdate(float elapsedTime);
	//�Q�[���I�[�o�[����
	void GameOverUpdate(float elapsedTime);

	//���Z�b�g����
	void Reset();


	//�X�e�[�W�ԍ��ݒ�
	void SetStageNum(int stageNum) { this->stageNum = stageNum; }

	//�Q�[���N���A���̊Ŕݒu
	void SetClearSignBoard(float elapsedTime);
	//�Q�[���I�[�o�[���̊Ŕݒu
	void SetOverSignBoard(float elapsedTime);

	//�ŔI���X�V����
	void selectSignbordUpdate();

	// 3D��Ԃ̕`��
	void Render3DScene();

	// �V���h�E�}�b�v�̕`��
	void RenderShadowmap();

private:
	enum State
	{
		Game,
		Over,
		Clear
	};

	State state = State::Game;

private:
	//Stage* stage = nullptr;
	
	Player* player = nullptr;

	std::unique_ptr<Model> playerModel;
	std::unique_ptr<Model> stage;
	std::unique_ptr<Model> bossModel;

	CameraController* cameraController = nullptr;

	EnemyBoss* boss = nullptr;
	
	//����
	//Sprite* sprite = nullptr;
	float angle = 0.0f;

	//�A�b�v�f�[�g2023�N4��2��
	std::unique_ptr<Texture> texture;

	Wall* wall = nullptr;

	//�N���A�ƃI�[�o�[���ɏo�Ă���Ŕ�
	//�^�C�g������
	GameSelectSignBoard* signBoard[3] = {};


private:
	int stageNum = 0;

	int selectNum = 0;

	bool resetNow = false;

private:
	std::unique_ptr<AudioSource> GameMusic = nullptr;
	std::unique_ptr<AudioSource> OverMusic = nullptr;
	std::unique_ptr<AudioSource> ClearMusic = nullptr;

private:
	Light* lightPlayer = nullptr;
	//UV�X�N���[���f�[�^
	UVScrollData uvScrollData;
	std::unique_ptr<Sprite>	sprite;
	//std::unique_ptr<Texture> texture;
	//�}�X�N�摜
	std::unique_ptr<Texture> maskTexture;
	float					 dissolveThreshold;
	float					 edgeThreshold; 	// ����臒l
	DirectX::XMFLOAT4		 edgeColor;			// ���̐F

		//	�F���␳���
	//ColorGradingData		colorGradingData;
	//	�I�t�X�N���[�������_�����O�p�`��^�[�Q�b�g
	std::unique_ptr<RenderTarget> renderTarget;
	// �K�E�X�t�B���^�[�f�[�^
	//GaussianFilterData			gaussianFilterData;
	//std::unique_ptr<Sprite>		gaussianBlurSprite;
	// �u���[���f�[�^
	//LuminanceExtractionData		luminanceExtractionData;

	//	�V���h�E�}�b�v�p���
	Light* mainDirectionalLight = nullptr;					//	�V���h�E�}�b�v�𐶐����镽�s����
	std::unique_ptr<DepthStencil> shadowmapDepthStencil;	//	�V���h�E�}�b�v�p�[�x�X�e���V���o�b�t�@
	float	shadowDrawRect = 66.157f;						//	�V���h�E�}�b�v�ɕ`�悷��͈�
	DirectX::XMFLOAT4X4	lightViewProjection;				//	���C�g�r���[�v���W�F�N�V�����s��
	DirectX::XMFLOAT3	shadowColor = { 0.0f, 0.0f, 0.0f };	//	�e�̐F
	float				shadowBias = 0.001f;				//	�[�x��r�p�̃I�t�Z�b�g�l

		//	�|�X�g�v���Z�X
	std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;
	//	�p�m���}�X�J�C�{�b�N�X�摜
	std::unique_ptr<Texture>	skyboxTexture;

	bool							drawDebugPrimitive;	//	�f�o�b�O�v���~�e�B�u�`��
	bool							drawLightDebugPrimitive;	//���C�g	�f�o�b�O�`��
	bool							drawPlayerDebugPrimitive;	//	�v���C���[�f�o�b�O�`��
	bool							drawEnemyPrimitive;	//	�G�l�~�[�f�o�b�O�`��
};
