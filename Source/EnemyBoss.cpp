#include <imgui.h>
#include "EnemyBoss.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "Effect.h"

//コンストラクタ
EnemyBoss::EnemyBoss(int stageNum)
{
    //model = new Model("Data/Model/Jummo/Jummo.mdl");
    model = new Model("Data/Model/Enemy/zaragon.mdl");

    //モデルが大きいのでスケーリング
    //scale.x = scale.y = scale.z = 0.1f;

    scale.x = scale.y = scale.z = 0.012f;
    scale.x=0.02f;

    //scale.x = scale.y = scale.z = 0.002f;

    //音楽のロードと再生
    step = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/bossmove.wav", false);

    //幅、高さ設定
    radius = 8.0f;
    height = 5.0f;

    //体力の設定
    health = 3;

#if SPEEDUP
    //加速力の設定
    acceleration = 3.0f;
#else
    switch (stageNum)
    {
    case 0:
        acceleration = 3.0f;
        break;
    case 1:
        acceleration = 4.0f;
        break;
    case 2:
        acceleration = 4.0f;
        break;
    }
#endif

    
    //減速力の設定
    friction = 0.0f;

    this->type = Type::Boss;


    //お試し

    // 移動ステートに切り替え
    //TransitionIdleState();
    TransitionMoveState();

    //エフェクト
    hitEffect = new Effect("Data/Effect/bossDamage.efkefc");
    moveEffect = new Effect("Data/Effect/bossMove.efkfc");
}

//デストラクタ
EnemyBoss::~EnemyBoss()
{
    delete model;

    delete hitEffect;
    delete moveEffect;
}

//更新処理
void EnemyBoss::Update(float elapsedTime)
{
    //死亡確認
    if (IsDead())return;

    // ステート毎の処理
    switch (state)
    {
    case State::Down:
        UpdateDownState(elapsedTime);
        break;
    case State::Move:
        UpdateMoveState(elapsedTime);
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
    case State::Reset:
        UpdateResetState(elapsedTime);
        break;
    case State::SpeedUp:
        UpdateSpeedUpState(elapsedTime);
        break;
    case State::GetUp:
        UpdateGetUpState(elapsedTime);
        break;
    }

    //敵とボスの衝突処理
    CollisionEnemyVsBoss();
    //速力更新
    UpdateVelocity(elapsedTime);

    //お試し移動制限
    
    offsetAreaWall(offsetX);

    //リセットの際に生じる問題の調整
    ResetOffsetUpdate();

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
//void EnemyBoss::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    shader->Draw(dc, model);
//}

void EnemyBoss::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);
}

//デバック用GUI描画
void EnemyBoss::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Boss", nullptr, ImGuiWindowFlags_None))
    {
        //トランスフォーム
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //状態
            int s = (int)state;
            ImGui::InputInt("State", &s);

            //位置
            ImGui::InputFloat3("Position", &position.x);
            /*
            //回転
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("Angle", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            //スケール
            ImGui::InputFloat3("Scale", &scale.x);
            */
            //体力
            ImGui::InputInt("Helth", &health);

            //速さ
            ImGui::InputFloat("velocityZ", &velocity.z);

            //スピードアップ時間
            ImGui::InputFloat("speedUpTimer", &speedUpTimer);
        }
    }
    ImGui::End();
}

//移動制限
void EnemyBoss::offsetAreaWall(float offsetX)
{
    //お試し移動制限
    if (position.x > offsetX)position.x = offsetX;
    if (position.x < 0.0f)position.x = 0.0f;
}


//死亡したときに呼ばれる
void EnemyBoss::OnDead()
{
    //TransitionDeadState();
}

//ダメージをくらったときに呼ばれる
void EnemyBoss::OnDamaged()
{
    TransitionDamageState();
}

//敵とボスの衝突処理
void EnemyBoss::CollisionEnemyVsBoss()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //すべての敵と総当たりで衝突処理
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //敵が発射状態じゃない
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

            if (state == State::Move)
            {
                //ダメージを与える
                ApplyDamage(1, 0.5f);
            }

            //outPosition.x = 0.0f;
            enemy->SetPosition(outPosition);
            enemy->Destroy();
        }
    }

}


// ダメージステートへ遷移
void EnemyBoss::TransitionDamageState()
{
    state = State::Damage;

    // 着地アニメーション再生
    model->PlayAnimation(Anim_Down, false);

    //効果音
    step->Stop();

    //エフェクト
    DirectX::XMFLOAT3 enemy = GetPosition();
    enemy.y = enemy.y + height + 2.0f;
    enemy.x = enemy.x + 3.5f;
    enemy.z = enemy.z + 7.0f;
    hitEffect->Play(enemy, 2);
}
// ダメージステート更新処理
void EnemyBoss::UpdateDamageState(float elapsedTime)
{

    velocity = {};
    if (model->IsPlayAnimation())return;

    // ダウンステートに切り替え
    TransitionDownState();
}

// 待機ステートへ遷移
void EnemyBoss::TransitionDownState()
{
    state = State::Down;

    //待機アニメーション再生
    model->PlayAnimation(Anim_Stop, true);

    //停止時間設定
    stopTimer = stopTime;

    //効果音
    step->Stop();
}
// 待機ステート更新処理
void EnemyBoss::UpdateDownState(float elapsedTime)
{

    if (stopTimer >= 0)stopTimer--;
    if (stopTimer > 0.0f)return;

    stopTimer = 0.0f;

    TransitionGetUpState();
}

// 起き上がりステートへ遷移
void EnemyBoss::TransitionGetUpState()
{
    state = State::GetUp;

    //待機アニメーション再生
    model->PlayAnimation(Anim_GetUp, false);

    //効果音
    step->Stop();
}
// 起き上がりステート更新処理
void EnemyBoss::UpdateGetUpState(float elapsedTime)
{

    velocity = {};
    if (model->IsPlayAnimation())return;

    TransitionMoveState();
}

// 移動ステートへ遷移
void EnemyBoss::TransitionMoveState()
{
    state = State::Move;

    // 走りアニメーション再生
    model->PlayAnimation(Anim_Walk, true);

    //効果音
    step->Play();
}
// 移動ステート更新処理
void EnemyBoss::UpdateMoveState(float elapsedTime)
{
    velocity.z = acceleration;

    float resetLength = 30.0f;
    float length = position.z - player->GetPosition().z;

    //前でもリセット
    if (length > 0)length = -length;

    /*
    DirectX::XMFLOAT3 pos = GetPosition();
    moveEffect->Play(pos);
    */

    if (length < -resetLength)
    {
        // リセットステートに切り替え
        TransitionResetState();
    }
}


// 着地ステートへ遷移
void EnemyBoss::TransitionLandState()
{
    state = State::Land;

    // 着地アニメーション再生
    model->PlayAnimation(Anim_Walk, false);
}
// 着地ステート更新処理
void EnemyBoss::UpdateLandState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;

    //移動処理

    // 移動ステートに切り替え
    TransitionMoveState();
    
}

// 死亡ステートへ遷移
void EnemyBoss::TransitionDeadState()
{
    state = State::Dead;

    // 着地アニメーション再生
    model->PlayAnimation(Anim_Walk, false);

    //効果音
    step->Stop();
}
// 死亡ステート更新処理
void EnemyBoss::UpdateDeadState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;
}

// リセットステートへ遷移
void EnemyBoss::TransitionResetState()
{
    state = State::Reset;

    // 着地アニメーション再生
    model->PlayAnimation(Anim_Warp, false);

    //位置リセット
    ResetPosition();

    //地面にめり込むように
    SetGroundJudgeFlag(false);

    //リセット時間設定
    resetTimer = resetTime;

    //効果音
    //step->Stop();
    step->Play();

}
// リセットステート更新処理
void EnemyBoss::UpdateResetState(float elapsedTime)
{

    //地面から出たか、時間外ならなら
    if (position.y > 0.0f||resetTimer<=0.0f)
    {
        resetTimer = 0.0f;

        //地面にめり込まないように
        SetGroundJudgeFlag(true);
    }

    if (model->IsPlayAnimation())return;

    velocity = {};
    // 移動ステートに切り替え
    TransitionMoveState();

}

// スピードアップステートへ遷移
void EnemyBoss::TransitionSpeedUpState()
{
    state = State::SpeedUp;

    speedUpTimer = speedUpTime;

    // 着地アニメーション再生
    model->PlayAnimation(Anim_Walk, false);

    //効果音
    step->Play();
}
// スピードアップステート更新処理
void EnemyBoss::UpdateSpeedUpState(float elapsedTime)
{
    //TODO::忘れずに
#if GAME
    velocity.z = acceleration * 2.0f;
#endif

    if (speedUpTimer > 0.0f)speedUpTimer -= elapsedTime;

    if(speedUpTimer<=0.0f)
    {
        speedUpTimer = 0.0f;
        // リセットステートに切り替え
        TransitionMoveState();
    }
}

void EnemyBoss::ResetPosition()
{
    float offsetZ = 12.5f;
    const DirectX::XMFLOAT3 pos = { 4.5f,-1.0f, player->GetPosition().z - offsetZ };
    SetPosition(pos);
    //TODO::忘れずに
#if GAME
    const DirectX::XMFLOAT3 speed = {0.0f,30.0f,0.0f};
    velocity = speed;
#endif
}

void EnemyBoss::ResetOffsetUpdate()
{
    //リセット中にスピードアップに移行して、
    //地面判定フラグを取らずに行くことがあるため

    if (resetTimer <= 0) {
        if (!GetGroundJudgeFlag()) {
            const DirectX::XMFLOAT3 pos = { position.x,0.0f, position.z };
            SetPosition(pos);
            SetGroundJudgeFlag(true);
        }
    }
    else resetTimer--;
}