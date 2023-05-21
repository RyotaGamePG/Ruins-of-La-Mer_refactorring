#include <imgui.h>
#include "EnemyScallops.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "Graphics/Graphics.h"

//コンストラクタ
EnemyScallops::EnemyScallops()
{
    //model = new Model("Data/Model/Slime/Slime.mdl");
    model = new Model("Data/Model/Enemy/hotate.mdl");

    //モデルが大きいのでスケーリング
    //scale.x = scale.y = scale.z = 0.02f;
    scale.x = scale.y = scale.z = 0.01f;


    //幅、高さ設定
    radius = 1.5f;
    height = 1.0f;

    //体力の設定
    health = 3;

    //加速力の設定
    acceleration = 3.0f;

    type = Type::Scallops;

    //お試し

    //TransitionIdleState();

}

//デストラクタ
EnemyScallops::~EnemyScallops()
{
    delete model;
}

//更新処理
void EnemyScallops::Update(float elapsedTime)
{
    //死亡確認
    if (IsDead())return;

    // ステート毎の処理
    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Open:
        UpdateOpenState(elapsedTime);
        break;
    }


    //敵とボスの衝突処理
    //CollisionEnemyVsBoss();



    //速力更新
    //UpdateVelocity(elapsedTime);

    //お試し移動制限
    offsetAreaWall(offsetX);


    //無敵時間更新
    UpdateInvincibleTimer(elapsedTime);

    //待機時間更新
    //UpdateStopTimer(elapsedTime);

    //オブジェクト行列を更新
    UpdateTransform();

    //モデルアニメーション更新処理
    model->UpdateAnimation(elapsedTime);

    //モデル行列更新
    model->UpdateTransform(transform);
}

//描画処理
//void EnemyScallops::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    shader->Draw(dc, model);
//
//
//#if EDIT
//    if (ImGui::Begin("Scallops", nullptr, ImGuiWindowFlags_None))
//    {
//        //トランスフォーム
//        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
//        {
//            //位置
//            ImGui::InputFloat3("Position", &position.x);
//
//            ImGui::Text("Num:%d", num);
//
//            if (editMove)
//                ImGui::Text("isMove");
//            /*
//            //回転
//            DirectX::XMFLOAT3 a;
//            a.x = DirectX::XMConvertToDegrees(angle.x);
//            a.y = DirectX::XMConvertToDegrees(angle.y);
//            a.z = DirectX::XMConvertToDegrees(angle.z);
//            ImGui::InputFloat3("Angle", &a.x);
//            angle.x = DirectX::XMConvertToRadians(a.x);
//            angle.y = DirectX::XMConvertToRadians(a.y);
//            angle.z = DirectX::XMConvertToRadians(a.z);
//            //スケール
//            ImGui::InputFloat3("Scale", &scale.x);
//
//            //体力
//            ImGui::InputInt("Helth", &health);
//
//            //速さ
//            ImGui::InputFloat("acceleration", &acceleration);
//
//            //待機時間
//            ImGui::InputFloat("stopTimer", &stopTimer);
//
//
//            int s = (int)state;
//            //状態
//            ImGui::InputInt("state", &s);
//            */
//
//        }
//    }
//    ImGui::End();
//
//#endif
//}

void EnemyScallops::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);
}

//死亡したときに呼ばれる
void EnemyScallops::OnDead()
{
}

//ダメージをくらったときに呼ばれる
void EnemyScallops::OnDamaged()
{
}

//敵とボスの衝突処理
void EnemyScallops::CollisionEnemyVsBoss()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //すべての敵と総当たりで衝突処理
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        if (!enemy->IsLaunch())continue;

        //衝突処理
        DirectX::XMFLOAT3 outPosition;
        if (Collision::InterSectCylinderVsCylinder(
            position,
            radius,
            height,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition))
        {
            ApplyDamage(1, 0.5f);
            enemy->SetPosition(outPosition);
            enemy->Destroy();
        }
    }

}


// 待機ステートへ遷移
void EnemyScallops::TransitionIdleState()
{
    state = State::Idle;

    //待機アニメーション再生
    model->PlayAnimation(Anim_Idle, true);
}
// 待機ステート更新処理
void EnemyScallops::UpdateIdleState(float elapsedTime)
{
    if (stopTimer > 0.0f)return;

    float move =velocity.x + velocity.y + velocity.z;
    //外から衝撃が来たなら
    if (move != 0.0f)
    {
        TransitionOpenState();
    }
}

// 死亡ステートへ遷移
void EnemyScallops::TransitionOpenState()
{
    state = State::Open;
    // 着地アニメーション再生
    model->PlayAnimation(Anim_Open, false);
}
// 死亡ステート更新処理
void EnemyScallops::UpdateOpenState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;
    velocity = {};      //フラグ代わりの衝撃を消す
    TransitionIdleState();
}


void EnemyScallops::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //衝突判定用のデバッグ円柱を描画
    if(state==State::Idle)
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 1, 1, 1));
    else
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 1, 1));

}