#pragma once

#include "Stage.h"
#include "Wall.h"
#include "Player.h"
#include "EnemyBoss.h"
#include "CameraController.h"
#include "Scene.h"

#include "Audio/Audio.h"

//試し
#include "Graphics/Sprite.h"

#include"Graphics/RenderTarget.h"
#include"Graphics/DepthStencil.h"
//#include"Graphics/LightManager.h"
#include"Graphics/Light.h"
#include "PostprocessingRenderer.h"

#include "Graphics/Model.h"
#include "Graphics/Sprite.h"
#include "Graphics/Texture.h"

// ゲームシーン
class SceneGame:public Scene
{
public:
	SceneGame() {}
	~SceneGame() override{}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render()override;

	//ゲーム通常処理
	void GameUpdate(float elapsedTime);
	//ゲームクリア処理
	void GameClearUpdate(float elapsedTime);
	//ゲームオーバー処理
	void GameOverUpdate(float elapsedTime);

	//リセット処理
	void Reset();


	//ステージ番号設定
	void SetStageNum(int stageNum) { this->stageNum = stageNum; }

	//ゲームクリア時の看板設置
	void SetClearSignBoard(float elapsedTime);
	//ゲームオーバー時の看板設置
	void SetOverSignBoard(float elapsedTime);

	//看板選択更新処理
	void selectSignbordUpdate();

	// 3D空間の描画
	void Render3DScene();

	// シャドウマップの描画
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
	
	//試し
	//Sprite* sprite = nullptr;
	float angle = 0.0f;

	//アップデート2023年4月2日
	std::unique_ptr<Texture> texture;

	Wall* wall = nullptr;

	//クリアとオーバー時に出てくる看板
	//タイトル文字
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
	//UVスクロールデータ
	UVScrollData uvScrollData;
	std::unique_ptr<Sprite>	sprite;
	//std::unique_ptr<Texture> texture;
	//マスク画像
	std::unique_ptr<Texture> maskTexture;
	float					 dissolveThreshold;
	float					 edgeThreshold; 	// 縁の閾値
	DirectX::XMFLOAT4		 edgeColor;			// 縁の色

		//	色調補正情報
	//ColorGradingData		colorGradingData;
	//	オフスクリーンレンダリング用描画ターゲット
	std::unique_ptr<RenderTarget> renderTarget;
	// ガウスフィルターデータ
	//GaussianFilterData			gaussianFilterData;
	//std::unique_ptr<Sprite>		gaussianBlurSprite;
	// ブルームデータ
	//LuminanceExtractionData		luminanceExtractionData;

	//	シャドウマップ用情報
	Light* mainDirectionalLight = nullptr;					//	シャドウマップを生成する平行光源
	std::unique_ptr<DepthStencil> shadowmapDepthStencil;	//	シャドウマップ用深度ステンシルバッファ
	float	shadowDrawRect = 66.157f;						//	シャドウマップに描画する範囲
	DirectX::XMFLOAT4X4	lightViewProjection;				//	ライトビュープロジェクション行列
	DirectX::XMFLOAT3	shadowColor = { 0.0f, 0.0f, 0.0f };	//	影の色
	float				shadowBias = 0.001f;				//	深度比較用のオフセット値

		//	ポストプロセス
	std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer;
	//	パノラマスカイボックス画像
	std::unique_ptr<Texture>	skyboxTexture;

	bool							drawDebugPrimitive;	//	デバッグプリミティブ描画
	bool							drawLightDebugPrimitive;	//ライト	デバッグ描画
	bool							drawPlayerDebugPrimitive;	//	プレイヤーデバッグ描画
	bool							drawEnemyPrimitive;	//	エネミーデバッグ描画
};
