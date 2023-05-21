#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "Camera.h"
#include "WallManager.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EnemyCrab.h"
#include "EnemyScallops.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "FlagAreaManager.h"
#include "StageManager.h"
#include "Collision.h"
#include "EffectManager.h"
#include "Graphics/LightManager.h"

#include <Windows.h>
#include <sstream>
#include <iostream>
#include <string>


//	�V���h�E�}�b�v�̃T�C�Y
static	const	UINT	SHADOWMAP_SIZE = 2048;


// ������
void SceneGame::Initialize()
{
	//playerModel = std::make_unique<Model>("Data/Model/player.mdl");
	//stage = std::make_unique<Model>("Data/model/Enemy/enemywall.mdl");
	//�ʏ��Ԑݒ�
	state = State::Game;
	
	//�X�e�[�W������
	StageManager::Instance().SetStageObj(stageNum);

	//�Ǐ�����
	
	//for (int i = 0; i < 0; ++i)
	//{
	//	Wall* wall = new Wall();
	//	wall->SetPosition(DirectX::XMFLOAT3(2.0f, 0, 30+i));
	//	WallManager::Instance().Register(wall);
	//}
	

	//�Ǐ�����
	WallManager::Instance().SetWall(stageNum);

	//�v���C���[������
	player = new Player();
	player->SetPosition(DirectX::XMFLOAT3(4.5f, 0, 0));
	//�J�����R���g���[���[������
	cameraController = new CameraController();

	//�J�����̏����ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(10, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 1)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);


	//----�{�X-------

	//�{�X������
	boss = new EnemyBoss(stageNum);

	boss->SetPosition(DirectX::XMFLOAT3(4.5f, 0, -15.0f));
	//�ʒu���Z�b�g���߂̓o�^(�v���C���[�̌��)
	boss->PlayerSet(player);
	//�Փˏ����̂��߂ɓo�^
	player->BossSet(boss);

	
	//----�G-------

	EnemyManager::Instance().SetEnemy(stageNum);

	

	//----����G���A-------

	
	//�G���A�}�l�[�W���[������
	FlagAreaManager::Instance().SetPlayer(player);
	FlagAreaManager::Instance().SetBoss(boss);

	FlagAreaManager::Instance().SetFlagArea(stageNum);
	//FlagAreaManager::Instance().SetFlagArea(0);
	//----������-------

	//�X�v���C�g������
	//sprite = new Sprite("Data/Sprite/LoadingIcon.png");
	texture = std::make_unique<Texture>("Data/Sprite/LoadingIcon.png");


	//���y�̃��[�h�ƍĐ�
	GameMusic = Audio::Instance().LoadAudioSource("Data/Audio/BGM/Game.wav", true);
	GameMusic->SetVolume(0.3f);
	GameMusic->Play();

	OverMusic = Audio::Instance().LoadAudioSource("Data/Audio/BGM/GameOver.wav", true);
	OverMusic->SetVolume(0.3f);

	ClearMusic = Audio::Instance().LoadAudioSource("Data/Audio/BGM/GameClear.wav", false);
	ClearMusic->SetVolume(0.3f);


	// ���s������ǉ�
	{
		mainDirectionalLight = new Light(LightType::Directional);
		mainDirectionalLight->SetDirection({ -0.410, -0.246f, 0.878f });
		mainDirectionalLight->SetColor(DirectX::XMFLOAT4(1.00, 0.915, 0.915, 1.0f));
		LightManager::Instane().Register(mainDirectionalLight);
	}
	static	const	int	Range = 500;
	static	const	int	HalfRange = Range / 2;
	// �_������ǉ�
	{
		lightPlayer = new Light(LightType::Point);
		lightPlayer->SetRange(7.00f);
		lightPlayer->SetPosition(DirectX::XMFLOAT3(player->GetPosition().x, player->GetPosition().y , player->GetPosition().z));
		lightPlayer->SetColor(DirectX::XMFLOAT4(0.977, 0.977f, 0.977f, 1.0f));
		LightManager::Instane().Register(lightPlayer);
		for (int i = 0; i < PointLightMax; ++i)
		{
			Light* light = new Light(LightType::Point);
			//light->SetPosition({ static_cast<float>(rand() % Range - HalfRange),
			//					static_cast<float>(rand() % 20 + 20),
			//					static_cast<float>(rand() % Range - HalfRange) });
			//light->SetRange(100);
			//light->SetColor({ static_cast<float>(rand() % 360) / 360,
			//				 static_cast<float>(rand() % 360) / 360,
			//				 static_cast<float>(rand() % 360) / 360,
			//				 1.0f });
			
			//Light* signboardLight = new Light(LightType::Point);
			//lightPlayer->SetRange(50.0f);
			//signboardLight->SetPosition(DirectX::XMFLOAT3(-0.5, 6, 8));
			//signboardLight->SetPosition(DirectX::XMFLOAT3(-0.5, 6, 24));
			//signboardLight->SetPosition(DirectX::XMFLOAT3(-0.5, 6, 37));
			//signboardLight->SetPosition(DirectX::XMFLOAT3(-0.5, 6, 77));
			//lightPlayer->SetColor(DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f));
			//LightManager::Instane().Register(signboardLight);
		}
		Light* signboardLight1 = new Light(LightType::Point);
		Light* signboardLight2 = new Light(LightType::Point);
		Light* signboardLight3 = new Light(LightType::Point);
		signboardLight1->SetPosition(DirectX::XMFLOAT3(1.500, 11.000, 8.000));
		signboardLight1->SetRange(15.200);
		signboardLight1->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		signboardLight2->SetPosition(DirectX::XMFLOAT3(3.500, 12.000, 37.000));
		signboardLight2->SetRange(15.600);
		signboardLight2->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		signboardLight3->SetPosition(DirectX::XMFLOAT3(2.500, 11, 24));
		signboardLight3->SetRange(17.600);
		signboardLight3->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		LightManager::Instane().Register(signboardLight1);
		LightManager::Instane().Register(signboardLight2);
		LightManager::Instane().Register(signboardLight3);
	}
	// �X�|�b�g���C�g��ǉ�
	{
		//Light* light = new Light(LightType::Spot);
		//light->SetPosition(DirectX::XMFLOAT3(-30, 20, 0));
		//light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		//light->SetDirection(DirectX::XMFLOAT3(+1, -1, 0));
		//light->SetRange(40.0f);
		//LightManager::Instane().Register(light);
		for (int i = 0; i < SpotLightMax; ++i)
		{
			/*Light* light = new Light(LightType::Spot);
			light->SetPosition({ static_cast<float>(rand() % Range - HalfRange),
								static_cast<float>(rand() % 60 + 20),
								static_cast<float>(rand() % Range - HalfRange) });
			light->SetRange(200);
			light->SetDirection({ static_cast<float>(rand() % 360) / 360 * 2 - 1,
								 static_cast<float>(rand() % 360) / 360 - 1,
								 static_cast<float>(rand() % 360) / 360 * 2 - 1 });
			light->SetColor({ static_cast<float>(rand() % 360) / 360,
							 static_cast<float>(rand() % 360) / 360,
							 static_cast<float>(rand() % 360) / 360,
							 1.0f });
			LightManager::Instane().Register(light);*/
		}
	}
	// �V�����`��^�[�Q�b�g�̐���
	{
		Graphics& graphics = Graphics::Instance();
		renderTarget = std::make_unique<RenderTarget>(static_cast<UINT>(graphics.GetScreenWidth())
			, static_cast<UINT>(graphics.GetScreenHeight())
			, DXGI_FORMAT_R8G8B8A8_UNORM);
		// �X�v���C�g�ŕ`�悷����̂��V�[���̕`�挋�ʂɕς���
		//sprite = std::make_unique<Sprite>();
		//sprite->SetShaderResourceView(renderTarget->GetShaderResourceView()
		//	, renderTarget->GetWidth()
		//	, renderTarget->GetHeight());
	}
	// �K�E�X�u���[�p�X�v���C�g����
	//{
	//	// �e�N�X�`����ǂݍ���
	//	texture = std::make_unique<Texture>("Data/Texture/1920px-Equirectangular-projection.jpg");
	//	gaussianBlurSprite = std::make_unique<Sprite>();
	//	gaussianBlurSprite->SetShaderResourceView(texture->GetShaderResourceView(), texture->GetWidth(), texture->GetHeight());
	//}

	// �V���h�E�}�b�v�p�ɐ[�x�X�e���V���̐���
	{
		Graphics& graphics = Graphics::Instance();
		shadowmapDepthStencil = std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	}

	// �|�X�g�v���Z�X�`��N���X����
	{
		postprocessingRenderer = std::make_unique<PostprocessingRenderer>();
		// �V�[���e�N�X�`����ݒ肵�Ă���
		ShaderResourceViewData srvData;
		srvData.srv = renderTarget->GetShaderResourceView();
		srvData.width = renderTarget->GetWidth();
		srvData.height = renderTarget->GetHeight();
		postprocessingRenderer->SetSceneData(srvData);
	}
	// �X�J�C�{�b�N�X�e�N�X�`���̓ǂݍ���
	{
		Graphics& graphics = Graphics::Instance();
		skyboxTexture = std::make_unique<Texture>("Data/Texture/1920px-Equirectangular-projection.jpg");
		//skyboxTexture = std::make_unique<Texture>("Data/Texture/incskies_046_16k.png");
		//skyboxTexture = std::make_unique<Texture>("Data/Texture/incskies_044_16k.png");

		sprite = std::make_unique<Sprite>();
		sprite->SetShaderResourceView(skyboxTexture->GetShaderResourceView(), skyboxTexture->GetWidth(), skyboxTexture->GetHeight());
		sprite->Update(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
			0, 0, static_cast<float>(skyboxTexture->GetWidth()), static_cast<float>(skyboxTexture->GetHeight()),
			0,
			1, 1, 1, 1);
	}


	// �f�o�b�O�t���O
	drawDebugPrimitive = true;
	drawLightDebugPrimitive = false;
	drawPlayerDebugPrimitive = false;
	drawEnemyPrimitive = false;
}

// �I����
void SceneGame::Finalize()
{
	//�G�l�~�[�I����
	EnemyManager::Instance().Clear();

	//�{�X�I����
	if (boss != nullptr)
	{
		delete boss;
		boss = nullptr;
	}

	//�J�����R���g���[���[�I����
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//�v���C���[�I����
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	//�ǏI����
	WallManager::Instance().Clear();

	//�G���A�I����
	FlagAreaManager::Instance().Clear();

	//�X�e�[�W�I����
	StageManager::Instance().Clear();

	//�G�I��
	//EnemyManager::Instance().Clear();

	//----����------

	//�X�v���C�g�I����
	//if (sprite != nullptr)
	//{
	//	delete sprite;
	//	sprite = nullptr;
	//}

	//�Z���N�g�ŔI����
	for (int i=0;i<3;i++)
	{
		if (signBoard != nullptr)
		{
			delete signBoard[i];
			signBoard[i] = nullptr;
		}
	}
}

void SceneGame::Reset()
{
	//------------
	//	�I����
	//------------

	//���y��~
	OverMusic->Stop();
	ClearMusic->Stop();


	//�G���A�I����
	FlagAreaManager::Instance().Clear();

	//�G�l�~�[�I����
	EnemyManager::Instance().Clear();

	//�{�X�I����
	if (boss != nullptr)
	{
		delete boss;
		boss = nullptr;
	}

	//�v���C���[�I����
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	//�ǏI����
	WallManager::Instance().Clear();


	//�G�t�F�N�g�}�l�[�W���[�I����
	EffectManager::Instance().Finalize();

	//�Z���N�g�ŔI����
	for (int i = 0; i < 3; i++)
	{
		if (signBoard != nullptr)
		{
			delete signBoard[i];
			signBoard[i] = nullptr;
		}
	}

	//------------
	//	������
	//------------

	//�G�t�F�N�g�}�l�[�W���[������
	EffectManager::Instance().Initialize();

	//�Ǐ�����
	WallManager::Instance().SetWall(stageNum);


	//�v���C���[������
	player = new Player();
	player->SetPosition(DirectX::XMFLOAT3(4.5f, 0, 0));
	

	// �{�X������
	boss = new EnemyBoss(stageNum);
	boss->SetPosition(DirectX::XMFLOAT3(4.5f, 0, -15.0f));
	//�ʒu���Z�b�g���߂̓o�^(�v���C���[�̌��)
	boss->PlayerSet(player);
	//�Փˏ����̂��߂ɓo�^
	player->BossSet(boss);

	//�G�l�~�[������
	EnemyManager::Instance().SetEnemy(stageNum);

	//�G���A�}�l�[�W���[������
	FlagAreaManager::Instance().SetPlayer(player);
	FlagAreaManager::Instance().SetBoss(boss);
	FlagAreaManager::Instance().SetFlagArea(stageNum);


	state = State::Game;
	


	//�Q�[�����y�Đ�
	GameMusic = Audio::Instance().LoadAudioSource("Data/Audio/BGM/Game.wav", true);
	GameMusic->SetVolume(0.3f);
	GameMusic->Play();

	resetNow = false;
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	switch (state)
	{
	case State::Game:
		GameUpdate(elapsedTime);
		break;
	case State::Over:
		GameOverUpdate(elapsedTime);
		break;
	case State::Clear:
		GameClearUpdate(elapsedTime);
		break;
	}

	GamePad& gamePad = Input::Instance().GetGamePad();

	//�G�t�F�N�g�X�V����
	EffectManager::Instance().Update(elapsedTime);
	
}

// �Q�[���ʏ�X�V����
void SceneGame::GameUpdate(float elapsedTime)
{
	//���Z�b�g������Ȃ��Ȃ�
	if (resetNow)return;

//--------�Q�[���I�[�o�[�ڍs�������ݒ�-------------
	if (player->IsGameOver() && state != State::Over)
	{
		//�Q�[�����y��~
		GameMusic->Stop();

		//�Z���N�g�Ŕ�ݒu
		SetOverSignBoard(elapsedTime);

		//�J�����̐ݒ�(�ړ������邽�߂ɌÂ����̂Ǝ��̂��̂̈ʒu��ۑ�)
		DirectX::XMFLOAT3 oldTarget = player->GetPosition();
		oldTarget.y = 2.0f;
		oldTarget.x -= 8.0f;
		//oldTarget.z += 1.0f;
		cameraController->SetOldTarget(oldTarget);

		DirectX::XMFLOAT3 nextTarget = player->GetPosition();
		nextTarget.y = 2.0f;
		nextTarget.x -= 12.0f;
		nextTarget.z += 3.0f;
		cameraController->SetNextTarget(nextTarget);

		state = State::Over;

		//�Q�[���I�[�o�[�Đ�
		OverMusic = Audio::Instance().LoadAudioSource("Data/Audio/BGM/GameOver.wav", true);
		OverMusic->SetVolume(0.3f);
		OverMusic->Play();

		return;
	}

//--------�Q�[���N���A�ڍs�������ݒ�------
	if (player->IsGameClear()&&state!=State::Clear)
	{
		//�Q�[�����y��~
		GameMusic->Stop();
		
		//�Z���N�g�Ŕ�ݒu
		SetClearSignBoard(elapsedTime);

		//�J�����̐ݒ�(�ړ������邽�߂ɌÂ����̂Ǝ��̂��̂̈ʒu��ۑ�)
		DirectX::XMFLOAT3 oldTarget = player->GetPosition();
		oldTarget.y = 2.0f;
		oldTarget.x -= 8.0f;
		//oldTarget.z += 1.0f;
		cameraController->SetOldTarget(oldTarget);

		DirectX::XMFLOAT3 nextTarget = player->GetPosition();
		nextTarget.y = 2.0f;
		nextTarget.x -= 12.0f;
		nextTarget.z += 3.0f;
		cameraController->SetNextTarget(nextTarget);

		state = State::Clear;

		//�Q�[���N���A�Đ�
		ClearMusic = Audio::Instance().LoadAudioSource("Data/Audio/BGM/GameClear.wav", false);
		ClearMusic->SetVolume(0.3f);
		ClearMusic->Play();


		return;
	}

//-------------�ʏ펞--------------

#if EDIT
	//TODO::�Y�ꂸ��
	//--------�G�f�B�^�[���ǂ�--------
	GamePad& gamePad = Input::Instance().GetGamePad();

	//�E�j�z�u
	if (gamePad.GetButtonDown() & GamePad::BTN_B)
	{
		switch (player->enemyType)
		{
		case 0:	//�E�j
		{
			DirectX::XMFLOAT3 ps = player->GetPosition();
			EnemySlime* slime = new EnemySlime();
			//slime->SetPosition(DirectX::XMFLOAT3(ps.x, 0, ps.z-1));
			slime->SetPosition(DirectX::XMFLOAT3(ps.x, 0, ps.z));
			EnemyManager::Instance().Register(slime);
		}
			break;
		case 1:	//���E�J�j
		{
			DirectX::XMFLOAT3 ps = player->GetPosition();
			EnemyCrab* crab = new EnemyCrab(true, true);
			crab->SetPosition(DirectX::XMFLOAT3(ps.x, 0, ps.z));
			crab->setTatgetMovePosition();
			EnemyManager::Instance().Register(crab);
		}
			break;
		case 2: //�㉺�J�j
		{
			DirectX::XMFLOAT3 ps = player->GetPosition();
			EnemyCrab* crab = new EnemyCrab(false, true);
			crab->SetPosition(DirectX::XMFLOAT3(ps.x, 0, ps.z));
			crab->setTatgetMovePosition();
			EnemyManager::Instance().Register(crab);
		}
			break;

		case 3://�z�^�e
		{
			DirectX::XMFLOAT3 ps = player->GetPosition();
			EnemyScallops* scallops = new EnemyScallops;
			scallops->SetPosition(DirectX::XMFLOAT3(ps.x, 0, ps.z));
			EnemyManager::Instance().Register(scallops);
		}
		break;

		case 4://�G��
		{
			DirectX::XMFLOAT3 ps = player->GetPosition();
			Wall* wall = new Wall();
			wall->SetPosition(DirectX::XMFLOAT3(DirectX::XMFLOAT3(6, 0, ps.z+ 2)));
			WallManager::Instance().Register(wall);
		}
		break;

		case 5://�X�s�[�h�A�b�v�G���A
		{

			float length = 2.0f;
			DirectX::XMFLOAT3 ps = player->GetPosition();
			float x = ps.x - length;
			float z = ps.z + length;
			DirectX::XMFLOAT3 posRU = { x,0,z };	//�E��
			x = ps.x + length;
			z = ps.z - length;
			DirectX::XMFLOAT3 posLD = { x,0,z };   //����
			FlagArea* area = new FlagArea(posRU, posLD, FlagArea::Type::SpeedUp);
			FlagAreaManager::Instance().Register(area);
		}
		break;

		case 6://�S�[���G���A
		{

			float length = 2.0f;
			DirectX::XMFLOAT3 ps = player->GetPosition();
			float x = ps.x - length;
			float z = ps.z + length;
			DirectX::XMFLOAT3 posRU = { x,0,z };	//�E��
			x = ps.x + length;
			z = ps.z - length;
			DirectX::XMFLOAT3 posLD = { x,0,z };   //����
			FlagArea* area = new FlagArea(posRU, posLD, FlagArea::Type::Goal);
			FlagAreaManager::Instance().Register(area);
		}
		break;

		}
	}

	if (gamePad.GetButtonDown() & GamePad::BTN_Y)
	{
		WallManager::Instance().SetCollisionFlag();
	}

#endif

	//�J�����R���g���[���[�X�V����
	
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.x = 2.0f;
	target.y = 2.0f;
	target.z += 1.0f;
	float length = 15.0f;
	if (target.z + length > stageGoalPos[0])target.z = stageGoalPos[0] - length;
	cameraController->SetTarget(target);	//�v���C���[�̍���������^�[�Q�b�g�ɐݒ�
	cameraController->GameUpdate(elapsedTime);

	//�X�e�[�W�X�V����
	target = player->GetPosition();
	StageManager::Instance().SetTarget(target);
	StageManager::Instance().Update(elapsedTime);

	//�ǍX�V����
	WallManager::Instance().Update(elapsedTime);

	//�v���C���[�X�V����
	player->Update(elapsedTime);

	lightPlayer->SetPosition(DirectX::XMFLOAT3(player->GetPosition().x, player->GetPosition().y + 0.5, player->GetPosition().z));

	//TODO::�Y�ꂸ��
#if GAME
	//�{�X�X�V����
	boss->Update(elapsedTime);
#endif

	//�G�l�~�[�X�V����
	target = player->GetPosition();
	EnemyManager::Instance().SetTarget(target);
	EnemyManager::Instance().Update(elapsedTime);

	//�G���A�X�V����
	FlagAreaManager::Instance().Update(elapsedTime);

	/*
	//�t���O�n�_�o�^
	if (gamePad.GetButtonDown()& GamePad::BTN_B)
	{
		//�G���A������
		DirectX::XMFLOAT3 ps=player->GetPosition();
		float length = 2.0f;
		DirectX::XMFLOAT3 posRU = { 2,0,ps.z-length };	//�E��
		DirectX::XMFLOAT3 posLD = { -2,0,ps.z+length };   //����
		FlagArea* area = new FlagArea(posRU, posLD, FlagArea::Type::SpeedUp);
		FlagAreaManager::Instance().Register(area);
	}
	*/


}

// �`�揈��
//void SceneGame::Render()
//{
//	Graphics& graphics = Graphics::Instance();
//	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
//	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
//	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
//
//	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
//	FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA(0.0�`1.0)
//	dc->ClearRenderTargetView(rtv, color);
//	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//	dc->OMSetRenderTargets(1, &rtv, dsv);
//
//	// �`�揈��
//	RenderContext rc;
//	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j
//
//	//�J�����p�����[�^�ݒ�
//	Camera& camera = Camera::Instance();
//	rc.view = camera.GetView();
//	rc.projection = camera.GetProjection();
//
//
//	// 3D���f���`��
//	{
//		Shader* shader = graphics.GetShader();
//		shader->Begin(dc, rc);
//		//�X�e�[�W�`��
//		//stage->Render(dc, shader);
//		StageManager::Instance().Render(dc, shader);
//		if (state != State::Clear && state != State::Over)
//		{
//			//�Ǖ`��
//			WallManager::Instance().Render(dc, shader);
//		}
//
//		//�v���C���[�`��
//		player->Render(dc, shader);
//
//		if (state != State::Clear && state != State::Over)
//		{
//			//�{�X�`��
//			boss->Render(dc, shader);
//		}
//
//		if (state != State::Clear)
//		{
//			//�G�l�~�[�`��
//			EnemyManager::Instance().Render(dc, shader);
//
//		}
//
//		//�Z���N�g�Ŕ�
//		if (state == State::Clear||state==State::Over) 
//		{
//			for (int i = 0; i < 3; i++)
//			{
//				signBoard[i]->Render(dc, shader);
//			}
//		}
//
//		//�G�l�~�[�`��
//		shader->End(dc);
//	}
//
//
//	//3D�G�t�F�N�g�`��
//	{
//		if (state != State::Clear && state != State::Over)
//		{
//			EffectManager::Instance().Render(rc.view, rc.projection);
//		}
//	}
//
//	// 3D�f�o�b�O�`��
//	{
//#if EDIT
//		//�ǃf�o�b�O�v���~�e�B�u�`��
//		StageManager::Instance().DrawDebugPrimitive();
//
//		//�v���C���[�f�o�b�O�v���~�e�B�u�`��
//		player->DrawDebugprimitive();
//		
//		// �{�X�f�o�b�O�v���~�e�B�u�`��
//		boss->DrawDebugPrimitive();
//
//		//�G�ǃf�o�b�O�v���~�e�B�u�`��
//		WallManager::Instance().DrawDebugPrimitive();
//
//		//�G�l�~�[�f�o�b�O�v���~�e�B�u�`��
//		EnemyManager::Instance().DrawDebugPrimitive();
//#endif
//
//		//�G���A�f�o�b�O�v���~�e�B�u�`��
//		FlagAreaManager::Instance().DrawDebugPrimitive();
//
//		// ���C�������_���`����s
//		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);
//
//		// �f�o�b�O�����_���`����s
//		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
//	}
//
//	// 2D�X�v���C�g�`��
//	{
//		Graphics& graphics = Graphics::Instance();
//		ID3D11DeviceContext* dc = graphics.GetDeviceContext();
//		ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
//		ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
//
//		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
//		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
//		float textureWidth = static_cast<float>(sprite->GetTextureWidth());
//		float textureHeight = static_cast<float>(sprite->GetTextureHeight());
//		float positionX = screenWidth - textureWidth;
//		float positionY = screenHeight - textureHeight;
//
//
//
//		angle = 1.0;
//
//		/*
//		sprite->Render(dc,
//			positionX - angle, positionY, textureWidth + angle * 10.0f, textureHeight,
//			0, 0, textureWidth, textureHeight,
//			angle,
//			1, 1, 1, 0.5f);
//			*/
//
//		/*
//		sprite->Render(dc,
//			0, 0, 1000, 1000,
//			0, 0, 1000, 0,
//			0,
//			1, 1, 0, 0.5f);
//			*/
//	}
//
//	// 2D�f�o�b�OGUI�`��
//	{
//#if EDIT
//
//		//�v���C���[�f�o�b�O�`��
//		player->DrawDebugGUI();
//
//		boss->DrawDebugGUI();
//
//#endif
//	}
//
//}

//�V�`�揈��
// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

	// �V���h�E�}�b�v�̕`��
	RenderShadowmap();

	// 3D��Ԃ̕`���ʂ̃o�b�t�@�ɑ΂��čs��
	Render3DScene();

	// �������ݐ���o�b�N�o�b�t�@�ɕς��ăI�t�X�N���[�������_�����O�̌��ʂ�`�悷��
	{
		ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

		// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
		FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
		dc->ClearRenderTargetView(rtv, color);
		dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		dc->OMSetRenderTargets(1, &rtv, dsv);

		// �r���[�|�[�g�̐ݒ�
		D3D11_VIEWPORT	vp = {};
		vp.Width = graphics.GetScreenWidth();
		vp.Height = graphics.GetScreenHeight();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		dc->RSSetViewports(1, &vp);


		//	�|�X�g�v���Z�X�������s��
		postprocessingRenderer->Render(dc);

	}

	// �f�o�b�O���̕\��
	{

		ImGui::Checkbox("DebugPrimitive", &drawDebugPrimitive);
		if (drawDebugPrimitive)
		{
			ImGui::Checkbox("LightDebugPrimitive", &drawLightDebugPrimitive);
			ImGui::Checkbox("PlayerDebugPrimitive", &drawPlayerDebugPrimitive);
			ImGui::Checkbox("EnemyDebugPrimitive", &drawEnemyPrimitive);

			if (ImGui::TreeNode("UVScroll"))
			{
				ImGui::SliderFloat2("Scroll Value", &uvScrollData.uvScrollValue.x, -10.0f, 10.0f);
				ImGui::TreePop();
			}
			ImGui::Separator();
			if (ImGui::TreeNode("Mask"))
			{
				ImGui::SliderFloat("Dissolve Threshold", &dissolveThreshold, 0.0f, 1.0f);
				ImGui::SliderFloat("Edge Threshold", &edgeThreshold, 0.0f, 1.0f);
				ImGui::ColorEdit4("Edge Color", &edgeColor.x);
				ImGui::TreePop();
			}
			ImGui::Separator();
			LightManager::Instane().DrawDebugGUI();
			ImGui::Separator();
			//bloom�f�o�b�O
			postprocessingRenderer->DrawDebugGUI();
			if (ImGui::TreeNode("Shadowmap"))
			{
				ImGui::SliderFloat("DrawRect", &shadowDrawRect, 1.0f, 2048.0f);
				ImGui::ColorEdit3("Color", &shadowColor.x);
				ImGui::SliderFloat("Bias", &shadowBias, 0.0f, 0.1f);
				ImGui::Text("texture");
				ImGui::Image(shadowmapDepthStencil->GetShaderResourceView().Get(), { 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });

				ImGui::TreePop();
			}
			ImGui::Separator();

			//�v���C���[�f�o�b�O�`��
			player->DrawDebugGUI();
		}
		ImGui::Separator();

	}

}

//----------------------
//		�Q�[���I�[�o�[
//----------------------

//�Q�[���N���A���̍X�V����
void SceneGame::GameOverUpdate(float elapsedTime)
{
	//�ŔX�V����
	for (int i = 0; i < 3; i++)
	{
		signBoard[i]->Update(elapsedTime);
	}

	//�J�����R���g���[���[�X�V����
	if (!cameraController->isTimeEnd())//�J�����ړ���
	{
		//�ړ����ݒ肵�Ĉړ�
		DirectX::XMFLOAT3 target = cameraController->MoveNextTarget(elapsedTime);
		cameraController->SetTarget(target);
		cameraController->ClearUpdate(elapsedTime);

		//�n�ʂɒ����܂Ńv���C���[�X�V����
		if(!player->IsGround())	//�n�ʂɂ��Ă��Ȃ��Ȃ�
		player->Update(elapsedTime);
	}
	else  //�J�����ړ��I��
	{
		//�ŔI���X�V����
		selectSignbordUpdate();

		//����
		GamePad& gamePad = Input::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_A)
		{
			//���g���C
			if (selectNum == 0) {
				//SceneGame* game = new SceneGame;
				//game->SetStageNum(stageNum);
				//SceneManager::Instance().ChangeScene(new SceneLoading(game));

				Reset();
			}
			else if (selectNum == 1)
			{
				//�Z���N�g�Ɉڍs
				SceneTitle* title = new SceneTitle;
				title->SetSelectState();
				SceneManager::Instance().ChangeScene(new SceneLoading(title));
			}
		}
	}
}

//�N���A�p�̊Ŕݒu
void SceneGame::SetOverSignBoard(float elapsedTime)
{
	//�I���Ŕ����ݒ�
	for (int i = 0; i < 3; i++) {
		signBoard[i] = new GameSelectSignBoard(i+3);
		DirectX::XMFLOAT3 pos = player->GetPosition();
		pos.x += 2;
		pos.y = 0;
		pos.y += 3 - i;
		pos.z += 4.0f;

		if (i == 0)pos.z -= 3.5f;

		//�{���̈ʒu
		//pos.y += 1.0f;
		signBoard[i]->SetNextPos(pos);

		pos.y += 6.0f;
		signBoard[i]->SetOldPos(pos);
		signBoard[i]->SetPosition(pos);

		signBoard[i]->SetMove(true);
	}


	//�J�����R���g���[���[���ԃ��Z�b�g
	cameraController->ResetTime();
}

//----------------------
//		�Q�[���N���A
//----------------------

//�Q�[���N���A���̍X�V����
void SceneGame::GameClearUpdate(float elapsedTime)
{
	//�ŔX�V����
	for (int i = 0; i < 3; i++)
	{
		signBoard[i]->Update(elapsedTime);
	}

	//�J�����R���g���[���[�X�V����
	if (!cameraController->isTimeEnd())//�J�����ړ���
	{
		//�ړ����ݒ肵�Ĉړ�
		DirectX::XMFLOAT3 target = cameraController->MoveNextTarget(elapsedTime);
		cameraController->SetTarget(target);
	    cameraController->ClearUpdate(elapsedTime);

		//�n�ʂɒ����܂Ńv���C���[�X�V����
		if (!player->IsGround())	//�n�ʂɂ��Ă��Ȃ��Ȃ�
		player->Update(elapsedTime);
	}
	else  //�J�����ړ��I��
	{
		//�ŔI���X�V����
		selectSignbordUpdate();

		player->Update(elapsedTime);

		//����
		GamePad& gamePad = Input::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_A)
		{
			//���̃X�e�[�W��
			if (selectNum == 0) {
				SceneGame* game = new SceneGame;
				++stageNum;
				if (stageNum > 2)stageNum = 2;
				game->SetStageNum(stageNum);
				SceneManager::Instance().ChangeScene(new SceneLoading(game));
			}
			else if (selectNum == 1)
			{
				//�Z���N�g�Ɉڍs
				SceneTitle* title = new SceneTitle;
				title->SetSelectState();
				SceneManager::Instance().ChangeScene(new SceneLoading(title));
			}
		}
	}
}

//�N���A�p�̊Ŕݒu
void SceneGame::SetClearSignBoard(float elapsedTime)
{
	//�I���Ŕ����ݒ�
	for (int i = 0; i < 3; i++) {
		signBoard[i] = new GameSelectSignBoard(i);
		DirectX::XMFLOAT3 pos = player->GetPosition();
		pos.x += 2;
		pos.y = 0;
		pos.y += 3-i;
		pos.z += 4.0f;

		if (i == 0)pos.z -= 3.5f;

		//�{���̈ʒu
		//pos.y += 1.0f;
		signBoard[i]->SetNextPos(pos);

		pos.y += 6.0f;
		signBoard[i]->SetOldPos(pos);
		signBoard[i]->SetPosition(pos);

		signBoard[i]->SetMove(true);
	}

	//�J�����R���g���[���[���ԃ��Z�b�g
	cameraController->ResetTime();
}


//----------------------
//		�Ŕ���
//----------------------

//�ŔI���X�V����
void SceneGame::selectSignbordUpdate()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//�ŏ��̃X�P�[���A�j���[�V����
	if(!signBoard[selectNum + 1]->GetScaleMove())
		signBoard[selectNum + 1]->SetScaleMove(true);

	if (selectNum < 1)
	{
		if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)
		{
			signBoard[selectNum+1]->SetScaleMove(false);
			selectNum++;
			signBoard[selectNum+1]->SetScaleMove(true);
		}
	}

	if (selectNum > 0)
	{
		if (gamePad.GetButtonDown() & GamePad::BTN_UP)
		{
			signBoard[selectNum+1]->SetScaleMove(false);
			selectNum--;
			signBoard[selectNum+1]->SetScaleMove(true);
		}
	}
}

void SceneGame::Render3DScene()
{

	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = renderTarget->GetRenderTargetView().Get();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT	vp = {};
	vp.Width = graphics.GetScreenWidth();
	vp.Height = graphics.GetScreenHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// �`�揈��
	RenderContext rc;
	rc.deviceContext = dc;

	// ���C�g�̏����l�ߍ���
	LightManager::Instane().PushRenderContext(rc);

	// �V���h�E�}�b�v�̐ݒ�
	rc.shadowMapData.shadowMap = shadowmapDepthStencil->GetShaderResourceView().Get();
	rc.shadowMapData.lightViewProjection = lightViewProjection;
	rc.shadowMapData.shadowColor = shadowColor;
	rc.shadowMapData.shadowBias = shadowBias;

	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();



	// �X�J�C�{�b�N�X�̕`��
	{
		SpriteShader* shader = graphics.GetShader(SpriteShaderId::Skybox);
		shader->Begin(rc);

		shader->Draw(rc, sprite.get());

		shader->End(rc);
	}

	// 3D���f���`��
	{
		// ModelShader* shader = graphics.GetShader(ModelShaderId::Default);
		ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
		shader->Begin(rc);

		//shader->Draw(rc, stage.get());
		//shader->Draw(rc, playerModel.get());
		//shader->Draw(rc, uncle.get());

		//�X�e�[�W�`��
		StageManager::Instance().ModelRender(rc, shader);
		//�v���C���[�`��
		player->ModelRender(rc, shader);
		//�G�l�~�[�`��
		EnemyManager::Instance().ModelRender(rc, shader);
		boss->ModelRender(rc, shader);
		WallManager::Instance().ModelRender(rc, shader);
		//�Z���N�g�Ŕ�
		if (state == State::Clear || state == State::Over)
		{
			for (int i = 0; i < 3; i++)
			{
				signBoard[i]->ModelRender(rc, shader);
			}
		}
		shader->End(rc);
	}

	//3D�G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	{
		ModelShader* shader = graphics.GetShader(ModelShaderId::Default);
		shader->Begin(rc);
		WallManager::Instance().ModelRender(rc, shader);
		shader->End(rc);
	}

	
}


// �V���h�E�}�b�v�̕`��
void SceneGame::RenderShadowmap()
{

	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11DepthStencilView* dsv = shadowmapDepthStencil->GetDepthStencilView().Get();

	// ��ʃN���A
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (!mainDirectionalLight)
		return;

	// �����_�[�^�[�Q�b�g�ݒ�
	dc->OMSetRenderTargets(0, &rtv, dsv);

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT	vp = {};
	vp.Width = static_cast<float>(shadowmapDepthStencil->GetWidth());
	vp.Height = static_cast<float>(shadowmapDepthStencil->GetHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// �`�揈��
	RenderContext rc;
	rc.deviceContext = dc;

	// �J�����p�����[�^�ݒ�
	{
		// ���s��������J�����ʒu���쐬���A�������猴�_�̈ʒu������悤�Ɏ����s��𐶐�
		DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat3(&mainDirectionalLight->GetDirection());
		LightPosition = DirectX::XMVectorScale(LightPosition, -250.0f);
		DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(LightPosition,
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

		// �V���h�E�}�b�v�ɕ`�悵�����͈͂̎ˉe�s��𐶐�
		DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowDrawRect, shadowDrawRect, 0.1f, 1000.0f);
		DirectX::XMStoreFloat4x4(&rc.view, V);
		DirectX::XMStoreFloat4x4(&rc.projection, P);

		DirectX::XMStoreFloat4x4(&lightViewProjection, V * P);
	}

	// 3D���f���`��
	{

		ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
		shader->Begin(rc);

		//shader->Draw(rc, jummo.get());

		//�X�e�[�W�`��
		StageManager::Instance().ModelRender(rc, shader);
		//�v���C���[�`��
		player->ModelRender(rc, shader);
		//�G�l�~�[�`��
		WallManager::Instance().ModelRender(rc, shader);
		boss->ModelRender(rc, shader);
		EnemyManager::Instance().ModelRender(rc, shader);
		OutputDebugStringA("�G���[�`�F�b�N�ł��I�I�I�I�I�I�I\n");

		//�Z���N�g�Ŕ�
		if (state == State::Clear || state == State::Over)
		{
			for (int i = 0; i < 3; i++)
			{
				signBoard[i]->ModelRender(rc, shader);
			}
		}

		shader->End(rc);
	}
}
