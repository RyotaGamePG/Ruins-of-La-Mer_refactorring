#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "EnemyBoss.h"

#include "Audio/Audio.h"

#include "EffectManager.h"
#include "Effect.h"

//プレイヤー
class Player :public Character
{
protected:
    //着地したときに呼ばれる
    void OnLanding() override;

public:
    Player();
    ~Player()override;

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    //void Render(ID3D11DeviceContext* dc, Shader* shader);
    void ModelRender(RenderContext rc, ModelShader* modelShader);
    //デバッグ用GUI描画
    void DrawDebugGUI();

    //デバッグプリミティブ描画
    void DrawDebugprimitive();

    //移動制限
    void offsetAreaWall(float offsetX);


    //ゲームオーバーかを返す
    bool IsGameOver() { return gameOverFlag; }
    //ゲームクリアかを返す
    bool IsGameClear() { return gameClearFlag; }

public:

    //ボスを設定
    //TODO::つながりが強い
    void BossSet(EnemyBoss* boss) { this->boss = boss; }


private:

    //スティック入力値から移動ベクトルを取得
    DirectX::XMFLOAT3 GetMoveVec() const;

    //プレイヤーとエネミーとの衝突処理
    void CollisionPlayerVsEnemies();

    //プレイヤーとボスの衝突処理
    void CollisionPlayerVsBoss();

    //プレイヤーと壁の衝突処理
    void CollisionPlayerVsWall();

    //移動入力処理
    bool InputMove(float elapsedTime);

    //ジャンプ入力処理
    bool InputJump();



    //ゲームオーバーを設定
    void SetGameOver() { gameOverFlag = true; }

    //ゲームクリアを設定
    void SetGameClear() { gameClearFlag = true; }


    // 待機ステートへ遷移
    void TransitionIdleState();
    // 待機ステート更新処理
    void UpdateIdleState(float elapsedTime);

    // 移動ステートへ遷移
    void TransitionMoveState();
    // 移動ステート更新処理
    void UpdateMoveState(float elapsedTim);

    // ジャンプステートへ遷移
    void TransitionJumpState();
    // ジャンプステート更新処理
    void UpdateJumpState(float elapsedTime);

    // 着地ステートへ遷移
    void TransitionLandState();
    // 着地ステート更新処理
    void UpdateLandState(float elapsedTime);

    // 死亡ステートへ遷移
    void TransitionDeadState();
    // 死亡ステート更新処理
    void UpdateDeadState(float elapsedTime);

public:
    // クリアステートへ遷移
    void TransitionClearState();
private:
    // クリアステート更新処理
    void UpdateClearState(float elapsedTime);

public:

    // ステート
    enum class State
    {
        Idle,
        Move,
        Jump,
        Land,
        Dead,
        Clear,
    };
public:

    State GetState() { return state; }
    //TODO::ここ体力化フラグで管理してアニメーション終了時にするs
    bool IsDead() { return state == State::Dead ? true : false; }

private:
    // アニメーション
    enum Animation
    {
        Anim_Idle,
        Anim_Running,
        Anim_Jump,
        Anim_Landing,
        Anim_Death,
    };

private:

    Model* model = nullptr;

    float   moveSpeed = 6.0f;

    float   turnSpeed = DirectX::XMConvertToRadians(1080);

    float   jumpSpeed = 20.0f;

    int     jumpCount = 0;
    int     jumpLimit = 1;

    float offsetX = 9.0f;

    State   state = State::Idle;

    EnemyBoss* boss = nullptr;

    //ゲームオーバーフラグ
    bool gameOverFlag = false;
    //ゲームクリアフラグ
    bool gameClearFlag = false;

private:
    //音楽
    std::unique_ptr<AudioSource> jump = nullptr;
    std::unique_ptr<AudioSource> stepOn = nullptr;
    std::unique_ptr<AudioSource> death = nullptr;
    std::unique_ptr<AudioSource> hotateOn = nullptr;


public:
    //TODO::エディターもどき
    int enemyType = 0;


private:
    float timer = 0;
    float timerSet = 0.2f;
    Effect* hitEffect = nullptr;
    Effect* damageEffect = nullptr;
    
};
