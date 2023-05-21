#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include"Graphics/Graphics.h"
#include "WallManager.h"
#include "EnemyManager.h"
#include "EnemyBoss.h"
#include "Collision.h"



//コンストラクタ
Player::Player()
{
    //model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");
    //model = new Model("Data/Model/Jummo/Jummo.mdl");
    //model = new Model("Data/Model/test/test.mdl");
    //model = new Model("Data/Model/testwalk.mdl");
    //model = new Model("Data/Model/test/tes.mdl");
    model = new Model("Data/Model/player.mdl");

    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.01f;



    //効果音初期化
    jump      = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/Jump.wav", false);
    stepOn    = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/stepon.wav", false);
    death     = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/death.wav", false);
    hotateOn  = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/hotate.wav", false);

    jump->SetVolume(2.0f);

    //エフェクト
    hitEffect = new Effect("Data/Effect/dash.efkefc");
    //hitEffect = new Effect("Data/Effect/bossDamage.efkefc");
    //エフェクト
    damageEffect = new Effect("Data/Effect/allDamage.efkefc");
}

//デストラクタ
Player::~Player()
{
    delete model;
    
    //エフェクト破棄
    delete hitEffect;
    delete damageEffect;
}

//更新処理
void Player::Update(float elapsedTime)
{
    /*
    // Bボタン押下でワンショットアニメーション再生
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_B)
    {
        model->PlayAnimation(Anim_Running, false);
    }
    // ワンショットアニメーション再生が終わったらループアニメーション再生
    if (!model->IsPlayAnimation())
    {
        model->PlayAnimation(1, true,0.1f);
    }
    
   //移動入力処理
    InputMove(elapsedTime);

    //ジャンプ入力処理
    InputJump();

    //弾丸入力処理
    InputProjectile();
    */

    // ステート毎の処理
    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Move:
        UpdateMoveState(elapsedTime);
        break;
    case State::Jump:
        UpdateJumpState(elapsedTime);
        break;
    case State::Land:
        UpdateLandState(elapsedTime);
        break;
    case State::Dead:
        UpdateDeadState(elapsedTime);
        break;
    case State::Clear:
        UpdateClearState(elapsedTime);
        break;
    }

    //速力処理更新処理
    UpdateVelocity(elapsedTime);

    //移動制限
    offsetAreaWall(offsetX);

    if (state != State::Dead &&
        state != State::Clear)
    {
        //プレイヤーと敵との衝突処理
        CollisionPlayerVsEnemies();

#if GAME
        //TODO::忘れずに
        //プレイヤーとボスの衝突処理
        CollisionPlayerVsBoss();
#endif

        //壁とプレイヤーの衝突処理
        CollisionPlayerVsWall();
    }

    //オブジェクト行列を更新
    UpdateTransform();

    //モデルアニメーション更新処理
    model->UpdateAnimation(elapsedTime);

    //モデル行列更新
    model->UpdateTransform(transform);
    //描画に必要なのでモデルにオブジェクトの行列を渡して
    //モデルに渡してモデルの行列を計算する
}

//描画処理
//void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    shader->Draw(dc, model);    //この中で描画する際に、上で計算されたモデルの行列をこの中で使っている
//
//}

void Player::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);

    //弾丸描画処理
    //projectileManager.ModelRender(rc, modelShader);
}

//デバック用GUI描画
void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        //トランスフォーム
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //位置
            ImGui::InputFloat3("Position",&position.x);
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

            //速力
            ImGui::InputFloat3("Velocity",&velocity.x);

            float m = cosf(angle.y);
            ImGui::InputFloat("angleY", &m);
            float u = u = cosf(angle.y) > 0.0f ? 1 : -1;
            ImGui::InputFloat("front", &u);

            int s = (int)state;
            ImGui::InputInt("State", &s);

            ImGui::InputFloat("moveSpeed", &moveSpeed);

            ImGui::InputInt("EnemyType", &enemyType);
            ImGui::Text("0:Uni 1:CrabLR 2:CrabUD ");
            ImGui::Text("3:hotate 4:EnemyWall ");
            ImGui::Text("5:SpeedUp 6:Goal ");

        }
    }
    ImGui::End();
}

void Player::DrawDebugprimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    ////衝突判定用のデバッグ球を描画
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    //衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

}

void Player::offsetAreaWall(float offsetX)
{
    //お試し移動制限
    if (position.x > offsetX)position.x = offsetX;
    if (position.x < 0.0f)position.x = 0.0f;

    if (position.z < -0.5f)position.z = -0.5f;
}

//スティック入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    //入力情報を取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    //カメラ方向とスティックの入力値によって進行方向を計算する
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //移動ベクトルはXZ平面に水平なベクトルになるようにする

    //カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        //単位ベクトル化
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    //カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        //単位ベクトル化
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    //スティックの水平入力値をカメラ右方向に反映し、
    //スティックの垂直入力値をカメラ前方向に反映し、
    //進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = cameraFrontX * ay + cameraRightX * ax;
    vec.z = cameraFrontZ * ay + cameraRightZ * ax;
    //Y軸方向には移動しない
    vec.y = 0.0f;

    return vec;
}


//移動入力処理
bool Player::InputMove(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //移動処理
    Move(moveVec.x, moveVec.z, moveSpeed);

    //旋回処理
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    // 進行ベクトルがゼロベクトルじゃない場合は入力された
    if (moveVec.x ==0&& moveVec.z == 0)return false;
    else return true;
}



//プレイヤーとエネミーとの衝突処理
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //すべての敵と総当たりで衝突処理
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //当たり判定なし
        if (!enemy->GetHitPositionCollision())continue;

        //衝突処理
        DirectX::XMFLOAT3 outPosition;
#if GAME
        if (Collision::InterSectCylinderVsCylinder(
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            position,
            radius,
            height,
            outPosition))
#endif
#if EDIT
        if (Collision::InterSectCylinderVsCylinder(
            position,
            radius,
            height,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition))
#endif
        {
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR N = DirectX::XMVectorSubtract(P,E);
            N= DirectX::XMVector3Normalize(N);
            DirectX::XMFLOAT3 p;
            DirectX::XMStoreFloat3(&p,N);
            float enemyHeight = enemy->GetPosition().y + enemy->GetHeight();
            if (p.y > 0.7f&&velocity.y<0.0f) {  //TODO:ここおかしいかも
  
                //ウニ踏みつけ
                if (enemy->getType() == Enemy::Type::Uni) {
                    Jump(8);
                    //ダメージを与える
                    enemy->ApplyDamage(1, 0.5f);

                    //吹き飛ばす
                    {
                        DirectX::XMFLOAT3 impulse;
                        float power = 40.0f;
                        //向きではない
                        int u = cosf(angle.y) > 0.0f ? 1 : -1;
                        impulse.x = 0.0f;
                        impulse.y = 0.0f;
                        impulse.z = u * power;
                        enemy->AddImpulse(impulse);

                        enemy->setLaunch();

                    }

                    //効果音
                    stepOn = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/stepon.wav", false);
                    stepOn->Play();
                }

                //ホタテジャンプ
                if (enemy->getType() == Enemy::Type::Scallops)
                {
                    Jump(30);
                    //衝撃を加える
                    enemy->AddImpulse(DirectX::XMFLOAT3(1, 1, 1));

                    //効果音
                    hotateOn = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/hotate.wav", false);
                    hotateOn->Play();

                    //エフェクト
                    DirectX::XMFLOAT3 enemy = GetPosition();
                    hitEffect->Play(enemy);
                }
#if EDIT
                if (enemy->getType() != Enemy::Type::Scallops)
                {
                    enemy->Destroy();
                }
#endif

            }
#if GAME
            else SetPosition(outPosition);
            //TODO::忘れずに
            
            if (enemy->getType() == Enemy::Type::Crab)
            {
                TransitionDeadState();
                DirectX::XMFLOAT3 player = GetPosition();
                player.y;
                damageEffect->Play(player, 0.5);
            }

#endif
#if EDIT
            else
            {
                enemy->SetPosition(outPosition);
                enemy->SetEditMove();
            }
#endif
        }
        /*
        if (Collision::InterSectSphereVsSphere(
            position,
            radius,
            enemy->GetPosition(),
            enemy->GetRadius(),
            outPosition))
        {
            //押し出し後の位置設定
            enemy->SetPosition(outPosition);
        }
        */
    }
}

//プレイヤーと壁との衝突処理
void Player::CollisionPlayerVsWall()
{
    WallManager& wallManager = WallManager::Instance();

    //すべての敵と総当たりで衝突処理
    int wallCount = wallManager.GetWallCount();
    for (int i = 0; i < wallCount; i++)
    {
        Wall* wall = wallManager.GetWall(i);

        //当たり判定なし
        if (!wall->GetHitPositionCollision())continue;

        //衝突処理
        for (int j = 0; j < wall->GetHitPositionNum(); j++) {   //持っている当たり判定の位置全部と判定。

            DirectX::XMFLOAT3 outPosition;
            if (Collision::InterSectCylinderVsCylinder(     //プレイヤーを動かす
                wall->GetHitPosition(j),
                wall->GetHitPositionRadius(),
                wall->GetHitPositionHeight(),
                position,
                radius,
                height,
                outPosition))
            {
                DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
                DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&wall->GetPosition());
                DirectX::XMVECTOR N = DirectX::XMVectorSubtract(P, E);
                N = DirectX::XMVector3Normalize(N);
                DirectX::XMFLOAT3 p;
                DirectX::XMStoreFloat3(&p, N);
                float enemyHeight = wall->GetPosition().y + wall->GetHitHeight();
                if (position.y>=enemyHeight-1.0f&&velocity.y <= 0.0f)   //TODO:ここおかしいかも
                {
                    //TODO::どうするか
                    //乗れる
                    position.y = enemyHeight;
                    velocity.y = 0.0f;
                    isGround = true;
                    if (state != State::Move && state != State::Idle)
                    {
                        TransitionMoveState();
                    }

                }
                else
                {
                    SetPosition(outPosition);
                    isGround = false;
                }

            }
        }
        /*
        if (Collision::InterSectSphereVsSphere(
            position,
            radius,
            enemy->GetPosition(),
            enemy->GetRadius(),
            outPosition))
        {
            //押し出し後の位置設定
            enemy->SetPosition(outPosition);
        }
        */
    }
}

//プレイヤーとボスの衝突処理
void Player::CollisionPlayerVsBoss()
{
        //衝突処理
        DirectX::XMFLOAT3 outPosition;
        if (Collision::InterSectCylinderVsCylinder(
            boss->GetPosition(),
            boss->GetRadius(),
            boss->GetHeight(),
            position,
            radius,
            height,
            outPosition))
        {
            if (state != State::Dead) {
                TransitionDeadState();
                SetPosition(outPosition);
            }
            DirectX::XMFLOAT3 player = GetPosition();
            damageEffect->Play(player);

        }
}

//ジャンプ入力処理
bool Player::InputJump()
{
    //ボタン入力でジャンプ(ジャンプ回数制限付き)
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        // ジャンプ回数制限
        if (jumpCount < jumpLimit) 
        {
            // ジャンプ
            jumpCount++;
            Jump(jumpSpeed);


            //効果音
            jump = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/Jump.wav", false);
            jump->Play();
            //death = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/death.wav", false);
            //death->Play();

            //ジャンプ入力した
            return true;
        }
    }
    return false;
}

//着地したときに呼ばれる
void Player::OnLanding()
{
    jumpCount=0;

    // 板方向の速力が一定以上なら着地ステートへ
    if (velocity.y >= 0)
    {
        //クリアや死亡じゃないなら
        if (state != State::Clear && state != State::Dead) 
        {
            TransitionLandState();
        }
    }
}

// 待機ステートへ遷移
void Player::TransitionIdleState()
{
    state = State::Idle;
    //待機アニメーション再生
    model->PlayAnimation(Anim_Idle, true);
    //model->PlayAnimation(0, true);
}

// 待機ステート更新処理
void Player::UpdateIdleState(float elapsedTime)
{
    //移動入力処理
    if (InputMove(elapsedTime))
    {
        // 移動ステートに切り替え
        TransitionMoveState();
    }


    //ジャンプ入力処理
    if (InputJump())
    {
        // ジャンプステートに切り替え
        TransitionJumpState();
    }
}

// 移動ステートへ遷移
void Player::TransitionMoveState()
{
    state = State::Move;

    // 走りアニメーション再生
    //model->PlayAnimation(0, true);
    model->PlayAnimation(Anim_Running, true);
}

// 移動ステート更新処理
void Player::UpdateMoveState(float elapsedTime)
{
    // 移動入力処理
    if (!InputMove(elapsedTime))
    {
        // 待機ステートに切り替え
        TransitionIdleState();
    }
        //ダッシュエフェクト
    else {
        if (timer >= 0.0f) {
            timer -= elapsedTime;
        }
        if (timer <= 0.0f) {
            DirectX::XMFLOAT3 player = GetPosition();
            float frontX = sinf(angle.y);
            float frontZ = cosf(angle.y);
            player.x -= frontX;
            player.z -= frontZ;
            hitEffect->Play(player);
            timer = timerSet;
        }
    }

    // ジャンプ入力処理
    if (InputJump()||!isGround)
    {
        // ジャンプステートに切り替え
        TransitionJumpState();
    }
}

// ジャンプステートへ遷移
void Player::TransitionJumpState()
{
    state = State::Jump;

    // ジャンプアニメーション再生
    model->PlayAnimation(Anim_Jump, false);

    timer = timerSet;

}

// ジャンプステート更新処理
void Player::UpdateJumpState(float elapsedTime)
{
    // 移動入力処理
    InputMove(elapsedTime);

    // ジャンプ入力処理
    if (InputJump())
    {
        // ジャンプステートに切り替え
        TransitionJumpState();
    }

}

// 着地ステートへ遷移
void Player::TransitionLandState()
{
    state = State::Land;

    // 着地アニメーション再生
    //model->PlayAnimation(Anim_Landing, false);
    model->PlayAnimation(Anim_Idle, false);

}

// 着地ステート更新処理
void Player::UpdateLandState(float elapsedTime)
{
    //if (model->IsPlayAnimation())return;

    //移動入力処理
    if (InputMove(elapsedTime))
    {
        // 移動ステートに切り替え
        TransitionMoveState();
    }
    else
    {
        // 待機ステートに切り替え
        TransitionIdleState();
    }
    
}

// 死亡ステートへ遷移
void Player::TransitionDeadState()
{
    state = State::Dead;

    // 着地アニメーション再生
    model->PlayAnimation(Anim_Death, false);

    //効果音
    death->Play();
}

// 死亡ステート更新処理
void Player::UpdateDeadState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;

    SetGameOver();
}

// クリアステートへ遷移
void Player::TransitionClearState()
{
    state = State::Clear;

    // 着地アニメーション再生
    //model->PlayAnimation(Anim_Death, false);
    model->PlayAnimation(1, true);
}

// クリアステート更新処理
void Player::UpdateClearState(float elapsedTime)
{
    //if (model->IsPlayAnimation())return;

    velocity.z = 6.0f;
    //旋回処理
    Turn(elapsedTime, 0.0f, velocity.z, turnSpeed);
    if (position.z > 1000.0f)position.z = 1000.0f; //クリア後移動制限

    SetGameClear();
}