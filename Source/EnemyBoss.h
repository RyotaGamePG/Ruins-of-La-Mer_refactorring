#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

#include "Audio/Audio.h"
#include "Effect.h"

//スライム
class EnemyBoss :public Enemy
{
public:
    EnemyBoss(int stageNum);
    ~EnemyBoss()override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    //void Render(ID3D11DeviceContext* dc, Shader* shader)override;
    void ModelRender(RenderContext rc, ModelShader* modelShader);
    //デバック用GUI描画
    void DrawDebugGUI();

    //位置リセット
    void ResetPosition();

    //スピードアップ
    void SpeedUp() { TransitionSpeedUpState(); }

    //移動制限
    void offsetAreaWall(float offsetX);

    //TODO::つながりが強い
    void PlayerSet(Character* player) { this->player =player; }

private:
    //死亡したときに呼ばれる
    void OnDead() override;
    //死亡したかを返す
    bool IsDead() { return health < 0 ? true : false; }

    //地面との判定を取るか
    void SetGroundJudgeFlag(bool b) { groundJudge = b; }
    bool GetGroundJudgeFlag() { return groundJudge; }

    //ダメージをくらったときに呼ばれる
    void OnDamaged()override;


    //エネミーとボスの当たり判定
    void CollisionEnemyVsBoss();

    // ダウンステートへ遷移
    void TransitionDownState();
    // ダウンステート更新処理
    void UpdateDownState(float elapsedTime);

    // 移動ステートへ遷移
    void TransitionMoveState();
    // 移動ステート更新処理
    void UpdateMoveState(float elapsedTim);

    // 着地ステートへ遷移
    void TransitionLandState();
    // 着地ステート更新処理
    void UpdateLandState(float elapsedTime);

    // ダメージステートへ遷移
    void TransitionDamageState();
    // ダメージステート更新処理
    void UpdateDamageState(float elapsedTime);

    // 起き上がりステートへ遷移
    void TransitionGetUpState();
    // 起き上がりテート更新処理
    void UpdateGetUpState(float elapsedTime);

    // 死亡ステートへ遷移
    void TransitionDeadState();
    // 死亡ステート更新処理
    void UpdateDeadState(float elapsedTime);

    // リセットステートへ遷移
    void TransitionResetState();
    // リセットステート更新処理
    void UpdateResetState(float elapsedTime);

    // スピードアップステートへ遷移
    void TransitionSpeedUpState();
    // スピードアップステート更新処理
    void UpdateSpeedUpState(float elapsedTime);


    //リセットの際の地面すり抜けを消す
    void ResetOffsetUpdate();

private:
    // ステート
    enum class State
    {
        Down,
        Move,
        Jump,
        GetUp,
        Land,
        Damage,
        Dead,
        Reset,
        SpeedUp,
    };

private:
    // アニメーション
    enum Animation
    {
        Anim_Walk,
        Anim_Warp,
        Anim_Down,
        Anim_Stop,
        Anim_GetUp,
    };


private:
    Model* model = nullptr;

    State  state = State::Down;

    Character* player = nullptr;

    float  speedUpTimer = 0.0f;
    const float  speedUpTime = 3.0f;

    float stopTimer = 0.0f;
    const float stopTime = 5.0f*60.0f;

    float resetTimer = 0.0f;
    const float resetTime = 1.0f * 60.0f;

    float offsetX = 9.0f;

private:
    //音楽

    //足音
    std::unique_ptr<AudioSource> step = nullptr;

private://エフェクト
    Effect* hitEffect = nullptr;
    Effect* moveEffect = nullptr;

};
