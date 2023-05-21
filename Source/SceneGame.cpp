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


//	シャドウマップのサイズ
static	const	UINT	SHADOWMAP_SIZE = 2048;


// 初期化
void SceneGame::Initialize()
{
	//playerModel = std::make_unique<Model>("Data/Model/player.mdl");
	//stage = std::make_unique<Model>("Data/model/Enemy/enemywall.mdl");
	//通常状態設定
	state = State::Game;
	
	//ステージ初期化
	StageManager::Instance().SetStageObj(stageNum);

	//壁初期化
	
	//for (int i = 0; i < 0; ++i)
	//{
	//	Wall* wall = new Wall();
	//	wall->SetPosition(DirectX::XMFLOAT3(2.0f, 0, 30+i));
	//	WallManager::Instance().Register(wall);
	//}
	

	//壁初期化
	WallManager::Instance().SetWall(stageNum);

	//プレイヤー初期化
	player = new Player();
	player->SetPosition(DirectX::XMFLOAT3(4.5f, 0, 0));
	//カメラコントローラー初期化
	cameraController = new CameraController();

	//カメラの初期設定
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


	//----ボス-------

	//ボス初期化
	boss = new EnemyBoss(stageNum);

	boss->SetPosition(DirectX::XMFLOAT3(4.5f, 0, -15.0f));
	//位置リセットための登録(プレイヤーの後ろ)
	boss->PlayerSet(player);
	//衝突処理のために登録
	player->BossSet(boss);

	
	//----敵-------

	EnemyManager::Instance().SetEnemy(stageNum);

	

	//----判定エリア-------

	
	//エリアマネージャー初期化
	FlagAreaManager::Instance().SetPlayer(player);
	FlagAreaManager::Instance().SetBoss(boss);

	FlagAreaManager::Instance().SetFlagArea(stageNum);
	//FlagAreaManager::Instance().SetFlagArea(0);
	//----お試し-------

	//スプライト初期化
	//sprite = new Sprite("Data/Sprite/LoadingIcon.png");
	texture = std::make_unique<Texture>("Data/Sprite/LoadingIcon.png");


	//音楽のロードと再生
	GameMusic = Audio::Instance().LoadAudioSource("Data/Audio/BGM/Game.wav", true);
	GameMusic->SetVolume(0.3f);
	GameMusic->Play();

	OverMusic = Audio::Instance().LoadAudioSource("Data/Audio/BGM/GameOver.wav", true);
	OverMusic->SetVolume(0.3f);

	ClearMusic = Audio::Instance().LoadAudioSource("Data/Audio/BGM/GameClear.wav", false);
	ClearMusic->SetVolume(0.3f);


	// 平行光源を追加
	{
		mainDirectionalLight = new Light(LightType::Directional);
		mainDirectionalLight->SetDirection({ -0.410, -0.246f, 0.878f });
		mainDirectionalLight->SetColor(DirectX::XMFLOAT4(1.00, 0.915, 0.915, 1.0f));
		LightManager::Instane().Register(mainDirectionalLight);
	}
	static	const	int	Range = 500;
	static	const	int	HalfRange = Range / 2;
	// 点光源を追加
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
	// スポットライトを追加
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
	// 新しい描画ターゲットの生成
	{
		Graphics& graphics = Graphics::Instance();
		renderTarget = std::make_unique<RenderTarget>(static_cast<UINT>(graphics.GetScreenWidth())
			, static_cast<UINT>(graphics.GetScreenHeight())
			, DXGI_FORMAT_R8G8B8A8_UNORM);
		// スプライトで描画するものをシーンの描画結果に変える
		//sprite = std::make_unique<Sprite>();
		//sprite->SetShaderResourceView(renderTarget->GetShaderResourceView()
		//	, renderTarget->GetWidth()
		//	, renderTarget->GetHeight());
	}
	// ガウスブラー用スプライト生成
	//{
	//	// テクスチャを読み込む
	//	texture = std::make_unique<Texture>("Data/Texture/1920px-Equirectangular-projection.jpg");
	//	gaussianBlurSprite = std::make_unique<Sprite>();
	//	gaussianBlurSprite->SetShaderResourceView(texture->GetShaderResourceView(), texture->GetWidth(), texture->GetHeight());
	//}

	// シャドウマップ用に深度ステンシルの生成
	{
		Graphics& graphics = Graphics::Instance();
		shadowmapDepthStencil = std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	}

	// ポストプロセス描画クラス生成
	{
		postprocessingRenderer = std::make_unique<PostprocessingRenderer>();
		// シーンテクスチャを設定しておく
		ShaderResourceViewData srvData;
		srvData.srv = renderTarget->GetShaderResourceView();
		srvData.width = renderTarget->GetWidth();
		srvData.height = renderTarget->GetHeight();
		postprocessingRenderer->SetSceneData(srvData);
	}
	// スカイボックステクスチャの読み込み
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


	// デバッグフラグ
	drawDebugPrimitive = true;
	drawLightDebugPrimitive = false;
	drawPlayerDebugPrimitive = false;
	drawEnemyPrimitive = false;
}

// 終了化
void SceneGame::Finalize()
{
	//エネミー終了化
	EnemyManager::Instance().Clear();

	//ボス終了化
	if (boss != nullptr)
	{
		delete boss;
		boss = nullptr;
	}

	//カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//プレイヤー終了化
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	//壁終了化
	WallManager::Instance().Clear();

	//エリア終了化
	FlagAreaManager::Instance().Clear();

	//ステージ終了化
	StageManager::Instance().Clear();

	//敵終了
	//EnemyManager::Instance().Clear();

	//----試し------

	//スプライト終了化
	//if (sprite != nullptr)
	//{
	//	delete sprite;
	//	sprite = nullptr;
	//}

	//セレクト看板終了化
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
	//	終了化
	//------------

	//音楽停止
	OverMusic->Stop();
	ClearMusic->Stop();


	//エリア終了化
	FlagAreaManager::Instance().Clear();

	//エネミー終了化
	EnemyManager::Instance().Clear();

	//ボス終了化
	if (boss != nullptr)
	{
		delete boss;
		boss = nullptr;
	}

	//プレイヤー終了化
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	//壁終了化
	WallManager::Instance().Clear();


	//エフェクトマネージャー終了化
	EffectManager::Instance().Finalize();

	//セレクト看板終了化
	for (int i = 0; i < 3; i++)
	{
		if (signBoard != nullptr)
		{
			delete signBoard[i];
			signBoard[i] = nullptr;
		}
	}

	//------------
	//	初期化
	//------------

	//エフェクトマネージャー初期化
	EffectManager::Instance().Initialize();

	//壁初期化
	WallManager::Instance().SetWall(stageNum);


	//プレイヤー初期化
	player = new Player();
	player->SetPosition(DirectX::XMFLOAT3(4.5f, 0, 0));
	

	// ボス初期化
	boss = new EnemyBoss(stageNum);
	boss->SetPosition(DirectX::XMFLOAT3(4.5f, 0, -15.0f));
	//位置リセットための登録(プレイヤーの後ろ)
	boss->PlayerSet(player);
	//衝突処理のために登録
	player->BossSet(boss);

	//エネミー初期化
	EnemyManager::Instance().SetEnemy(stageNum);

	//エリアマネージャー初期化
	FlagAreaManager::Instance().SetPlayer(player);
	FlagAreaManager::Instance().SetBoss(boss);
	FlagAreaManager::Instance().SetFlagArea(stageNum);


	state = State::Game;
	


	//ゲーム音楽再生
	GameMusic = Audio::Instance().LoadAudioSource("Data/Audio/BGM/Game.wav", true);
	GameMusic->SetVolume(0.3f);
	GameMusic->Play();

	resetNow = false;
}

// 更新処理
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

	//エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);
	
}

// ゲーム通常更新処理
void SceneGame::GameUpdate(float elapsedTime)
{
	//リセット中じゃないなら
	if (resetNow)return;

//--------ゲームオーバー移行＆初期設定-------------
	if (player->IsGameOver() && state != State::Over)
	{
		//ゲーム音楽停止
		GameMusic->Stop();

		//セレクト看板を設置
		SetOverSignBoard(elapsedTime);

		//カメラの設定(移動をするために古いものと次のものの位置を保存)
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

		//ゲームオーバー再生
		OverMusic = Audio::Instance().LoadAudioSource("Data/Audio/BGM/GameOver.wav", true);
		OverMusic->SetVolume(0.3f);
		OverMusic->Play();

		return;
	}

//--------ゲームクリア移行＆初期設定------
	if (player->IsGameClear()&&state!=State::Clear)
	{
		//ゲーム音楽停止
		GameMusic->Stop();
		
		//セレクト看板を設置
		SetClearSignBoard(elapsedTime);

		//カメラの設定(移動をするために古いものと次のものの位置を保存)
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

		//ゲームクリア再生
		ClearMusic = Audio::Instance().LoadAudioSource("Data/Audio/BGM/GameClear.wav", false);
		ClearMusic->SetVolume(0.3f);
		ClearMusic->Play();


		return;
	}

//-------------通常時--------------

#if EDIT
	//TODO::忘れずに
	//--------エディターもどき--------
	GamePad& gamePad = Input::Instance().GetGamePad();

	//ウニ配置
	if (gamePad.GetButtonDown() & GamePad::BTN_B)
	{
		switch (player->enemyType)
		{
		case 0:	//ウニ
		{
			DirectX::XMFLOAT3 ps = player->GetPosition();
			EnemySlime* slime = new EnemySlime();
			//slime->SetPosition(DirectX::XMFLOAT3(ps.x, 0, ps.z-1));
			slime->SetPosition(DirectX::XMFLOAT3(ps.x, 0, ps.z));
			EnemyManager::Instance().Register(slime);
		}
			break;
		case 1:	//左右カニ
		{
			DirectX::XMFLOAT3 ps = player->GetPosition();
			EnemyCrab* crab = new EnemyCrab(true, true);
			crab->SetPosition(DirectX::XMFLOAT3(ps.x, 0, ps.z));
			crab->setTatgetMovePosition();
			EnemyManager::Instance().Register(crab);
		}
			break;
		case 2: //上下カニ
		{
			DirectX::XMFLOAT3 ps = player->GetPosition();
			EnemyCrab* crab = new EnemyCrab(false, true);
			crab->SetPosition(DirectX::XMFLOAT3(ps.x, 0, ps.z));
			crab->setTatgetMovePosition();
			EnemyManager::Instance().Register(crab);
		}
			break;

		case 3://ホタテ
		{
			DirectX::XMFLOAT3 ps = player->GetPosition();
			EnemyScallops* scallops = new EnemyScallops;
			scallops->SetPosition(DirectX::XMFLOAT3(ps.x, 0, ps.z));
			EnemyManager::Instance().Register(scallops);
		}
		break;

		case 4://敵壁
		{
			DirectX::XMFLOAT3 ps = player->GetPosition();
			Wall* wall = new Wall();
			wall->SetPosition(DirectX::XMFLOAT3(DirectX::XMFLOAT3(6, 0, ps.z+ 2)));
			WallManager::Instance().Register(wall);
		}
		break;

		case 5://スピードアップエリア
		{

			float length = 2.0f;
			DirectX::XMFLOAT3 ps = player->GetPosition();
			float x = ps.x - length;
			float z = ps.z + length;
			DirectX::XMFLOAT3 posRU = { x,0,z };	//右上
			x = ps.x + length;
			z = ps.z - length;
			DirectX::XMFLOAT3 posLD = { x,0,z };   //左下
			FlagArea* area = new FlagArea(posRU, posLD, FlagArea::Type::SpeedUp);
			FlagAreaManager::Instance().Register(area);
		}
		break;

		case 6://ゴールエリア
		{

			float length = 2.0f;
			DirectX::XMFLOAT3 ps = player->GetPosition();
			float x = ps.x - length;
			float z = ps.z + length;
			DirectX::XMFLOAT3 posRU = { x,0,z };	//右上
			x = ps.x + length;
			z = ps.z - length;
			DirectX::XMFLOAT3 posLD = { x,0,z };   //左下
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

	//カメラコントローラー更新処理
	
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.x = 2.0f;
	target.y = 2.0f;
	target.z += 1.0f;
	float length = 15.0f;
	if (target.z + length > stageGoalPos[0])target.z = stageGoalPos[0] - length;
	cameraController->SetTarget(target);	//プレイヤーの腰あたりをターゲットに設定
	cameraController->GameUpdate(elapsedTime);

	//ステージ更新処理
	target = player->GetPosition();
	StageManager::Instance().SetTarget(target);
	StageManager::Instance().Update(elapsedTime);

	//壁更新処理
	WallManager::Instance().Update(elapsedTime);

	//プレイヤー更新処理
	player->Update(elapsedTime);

	lightPlayer->SetPosition(DirectX::XMFLOAT3(player->GetPosition().x, player->GetPosition().y + 0.5, player->GetPosition().z));

	//TODO::忘れずに
#if GAME
	//ボス更新処理
	boss->Update(elapsedTime);
#endif

	//エネミー更新処理
	target = player->GetPosition();
	EnemyManager::Instance().SetTarget(target);
	EnemyManager::Instance().Update(elapsedTime);

	//エリア更新処理
	FlagAreaManager::Instance().Update(elapsedTime);

	/*
	//フラグ地点登録
	if (gamePad.GetButtonDown()& GamePad::BTN_B)
	{
		//エリア初期化
		DirectX::XMFLOAT3 ps=player->GetPosition();
		float length = 2.0f;
		DirectX::XMFLOAT3 posRU = { 2,0,ps.z-length };	//右上
		DirectX::XMFLOAT3 posLD = { -2,0,ps.z+length };   //左下
		FlagArea* area = new FlagArea(posRU, posLD, FlagArea::Type::SpeedUp);
		FlagAreaManager::Instance().Register(area);
	}
	*/


}

// 描画処理
//void SceneGame::Render()
//{
//	Graphics& graphics = Graphics::Instance();
//	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
//	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
//	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
//
//	// 画面クリア＆レンダーターゲット設定
//	FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA(0.0～1.0)
//	dc->ClearRenderTargetView(rtv, color);
//	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//	dc->OMSetRenderTargets(1, &rtv, dsv);
//
//	// 描画処理
//	RenderContext rc;
//	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）
//
//	//カメラパラメータ設定
//	Camera& camera = Camera::Instance();
//	rc.view = camera.GetView();
//	rc.projection = camera.GetProjection();
//
//
//	// 3Dモデル描画
//	{
//		Shader* shader = graphics.GetShader();
//		shader->Begin(dc, rc);
//		//ステージ描画
//		//stage->Render(dc, shader);
//		StageManager::Instance().Render(dc, shader);
//		if (state != State::Clear && state != State::Over)
//		{
//			//壁描画
//			WallManager::Instance().Render(dc, shader);
//		}
//
//		//プレイヤー描画
//		player->Render(dc, shader);
//
//		if (state != State::Clear && state != State::Over)
//		{
//			//ボス描画
//			boss->Render(dc, shader);
//		}
//
//		if (state != State::Clear)
//		{
//			//エネミー描画
//			EnemyManager::Instance().Render(dc, shader);
//
//		}
//
//		//セレクト看板
//		if (state == State::Clear||state==State::Over) 
//		{
//			for (int i = 0; i < 3; i++)
//			{
//				signBoard[i]->Render(dc, shader);
//			}
//		}
//
//		//エネミー描画
//		shader->End(dc);
//	}
//
//
//	//3Dエフェクト描画
//	{
//		if (state != State::Clear && state != State::Over)
//		{
//			EffectManager::Instance().Render(rc.view, rc.projection);
//		}
//	}
//
//	// 3Dデバッグ描画
//	{
//#if EDIT
//		//壁デバッグプリミティブ描画
//		StageManager::Instance().DrawDebugPrimitive();
//
//		//プレイヤーデバッグプリミティブ描画
//		player->DrawDebugprimitive();
//		
//		// ボスデバッグプリミティブ描画
//		boss->DrawDebugPrimitive();
//
//		//敵壁デバッグプリミティブ描画
//		WallManager::Instance().DrawDebugPrimitive();
//
//		//エネミーデバッグプリミティブ描画
//		EnemyManager::Instance().DrawDebugPrimitive();
//#endif
//
//		//エリアデバッグプリミティブ描画
//		FlagAreaManager::Instance().DrawDebugPrimitive();
//
//		// ラインレンダラ描画実行
//		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);
//
//		// デバッグレンダラ描画実行
//		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
//	}
//
//	// 2Dスプライト描画
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
//	// 2DデバッグGUI描画
//	{
//#if EDIT
//
//		//プレイヤーデバッグ描画
//		player->DrawDebugGUI();
//
//		boss->DrawDebugGUI();
//
//#endif
//	}
//
//}

//新描画処理
// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

	// シャドウマップの描画
	RenderShadowmap();

	// 3D空間の描画を別のバッファに対して行う
	Render3DScene();

	// 書き込み先をバックバッファに変えてオフスクリーンレンダリングの結果を描画する
	{
		ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

		// 画面クリア＆レンダーターゲット設定
		FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
		dc->ClearRenderTargetView(rtv, color);
		dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		dc->OMSetRenderTargets(1, &rtv, dsv);

		// ビューポートの設定
		D3D11_VIEWPORT	vp = {};
		vp.Width = graphics.GetScreenWidth();
		vp.Height = graphics.GetScreenHeight();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		dc->RSSetViewports(1, &vp);


		//	ポストプロセス処理を行う
		postprocessingRenderer->Render(dc);

	}

	// デバッグ情報の表示
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
			//bloomデバッグ
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

			//プレイヤーデバッグ描画
			player->DrawDebugGUI();
		}
		ImGui::Separator();

	}

}

//----------------------
//		ゲームオーバー
//----------------------

//ゲームクリア時の更新処理
void SceneGame::GameOverUpdate(float elapsedTime)
{
	//看板更新処理
	for (int i = 0; i < 3; i++)
	{
		signBoard[i]->Update(elapsedTime);
	}

	//カメラコントローラー更新処理
	if (!cameraController->isTimeEnd())//カメラ移動中
	{
		//移動先を設定して移動
		DirectX::XMFLOAT3 target = cameraController->MoveNextTarget(elapsedTime);
		cameraController->SetTarget(target);
		cameraController->ClearUpdate(elapsedTime);

		//地面に着くまでプレイヤー更新処理
		if(!player->IsGround())	//地面についていないなら
		player->Update(elapsedTime);
	}
	else  //カメラ移動終了
	{
		//看板選択更新処理
		selectSignbordUpdate();

		//次の
		GamePad& gamePad = Input::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_A)
		{
			//リトライ
			if (selectNum == 0) {
				//SceneGame* game = new SceneGame;
				//game->SetStageNum(stageNum);
				//SceneManager::Instance().ChangeScene(new SceneLoading(game));

				Reset();
			}
			else if (selectNum == 1)
			{
				//セレクトに移行
				SceneTitle* title = new SceneTitle;
				title->SetSelectState();
				SceneManager::Instance().ChangeScene(new SceneLoading(title));
			}
		}
	}
}

//クリア用の看板設置
void SceneGame::SetOverSignBoard(float elapsedTime)
{
	//選択看板初期設定
	for (int i = 0; i < 3; i++) {
		signBoard[i] = new GameSelectSignBoard(i+3);
		DirectX::XMFLOAT3 pos = player->GetPosition();
		pos.x += 2;
		pos.y = 0;
		pos.y += 3 - i;
		pos.z += 4.0f;

		if (i == 0)pos.z -= 3.5f;

		//本来の位置
		//pos.y += 1.0f;
		signBoard[i]->SetNextPos(pos);

		pos.y += 6.0f;
		signBoard[i]->SetOldPos(pos);
		signBoard[i]->SetPosition(pos);

		signBoard[i]->SetMove(true);
	}


	//カメラコントローラー時間リセット
	cameraController->ResetTime();
}

//----------------------
//		ゲームクリア
//----------------------

//ゲームクリア時の更新処理
void SceneGame::GameClearUpdate(float elapsedTime)
{
	//看板更新処理
	for (int i = 0; i < 3; i++)
	{
		signBoard[i]->Update(elapsedTime);
	}

	//カメラコントローラー更新処理
	if (!cameraController->isTimeEnd())//カメラ移動中
	{
		//移動先を設定して移動
		DirectX::XMFLOAT3 target = cameraController->MoveNextTarget(elapsedTime);
		cameraController->SetTarget(target);
	    cameraController->ClearUpdate(elapsedTime);

		//地面に着くまでプレイヤー更新処理
		if (!player->IsGround())	//地面についていないなら
		player->Update(elapsedTime);
	}
	else  //カメラ移動終了
	{
		//看板選択更新処理
		selectSignbordUpdate();

		player->Update(elapsedTime);

		//次の
		GamePad& gamePad = Input::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_A)
		{
			//次のステージへ
			if (selectNum == 0) {
				SceneGame* game = new SceneGame;
				++stageNum;
				if (stageNum > 2)stageNum = 2;
				game->SetStageNum(stageNum);
				SceneManager::Instance().ChangeScene(new SceneLoading(game));
			}
			else if (selectNum == 1)
			{
				//セレクトに移行
				SceneTitle* title = new SceneTitle;
				title->SetSelectState();
				SceneManager::Instance().ChangeScene(new SceneLoading(title));
			}
		}
	}
}

//クリア用の看板設置
void SceneGame::SetClearSignBoard(float elapsedTime)
{
	//選択看板初期設定
	for (int i = 0; i < 3; i++) {
		signBoard[i] = new GameSelectSignBoard(i);
		DirectX::XMFLOAT3 pos = player->GetPosition();
		pos.x += 2;
		pos.y = 0;
		pos.y += 3-i;
		pos.z += 4.0f;

		if (i == 0)pos.z -= 3.5f;

		//本来の位置
		//pos.y += 1.0f;
		signBoard[i]->SetNextPos(pos);

		pos.y += 6.0f;
		signBoard[i]->SetOldPos(pos);
		signBoard[i]->SetPosition(pos);

		signBoard[i]->SetMove(true);
	}

	//カメラコントローラー時間リセット
	cameraController->ResetTime();
}


//----------------------
//		看板処理
//----------------------

//看板選択更新処理
void SceneGame::selectSignbordUpdate()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//最初のスケールアニメーション
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

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// ビューポートの設定
	D3D11_VIEWPORT	vp = {};
	vp.Width = graphics.GetScreenWidth();
	vp.Height = graphics.GetScreenHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// 描画処理
	RenderContext rc;
	rc.deviceContext = dc;

	// ライトの情報を詰め込む
	LightManager::Instane().PushRenderContext(rc);

	// シャドウマップの設定
	rc.shadowMapData.shadowMap = shadowmapDepthStencil->GetShaderResourceView().Get();
	rc.shadowMapData.lightViewProjection = lightViewProjection;
	rc.shadowMapData.shadowColor = shadowColor;
	rc.shadowMapData.shadowBias = shadowBias;

	// カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();



	// スカイボックスの描画
	{
		SpriteShader* shader = graphics.GetShader(SpriteShaderId::Skybox);
		shader->Begin(rc);

		shader->Draw(rc, sprite.get());

		shader->End(rc);
	}

	// 3Dモデル描画
	{
		// ModelShader* shader = graphics.GetShader(ModelShaderId::Default);
		ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
		shader->Begin(rc);

		//shader->Draw(rc, stage.get());
		//shader->Draw(rc, playerModel.get());
		//shader->Draw(rc, uncle.get());

		//ステージ描画
		StageManager::Instance().ModelRender(rc, shader);
		//プレイヤー描画
		player->ModelRender(rc, shader);
		//エネミー描画
		EnemyManager::Instance().ModelRender(rc, shader);
		boss->ModelRender(rc, shader);
		WallManager::Instance().ModelRender(rc, shader);
		//セレクト看板
		if (state == State::Clear || state == State::Over)
		{
			for (int i = 0; i < 3; i++)
			{
				signBoard[i]->ModelRender(rc, shader);
			}
		}
		shader->End(rc);
	}

	//3Dエフェクト描画
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


// シャドウマップの描画
void SceneGame::RenderShadowmap()
{

	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11DepthStencilView* dsv = shadowmapDepthStencil->GetDepthStencilView().Get();

	// 画面クリア
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (!mainDirectionalLight)
		return;

	// レンダーターゲット設定
	dc->OMSetRenderTargets(0, &rtv, dsv);

	// ビューポートの設定
	D3D11_VIEWPORT	vp = {};
	vp.Width = static_cast<float>(shadowmapDepthStencil->GetWidth());
	vp.Height = static_cast<float>(shadowmapDepthStencil->GetHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// 描画処理
	RenderContext rc;
	rc.deviceContext = dc;

	// カメラパラメータ設定
	{
		// 平行光源からカメラ位置を作成し、そこから原点の位置を見るように視線行列を生成
		DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat3(&mainDirectionalLight->GetDirection());
		LightPosition = DirectX::XMVectorScale(LightPosition, -250.0f);
		DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(LightPosition,
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

		// シャドウマップに描画したい範囲の射影行列を生成
		DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowDrawRect, shadowDrawRect, 0.1f, 1000.0f);
		DirectX::XMStoreFloat4x4(&rc.view, V);
		DirectX::XMStoreFloat4x4(&rc.projection, P);

		DirectX::XMStoreFloat4x4(&lightViewProjection, V * P);
	}

	// 3Dモデル描画
	{

		ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
		shader->Begin(rc);

		//shader->Draw(rc, jummo.get());

		//ステージ描画
		StageManager::Instance().ModelRender(rc, shader);
		//プレイヤー描画
		player->ModelRender(rc, shader);
		//エネミー描画
		WallManager::Instance().ModelRender(rc, shader);
		boss->ModelRender(rc, shader);
		EnemyManager::Instance().ModelRender(rc, shader);
		OutputDebugStringA("エラーチェックです！！！！！！！\n");

		//セレクト看板
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
