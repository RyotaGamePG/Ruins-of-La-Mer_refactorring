#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include"Graphics/Graphics.h"
#include "WallManager.h"
#include "EnemyManager.h"
#include "EnemyBoss.h"
#include "Collision.h"



//�R���X�g���N�^
Player::Player()
{
    //model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");
    //model = new Model("Data/Model/Jummo/Jummo.mdl");
    //model = new Model("Data/Model/test/test.mdl");
    //model = new Model("Data/Model/testwalk.mdl");
    //model = new Model("Data/Model/test/tes.mdl");
    model = new Model("Data/Model/player.mdl");

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;



    //���ʉ�������
    jump      = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/Jump.wav", false);
    stepOn    = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/stepon.wav", false);
    death     = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/death.wav", false);
    hotateOn  = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/hotate.wav", false);

    jump->SetVolume(2.0f);

    //�G�t�F�N�g
    hitEffect = new Effect("Data/Effect/dash.efkefc");
    //hitEffect = new Effect("Data/Effect/bossDamage.efkefc");
    //�G�t�F�N�g
    damageEffect = new Effect("Data/Effect/allDamage.efkefc");
}

//�f�X�g���N�^
Player::~Player()
{
    delete model;
    
    //�G�t�F�N�g�j��
    delete hitEffect;
    delete damageEffect;
}

//�X�V����
void Player::Update(float elapsedTime)
{
    /*
    // B�{�^�������Ń����V���b�g�A�j���[�V�����Đ�
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_B)
    {
        model->PlayAnimation(Anim_Running, false);
    }
    // �����V���b�g�A�j���[�V�����Đ����I������烋�[�v�A�j���[�V�����Đ�
    if (!model->IsPlayAnimation())
    {
        model->PlayAnimation(1, true,0.1f);
    }
    
   //�ړ����͏���
    InputMove(elapsedTime);

    //�W�����v���͏���
    InputJump();

    //�e�ۓ��͏���
    InputProjectile();
    */

    // �X�e�[�g���̏���
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

    //���͏����X�V����
    UpdateVelocity(elapsedTime);

    //�ړ�����
    offsetAreaWall(offsetX);

    if (state != State::Dead &&
        state != State::Clear)
    {
        //�v���C���[�ƓG�Ƃ̏Փˏ���
        CollisionPlayerVsEnemies();

#if GAME
        //TODO::�Y�ꂸ��
        //�v���C���[�ƃ{�X�̏Փˏ���
        CollisionPlayerVsBoss();
#endif

        //�ǂƃv���C���[�̏Փˏ���
        CollisionPlayerVsWall();
    }

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���A�j���[�V�����X�V����
    model->UpdateAnimation(elapsedTime);

    //���f���s��X�V
    model->UpdateTransform(transform);
    //�`��ɕK�v�Ȃ̂Ń��f���ɃI�u�W�F�N�g�̍s���n����
    //���f���ɓn���ă��f���̍s����v�Z����
}

//�`�揈��
//void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    shader->Draw(dc, model);    //���̒��ŕ`�悷��ۂɁA��Ōv�Z���ꂽ���f���̍s������̒��Ŏg���Ă���
//
//}

void Player::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);

    //�e�ە`�揈��
    //projectileManager.ModelRender(rc, modelShader);
}

//�f�o�b�N�pGUI�`��
void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        //�g�����X�t�H�[��
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //�ʒu
            ImGui::InputFloat3("Position",&position.x);
            //��]
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("Angle", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            //�X�P�[��
            ImGui::InputFloat3("Scale", &scale.x);

            //����
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

    ////�Փ˔���p�̃f�o�b�O����`��
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    //�Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

}

void Player::offsetAreaWall(float offsetX)
{
    //�������ړ�����
    if (position.x > offsetX)position.x = offsetX;
    if (position.x < 0.0f)position.x = 0.0f;

    if (position.z < -0.5f)position.z = -0.5f;
}

//�X�e�B�b�N���͒l����ړ��x�N�g�����擾
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    //���͏����擾
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    //�J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //�ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

    //�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        //�P�ʃx�N�g����
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    //�J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        //�P�ʃx�N�g����
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    //�X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
    //�X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
    //�i�s�x�N�g�����v�Z����
    DirectX::XMFLOAT3 vec;
    vec.x = cameraFrontX * ay + cameraRightX * ax;
    vec.z = cameraFrontZ * ay + cameraRightZ * ax;
    //Y�������ɂ͈ړ����Ȃ�
    vec.y = 0.0f;

    return vec;
}


//�ړ����͏���
bool Player::InputMove(float elapsedTime)
{
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //�ړ�����
    Move(moveVec.x, moveVec.z, moveSpeed);

    //���񏈗�
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    // �i�s�x�N�g�����[���x�N�g������Ȃ��ꍇ�͓��͂��ꂽ
    if (moveVec.x ==0&& moveVec.z == 0)return false;
    else return true;
}



//�v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //���ׂĂ̓G�Ƒ�������ŏՓˏ���
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //�����蔻��Ȃ�
        if (!enemy->GetHitPositionCollision())continue;

        //�Փˏ���
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
            if (p.y > 0.7f&&velocity.y<0.0f) {  //TODO:����������������
  
                //�E�j���݂�
                if (enemy->getType() == Enemy::Type::Uni) {
                    Jump(8);
                    //�_���[�W��^����
                    enemy->ApplyDamage(1, 0.5f);

                    //������΂�
                    {
                        DirectX::XMFLOAT3 impulse;
                        float power = 40.0f;
                        //�����ł͂Ȃ�
                        int u = cosf(angle.y) > 0.0f ? 1 : -1;
                        impulse.x = 0.0f;
                        impulse.y = 0.0f;
                        impulse.z = u * power;
                        enemy->AddImpulse(impulse);

                        enemy->setLaunch();

                    }

                    //���ʉ�
                    stepOn = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/stepon.wav", false);
                    stepOn->Play();
                }

                //�z�^�e�W�����v
                if (enemy->getType() == Enemy::Type::Scallops)
                {
                    Jump(30);
                    //�Ռ���������
                    enemy->AddImpulse(DirectX::XMFLOAT3(1, 1, 1));

                    //���ʉ�
                    hotateOn = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/hotate.wav", false);
                    hotateOn->Play();

                    //�G�t�F�N�g
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
            //TODO::�Y�ꂸ��
            
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
            //�����o����̈ʒu�ݒ�
            enemy->SetPosition(outPosition);
        }
        */
    }
}

//�v���C���[�ƕǂƂ̏Փˏ���
void Player::CollisionPlayerVsWall()
{
    WallManager& wallManager = WallManager::Instance();

    //���ׂĂ̓G�Ƒ�������ŏՓˏ���
    int wallCount = wallManager.GetWallCount();
    for (int i = 0; i < wallCount; i++)
    {
        Wall* wall = wallManager.GetWall(i);

        //�����蔻��Ȃ�
        if (!wall->GetHitPositionCollision())continue;

        //�Փˏ���
        for (int j = 0; j < wall->GetHitPositionNum(); j++) {   //�����Ă��铖���蔻��̈ʒu�S���Ɣ���B

            DirectX::XMFLOAT3 outPosition;
            if (Collision::InterSectCylinderVsCylinder(     //�v���C���[�𓮂���
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
                if (position.y>=enemyHeight-1.0f&&velocity.y <= 0.0f)   //TODO:����������������
                {
                    //TODO::�ǂ����邩
                    //����
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
            //�����o����̈ʒu�ݒ�
            enemy->SetPosition(outPosition);
        }
        */
    }
}

//�v���C���[�ƃ{�X�̏Փˏ���
void Player::CollisionPlayerVsBoss()
{
        //�Փˏ���
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

//�W�����v���͏���
bool Player::InputJump()
{
    //�{�^�����͂ŃW�����v(�W�����v�񐔐����t��)
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        // �W�����v�񐔐���
        if (jumpCount < jumpLimit) 
        {
            // �W�����v
            jumpCount++;
            Jump(jumpSpeed);


            //���ʉ�
            jump = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/Jump.wav", false);
            jump->Play();
            //death = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/death.wav", false);
            //death->Play();

            //�W�����v���͂���
            return true;
        }
    }
    return false;
}

//���n�����Ƃ��ɌĂ΂��
void Player::OnLanding()
{
    jumpCount=0;

    // �����̑��͂����ȏ�Ȃ璅�n�X�e�[�g��
    if (velocity.y >= 0)
    {
        //�N���A�⎀�S����Ȃ��Ȃ�
        if (state != State::Clear && state != State::Dead) 
        {
            TransitionLandState();
        }
    }
}

// �ҋ@�X�e�[�g�֑J��
void Player::TransitionIdleState()
{
    state = State::Idle;
    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, true);
    //model->PlayAnimation(0, true);
}

// �ҋ@�X�e�[�g�X�V����
void Player::UpdateIdleState(float elapsedTime)
{
    //�ړ����͏���
    if (InputMove(elapsedTime))
    {
        // �ړ��X�e�[�g�ɐ؂�ւ�
        TransitionMoveState();
    }


    //�W�����v���͏���
    if (InputJump())
    {
        // �W�����v�X�e�[�g�ɐ؂�ւ�
        TransitionJumpState();
    }
}

// �ړ��X�e�[�g�֑J��
void Player::TransitionMoveState()
{
    state = State::Move;

    // ����A�j���[�V�����Đ�
    //model->PlayAnimation(0, true);
    model->PlayAnimation(Anim_Running, true);
}

// �ړ��X�e�[�g�X�V����
void Player::UpdateMoveState(float elapsedTime)
{
    // �ړ����͏���
    if (!InputMove(elapsedTime))
    {
        // �ҋ@�X�e�[�g�ɐ؂�ւ�
        TransitionIdleState();
    }
        //�_�b�V���G�t�F�N�g
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

    // �W�����v���͏���
    if (InputJump()||!isGround)
    {
        // �W�����v�X�e�[�g�ɐ؂�ւ�
        TransitionJumpState();
    }
}

// �W�����v�X�e�[�g�֑J��
void Player::TransitionJumpState()
{
    state = State::Jump;

    // �W�����v�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Jump, false);

    timer = timerSet;

}

// �W�����v�X�e�[�g�X�V����
void Player::UpdateJumpState(float elapsedTime)
{
    // �ړ����͏���
    InputMove(elapsedTime);

    // �W�����v���͏���
    if (InputJump())
    {
        // �W�����v�X�e�[�g�ɐ؂�ւ�
        TransitionJumpState();
    }

}

// ���n�X�e�[�g�֑J��
void Player::TransitionLandState()
{
    state = State::Land;

    // ���n�A�j���[�V�����Đ�
    //model->PlayAnimation(Anim_Landing, false);
    model->PlayAnimation(Anim_Idle, false);

}

// ���n�X�e�[�g�X�V����
void Player::UpdateLandState(float elapsedTime)
{
    //if (model->IsPlayAnimation())return;

    //�ړ����͏���
    if (InputMove(elapsedTime))
    {
        // �ړ��X�e�[�g�ɐ؂�ւ�
        TransitionMoveState();
    }
    else
    {
        // �ҋ@�X�e�[�g�ɐ؂�ւ�
        TransitionIdleState();
    }
    
}

// ���S�X�e�[�g�֑J��
void Player::TransitionDeadState()
{
    state = State::Dead;

    // ���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Death, false);

    //���ʉ�
    death->Play();
}

// ���S�X�e�[�g�X�V����
void Player::UpdateDeadState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;

    SetGameOver();
}

// �N���A�X�e�[�g�֑J��
void Player::TransitionClearState()
{
    state = State::Clear;

    // ���n�A�j���[�V�����Đ�
    //model->PlayAnimation(Anim_Death, false);
    model->PlayAnimation(1, true);
}

// �N���A�X�e�[�g�X�V����
void Player::UpdateClearState(float elapsedTime)
{
    //if (model->IsPlayAnimation())return;

    velocity.z = 6.0f;
    //���񏈗�
    Turn(elapsedTime, 0.0f, velocity.z, turnSpeed);
    if (position.z > 1000.0f)position.z = 1000.0f; //�N���A��ړ�����

    SetGameClear();
}