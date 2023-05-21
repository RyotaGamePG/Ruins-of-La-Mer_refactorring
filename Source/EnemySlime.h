#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

//スライム
class EnemySlime :public Enemy
{
public:
    EnemySlime();
    ~EnemySlime()override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    //void Render(ID3D11DeviceContext* dc, Shader* shader)override;
    void ModelRender(RenderContext rc, ModelShader* modelShader);



    // 待機ステートへ遷移
    void TransitionIdleState();
    // 待機ステート更新処理
    void UpdateIdleState(float elapsedTime);

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
        Damage,
        Dead,
    };

private:
    // アニメーション
    enum Animation
    {
        Anim_Idle,
    };

protected:
    //死亡したときに呼ばれる
    void OnDead() override;

private:

    float   moveSpeed = 5.0f;
    float   turnSpeed = DirectX::XMConvertToRadians(720);

    Model* model = nullptr;


    State  state = State::Idle;

    float offsetX = 9.0f;
};