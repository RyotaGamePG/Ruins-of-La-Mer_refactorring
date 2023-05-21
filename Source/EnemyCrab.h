#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

//左右か上下に移動するカニ
class EnemyCrab :public Enemy
{
public:
    EnemyCrab(bool type,bool move);
    ~EnemyCrab()override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    //void Render(ID3D11DeviceContext* dc, Shader* shader)override;
    void ModelRender(RenderContext rc, ModelShader* modelShader);

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

    //待機時間更新処理
    void UpdateStopTimer(float elapsedTime);

    //左右の移動先セット
    void setTatgetMovePosition();

    DirectX::XMFLOAT3 GetMoveVec() const;
    

    //TODO::つながりが強い
    void PlayerSet(Character* player) { this->player = player; }

private:

    //地面のめりこみ許可をセット
    void SetGroundJudgeFlag(bool b) { groundJudge = b; }

    //死亡したときに呼ばれる
    void OnDead() override;
    //ダメージをくらったときに呼ばれる
    void OnDamaged()override;

    //エネミーとボスの当たり判定
    void CollisionEnemyVsBoss();


    // 待機ステートへ遷移
    void TransitionIdleState();
    // 待機ステート更新処理
    void UpdateIdleState(float elapsedTime);

    // 右移動ステートへ遷移
    void TransitionRightMoveState();
    // 右移動ステート更新処理
    void UpdateRightMoveState(float elapsedTim);

    // 左移動ステートへ遷移
    void TransitionLeftMoveState();
    // 左移動ステート更新処理
    void UpdateLeftMoveState(float elapsedTim);

    // 上移動ステートへ遷移
    void TransitionUpMoveState();
    // 上移動ステート更新処理
    void UpdateUpMoveState(float elapsedTim);

    // 下移動ステートへ遷移
    void TransitionDownMoveState();
    // 下移動ステート更新処理
    void UpdateDownMoveState(float elapsedTim);

    // 着地ステートへ遷移
    void TransitionLandState();
    // 着地ステート更新処理
    void UpdateLandState(float elapsedTime);

    // ダメージステートへ遷移
    void TransitionDamageState();
    // ダメージステート更新処理
    void UpdateDamageState(float elapsedTime);

    // 死亡ステートへ遷移
    void TransitionDeadState();
    // 死亡ステート更新処理
    void UpdateDeadState(float elapsedTime);

private:
    // ステート
    enum class State
    {
        Idle,
        Move,
        Jump,
        Land,
        Damage,
        Dead,
        Reset,
        Right,
        Left,
        Up,
        Down
    };

private:
    // アニメーション
    enum Animation
    {
        Anim_Running,
        Anim_Idle,
    };


private:

    //移動タイプ---
    bool  crabType = true;      //カニの種類。true:横移動  false:縦移動

    //左右移動に---
    DirectX::XMFLOAT3  pos0 = {};   //右、上
    DirectX::XMFLOAT3  pos1 = {};   //左、下
    const float moveRange = 5.0f;   //生成場所から移動先への半径

    //左右上下に---
    bool  isPos1 = true;        //左、下にいるか

    //待機時間
    float stopTimer = 0.0f;     //待ち時間
    const float stopTime = 2.0f;//待ち時間定数


    float   moveSpeed = 5.0f;
    float   turnSpeed = DirectX::XMConvertToRadians(720);

    float offsetX = 9.0f;


    Model* model = nullptr;

    State  state = State::Idle;

    Character* player = nullptr;

};
