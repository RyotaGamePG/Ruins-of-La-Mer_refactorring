#include <imgui.h>
#include "EnemyCrab.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "Graphics/Graphics.h"


//コンストラクタ
EnemyCrab::EnemyCrab(bool type,bool move)
    :crabType(type)
{
    //model = new Model("Data/Model/Slime/Slime.mdl");
    model = new Model("Data/Model/Enemy/kani.mdl");

    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.02f;


    //幅、高さ設定
    radius = 1.0f;
    height = 1.5f;

    //体力の設定
    health = 3;

    //加速力の設定
    acceleration = 3.0f;

    this->type = Type::Crab;

    //お試し

    //初動の動き設定
    if (move)isPos1 = true;
    else isPos1 = false;

    //TransitionIdleState();

    if(type)angle.y = DirectX::XMConvertToRadians(90);
    else    angle.y = DirectX::XMConvertToRadians(180);

}

//デストラクタ
EnemyCrab::~EnemyCrab()
{
    delete model;
}

//更新処理
void EnemyCrab::Update(float elapsedTime)
{
    //死亡確認
    if (IsDead())return;

    //TODO::忘れずに
    //エディターもどき用
#if EDIT
    state = State::Idle;
#endif
    // ステート毎の処理
    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Right:
        UpdateRightMoveState(elapsedTime);
        break;
    case State::Left:
        UpdateLeftMoveState(elapsedTime);
        break;
    case State::Up:
        UpdateUpMoveState(elapsedTime);
        break;
    case State::Down:
        UpdateDownMoveState(elapsedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case State::Land:
        UpdateLandState(elapsedTime);
        break;
    case State::Dead:
        UpdateDeadState(elapsedTime);
        break;
    }


    //敵とボスの衝突処理
    //CollisionEnemyVsBoss();
    
    

    //速力更新
    UpdateVelocity(elapsedTime);

    //お試し移動制限
    offsetAreaWall(offsetX);
    

    //無敵時間更新
    UpdateInvincibleTimer(elapsedTime);

    //待機時間更新
    UpdateStopTimer(elapsedTime);

    //オブジェクト行列を更新
    UpdateTransform();

    //モデルアニメーション更新処理
    model->UpdateAnimation(elapsedTime);

    //モデル行列更新
    model->UpdateTransform(transform);
}

//描画処理
//void EnemyCrab::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    shader->Draw(dc, model);
//
//#if EDIT
//    if (ImGui::Begin("Crab", nullptr, ImGuiWindowFlags_None))
//    {
//        //トランスフォーム
//        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
//        {
//            //位置
//            if(crabType)
//            ImGui::InputFloat3("LR", &position.x);
//            else
//            ImGui::InputFloat3("UD", &position.x);
//
//            ImGui::Text("Num:%d",num);
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
//#endif
//}

//死亡したときに呼ばれる
void EnemyCrab::OnDead()
{
    TransitionDeadState();
}

//ダメージをくらったときに呼ばれる
void EnemyCrab::OnDamaged()
{
    TransitionDamageState();
}

//敵とボスの衝突処理
void EnemyCrab::CollisionEnemyVsBoss()
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
void EnemyCrab::TransitionIdleState()
{
    state = State::Idle;

    //待機アニメーション再生
    model->PlayAnimation(Anim_Idle, true);
}
// 待機ステート更新処理
void EnemyCrab::UpdateIdleState(float elapsedTime)
{
    if (stopTimer > 0.0f)return;

    isAreaWall = false;


    //次に設定されたもの
    if (crabType)
    {
        if (!isPos1)TransitionRightMoveState();
        if (isPos1)TransitionLeftMoveState();
       
    }
    else
    {
        if (!isPos1)TransitionUpMoveState();
        if (isPos1)TransitionDownMoveState();
    }
}

// 右移動ステートへ遷移
void EnemyCrab::TransitionRightMoveState()
{
    state = State::Right;

    // 走りアニメーション再生
    model->PlayAnimation(Anim_Running, true);
}
// 右移動ステート更新処理
void EnemyCrab::UpdateRightMoveState(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //moveVec.z *= 1.2f;
    moveVec.x = 0.0f;
    moveVec.y = 0.0f;
    moveVec.z +=1.0f;
    //移動処理
    Move(moveVec.x, moveVec.z, moveSpeed);

    if (position.z > pos0.z-radius)
    {
        position.z = pos0.z - radius;
        stopTimer = stopTime;
        isPos1 = true; //次は左
        TransitionIdleState();
    }
}

// 左移動ステートへ遷移
void EnemyCrab::TransitionLeftMoveState()
{
    state = State::Left;

    // 走りアニメーション再生
    model->PlayAnimation(Anim_Running, true);
}
// 左移動ステート更新処理
void EnemyCrab::UpdateLeftMoveState(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //moveVec.z *= 1.2f;
    moveVec.x = 0.0f;
    moveVec.y = 0.0f;
    moveVec.z -= 1.0f;
    //移動処理
    Move(moveVec.x, moveVec.z, moveSpeed);
    if (position.z < pos1.z+radius)
    {
        position.z = pos1.z + radius;
        stopTimer = stopTime;
        isPos1 = false;//次は右
        TransitionIdleState();
    }
}


// 上移動ステートへ遷移
void EnemyCrab::TransitionUpMoveState()
{
    state = State::Up;

    // 走りアニメーション再生
    model->PlayAnimation(Anim_Running, true);
}
// 上移動ステート更新処理
void EnemyCrab::UpdateUpMoveState(float elapsedTime)
{
    //回転させたいがために
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    moveVec.x += 1.0f;
    moveVec.y = 0.0f;
    moveVec.z = 0.0f;
    //移動処理
    Move(moveVec.x, moveVec.z, moveSpeed);
    if (isAreaWall||position.x > pos0.x - radius)
    {
        stopTimer = stopTime;
        isPos1 = true;//次は下
        TransitionIdleState();
    }
}


// 下移動ステートへ遷移
void EnemyCrab::TransitionDownMoveState()
{
    state = State::Down;

    // 走りアニメーション再生
    model->PlayAnimation(Anim_Running, true);
}
// 下移動ステート更新処理
void EnemyCrab::UpdateDownMoveState(float elapsedTime)
{
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    moveVec.x -= 1.0f;
    moveVec.y = 0.0f;
    moveVec.z = 0.0f;
    //移動処理
    Move(moveVec.x, moveVec.z, moveSpeed);
    if (isAreaWall || position.x < pos1.x + radius)
    {
        stopTimer = stopTime;
        isPos1 = false;//次は上
        TransitionIdleState();
    }
}


// 着地ステートへ遷移
void EnemyCrab::TransitionLandState()
{
    state = State::Land;

    // 着地アニメーション再生
    model->PlayAnimation(Anim_Idle, false);
}
// 着地ステート更新処理
void EnemyCrab::UpdateLandState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;

    //移動処理

    // 移動ステートに切り替え
    TransitionRightMoveState();

}

// ダメージステートへ遷移
void EnemyCrab::TransitionDamageState()
{
    state = State::Damage;

    // 着地アニメーション再生
    model->PlayAnimation(Anim_Idle, false);


}
// ダメージステート更新処理
void EnemyCrab::UpdateDamageState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;

    // 移動ステートに切り替え
    TransitionRightMoveState();
}


// 死亡ステートへ遷移
void EnemyCrab::TransitionDeadState()
{
    state = State::Dead;

    // 着地アニメーション再生
    model->PlayAnimation(Anim_Idle, false);
}
// 死亡ステート更新処理
void EnemyCrab::UpdateDeadState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;
}

//左右の移動先セット
void EnemyCrab::setTatgetMovePosition()
{
    if (crabType)
    {
        //右位置設定
        pos0 = { position.x,position.y,position.z + moveRange };
        //左位置設定
        pos1 = { position.x,position.y,position.z - moveRange };
    }
    else
    {
        //上位置設定
        pos0 = { position.x + moveRange ,position.y,position.z };
        //下位置設定
        pos1 = { position.x - moveRange ,position.y,position.z };
    }
    
}

void EnemyCrab::UpdateStopTimer(float elapsedTime)
{
    if (stopTimer > 0.0f)
    {
        stopTimer -= elapsedTime;
        if (stopTimer < 0)stopTimer = 0.0f;
    }
}

DirectX::XMFLOAT3 EnemyCrab::GetMoveVec() const
{
    DirectX::XMFLOAT3 moveVec = {};

    
    if (crabType)
    {
        if (!isPos1)//右
            moveVec = { position.x,position.y,pos0.z - position.z };
        if (isPos1)//左
            moveVec = { position.x,position.y,pos1.z - position.z };
        
    }
    else
    {
        if (!isPos1)//上
            moveVec = { pos0.x - position.x,position.y,position.z };
        if (isPos1)//下
            moveVec = { pos1.x - position.x,position.y,position.z };
    }

    return moveVec;
}

void EnemyCrab::ModelRender(RenderContext rc, ModelShader* modelShader)
{

    modelShader->Draw(rc, model);

}

void EnemyCrab::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
    debugRenderer->DrawCylinder(pos0, radius, height, DirectX::XMFLOAT4(0, 0, 0.7f, 1));
    debugRenderer->DrawCylinder(pos1, radius, height, DirectX::XMFLOAT4(0, 0.7f, 0, 1));
}