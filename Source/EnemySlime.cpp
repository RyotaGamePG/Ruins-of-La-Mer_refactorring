#include <imgui.h>
#include "EnemySlime.h"
#include "WallManager.h"
#include "Collision.h"


//コンストラクタ
EnemySlime::EnemySlime()
{
    //model = new Model("Data/model/Slime/Slime.mdl");
    model = new Model("Data/Model/Enemy/uni.mdl");

    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.01f;


    //幅、高さ設定
    radius = 0.8f;
    height = 1.0f;

    this->type = Type::Uni;

    //お試し
    TransitionIdleState();
    angle.y = DirectX::XMConvertToRadians(90);


    //エフェクト
    hitEffect = new Effect("Data/Effect/allDamage.efkefc");
}

//デストラクタ
EnemySlime::~EnemySlime()
{
    delete model;

    delete hitEffect;
}

//更新処理
void EnemySlime::Update(float elapsedTime)
{
    //死亡確認
    if (IsDead())return;

    // ステート毎の処理
    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case State::Dead:
        UpdateDeadState(elapsedTime);
        break;
    }

    //速力更新
    UpdateVelocity(elapsedTime);

    //お試し移動制限
    offsetAreaWall(offsetX);

    //無敵時間更新
    UpdateInvincibleTimer(elapsedTime);

    //吹き飛ばし処理
    LaunchedUpdate(elapsedTime);

    //オブジェクト行列を更新
    UpdateTransform();

    //モデルアニメーション更新処理
    model->UpdateAnimation(elapsedTime);

    //モデル行列更新
    model->UpdateTransform(transform);
}

void EnemySlime::ModelRender(RenderContext rc, ModelShader* modelShader)
{

    modelShader->Draw(rc, model);
}

//描画処理
//void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    shader->Draw(dc, model);
//
//#if EDIT
//    if (ImGui::Begin("enemy", nullptr, ImGuiWindowFlags_None))
//    {
//        //トランスフォーム
//        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
//        {
//            ImGui::DragFloat3("Position", &position.x,0.5f);
//
//            //位置
//            ImGui::Text("Num:%d", num);
//
//            if(editMove)
//            ImGui::Text("isMove");
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
//            int l = launch;
//            //吹き飛ばされているか
//            ImGui::InputInt("Launch", &l);
//            */
//
//        }
//    }
//    ImGui::End();
//#endif
//}

//死亡したときに呼ばれる
void EnemySlime::OnDead()
{
    Destroy();
}

// 待機ステートへ遷移
void EnemySlime::TransitionIdleState()
{
    state = State::Idle;

    // 着地アニメーション再生
    model->PlayAnimation(Anim_Idle, true);


}
// 待機ステート更新処理
void EnemySlime::UpdateIdleState(float elapsedTime)
{
    

}

// ダメージステートへ遷移
void EnemySlime::TransitionDamageState()
{
    state = State::Damage;

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_Idle, false);


}
// ダメージステート更新処理
void EnemySlime::UpdateDamageState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;

}

// 死亡ステートへ遷移
void EnemySlime::TransitionDeadState()
{
    state = State::Dead;

    //死亡アニメーション再生
    model->PlayAnimation(Anim_Idle, false);

}
// 死亡ステート更新処理
void EnemySlime::UpdateDeadState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;

}