#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

//左右か上下に移動するカニ
class EnemyScallops :public Enemy
{
public:
    EnemyScallops();
    ~EnemyScallops()override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    //void Render(ID3D11DeviceContext* dc, Shader* shader)override;
    void ModelRender(RenderContext rc, ModelShader* modelShader);

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

private:

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

    // 死亡ステートへ遷移
    void TransitionOpenState();
    // 死亡ステート更新処理
    void UpdateOpenState(float elapsedTime);

private:
    // ステート
    enum class State
    {
        Idle,
        Open
    };

private:
    // アニメーション
    enum Animation
    {
        Anim_Idle,
        Anim_Open,
    };

private:

    float   moveSpeed = 5.0f;
    float   turnSpeed = DirectX::XMConvertToRadians(720);


    Model* model = nullptr;

    State  state = State::Idle;

    Character* player = nullptr;

    float offsetX = 9.0f;

};

