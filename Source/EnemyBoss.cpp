#include <imgui.h>
#include "EnemyBoss.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "Effect.h"

//�R���X�g���N�^
EnemyBoss::EnemyBoss(int stageNum)
{
    //model = new Model("Data/Model/Jummo/Jummo.mdl");
    model = new Model("Data/Model/Enemy/zaragon.mdl");

    //���f�����傫���̂ŃX�P�[�����O
    //scale.x = scale.y = scale.z = 0.1f;

    scale.x = scale.y = scale.z = 0.012f;
    scale.x=0.02f;

    //scale.x = scale.y = scale.z = 0.002f;

    //���y�̃��[�h�ƍĐ�
    step = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/bossmove.wav", false);

    //���A�����ݒ�
    radius = 8.0f;
    height = 5.0f;

    //�̗͂̐ݒ�
    health = 3;

#if SPEEDUP
    //�����͂̐ݒ�
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

    
    //�����͂̐ݒ�
    friction = 0.0f;

    this->type = Type::Boss;


    //������

    // �ړ��X�e�[�g�ɐ؂�ւ�
    //TransitionIdleState();
    TransitionMoveState();

    //�G�t�F�N�g
    hitEffect = new Effect("Data/Effect/bossDamage.efkefc");
    moveEffect = new Effect("Data/Effect/bossMove.efkfc");
}

//�f�X�g���N�^
EnemyBoss::~EnemyBoss()
{
    delete model;

    delete hitEffect;
    delete moveEffect;
}

//�X�V����
void EnemyBoss::Update(float elapsedTime)
{
    //���S�m�F
    if (IsDead())return;

    // �X�e�[�g���̏���
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

    //�G�ƃ{�X�̏Փˏ���
    CollisionEnemyVsBoss();
    //���͍X�V
    UpdateVelocity(elapsedTime);

    //�������ړ�����
    
    offsetAreaWall(offsetX);

    //���Z�b�g�̍ۂɐ�������̒���
    ResetOffsetUpdate();

    //���G���ԍX�V
    UpdateInvincibleTimer(elapsedTime);
    //�ҋ@���ԍX�V
    //UpdateStopTimer(elapsedTime);

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���A�j���[�V�����X�V����
    model->UpdateAnimation(elapsedTime);

    //���f���s��X�V
    model->UpdateTransform(transform);
}

//�`�揈��
//void EnemyBoss::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    shader->Draw(dc, model);
//}

void EnemyBoss::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);
}

//�f�o�b�N�pGUI�`��
void EnemyBoss::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Boss", nullptr, ImGuiWindowFlags_None))
    {
        //�g�����X�t�H�[��
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //���
            int s = (int)state;
            ImGui::InputInt("State", &s);

            //�ʒu
            ImGui::InputFloat3("Position", &position.x);
            /*
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
            */
            //�̗�
            ImGui::InputInt("Helth", &health);

            //����
            ImGui::InputFloat("velocityZ", &velocity.z);

            //�X�s�[�h�A�b�v����
            ImGui::InputFloat("speedUpTimer", &speedUpTimer);
        }
    }
    ImGui::End();
}

//�ړ�����
void EnemyBoss::offsetAreaWall(float offsetX)
{
    //�������ړ�����
    if (position.x > offsetX)position.x = offsetX;
    if (position.x < 0.0f)position.x = 0.0f;
}


//���S�����Ƃ��ɌĂ΂��
void EnemyBoss::OnDead()
{
    //TransitionDeadState();
}

//�_���[�W����������Ƃ��ɌĂ΂��
void EnemyBoss::OnDamaged()
{
    TransitionDamageState();
}

//�G�ƃ{�X�̏Փˏ���
void EnemyBoss::CollisionEnemyVsBoss()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //���ׂĂ̓G�Ƒ�������ŏՓˏ���
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //�G�����ˏ�Ԃ���Ȃ�
        if (!enemy->IsLaunch())continue;

        //�Փˏ���
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
                //�_���[�W��^����
                ApplyDamage(1, 0.5f);
            }

            //outPosition.x = 0.0f;
            enemy->SetPosition(outPosition);
            enemy->Destroy();
        }
    }

}


// �_���[�W�X�e�[�g�֑J��
void EnemyBoss::TransitionDamageState()
{
    state = State::Damage;

    // ���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Down, false);

    //���ʉ�
    step->Stop();

    //�G�t�F�N�g
    DirectX::XMFLOAT3 enemy = GetPosition();
    enemy.y = enemy.y + height + 2.0f;
    enemy.x = enemy.x + 3.5f;
    enemy.z = enemy.z + 7.0f;
    hitEffect->Play(enemy, 2);
}
// �_���[�W�X�e�[�g�X�V����
void EnemyBoss::UpdateDamageState(float elapsedTime)
{

    velocity = {};
    if (model->IsPlayAnimation())return;

    // �_�E���X�e�[�g�ɐ؂�ւ�
    TransitionDownState();
}

// �ҋ@�X�e�[�g�֑J��
void EnemyBoss::TransitionDownState()
{
    state = State::Down;

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Stop, true);

    //��~���Ԑݒ�
    stopTimer = stopTime;

    //���ʉ�
    step->Stop();
}
// �ҋ@�X�e�[�g�X�V����
void EnemyBoss::UpdateDownState(float elapsedTime)
{

    if (stopTimer >= 0)stopTimer--;
    if (stopTimer > 0.0f)return;

    stopTimer = 0.0f;

    TransitionGetUpState();
}

// �N���オ��X�e�[�g�֑J��
void EnemyBoss::TransitionGetUpState()
{
    state = State::GetUp;

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_GetUp, false);

    //���ʉ�
    step->Stop();
}
// �N���オ��X�e�[�g�X�V����
void EnemyBoss::UpdateGetUpState(float elapsedTime)
{

    velocity = {};
    if (model->IsPlayAnimation())return;

    TransitionMoveState();
}

// �ړ��X�e�[�g�֑J��
void EnemyBoss::TransitionMoveState()
{
    state = State::Move;

    // ����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Walk, true);

    //���ʉ�
    step->Play();
}
// �ړ��X�e�[�g�X�V����
void EnemyBoss::UpdateMoveState(float elapsedTime)
{
    velocity.z = acceleration;

    float resetLength = 30.0f;
    float length = position.z - player->GetPosition().z;

    //�O�ł����Z�b�g
    if (length > 0)length = -length;

    /*
    DirectX::XMFLOAT3 pos = GetPosition();
    moveEffect->Play(pos);
    */

    if (length < -resetLength)
    {
        // ���Z�b�g�X�e�[�g�ɐ؂�ւ�
        TransitionResetState();
    }
}


// ���n�X�e�[�g�֑J��
void EnemyBoss::TransitionLandState()
{
    state = State::Land;

    // ���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Walk, false);
}
// ���n�X�e�[�g�X�V����
void EnemyBoss::UpdateLandState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;

    //�ړ�����

    // �ړ��X�e�[�g�ɐ؂�ւ�
    TransitionMoveState();
    
}

// ���S�X�e�[�g�֑J��
void EnemyBoss::TransitionDeadState()
{
    state = State::Dead;

    // ���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Walk, false);

    //���ʉ�
    step->Stop();
}
// ���S�X�e�[�g�X�V����
void EnemyBoss::UpdateDeadState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;
}

// ���Z�b�g�X�e�[�g�֑J��
void EnemyBoss::TransitionResetState()
{
    state = State::Reset;

    // ���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Warp, false);

    //�ʒu���Z�b�g
    ResetPosition();

    //�n�ʂɂ߂荞�ނ悤��
    SetGroundJudgeFlag(false);

    //���Z�b�g���Ԑݒ�
    resetTimer = resetTime;

    //���ʉ�
    //step->Stop();
    step->Play();

}
// ���Z�b�g�X�e�[�g�X�V����
void EnemyBoss::UpdateResetState(float elapsedTime)
{

    //�n�ʂ���o�����A���ԊO�Ȃ�Ȃ�
    if (position.y > 0.0f||resetTimer<=0.0f)
    {
        resetTimer = 0.0f;

        //�n�ʂɂ߂荞�܂Ȃ��悤��
        SetGroundJudgeFlag(true);
    }

    if (model->IsPlayAnimation())return;

    velocity = {};
    // �ړ��X�e�[�g�ɐ؂�ւ�
    TransitionMoveState();

}

// �X�s�[�h�A�b�v�X�e�[�g�֑J��
void EnemyBoss::TransitionSpeedUpState()
{
    state = State::SpeedUp;

    speedUpTimer = speedUpTime;

    // ���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Walk, false);

    //���ʉ�
    step->Play();
}
// �X�s�[�h�A�b�v�X�e�[�g�X�V����
void EnemyBoss::UpdateSpeedUpState(float elapsedTime)
{
    //TODO::�Y�ꂸ��
#if GAME
    velocity.z = acceleration * 2.0f;
#endif

    if (speedUpTimer > 0.0f)speedUpTimer -= elapsedTime;

    if(speedUpTimer<=0.0f)
    {
        speedUpTimer = 0.0f;
        // ���Z�b�g�X�e�[�g�ɐ؂�ւ�
        TransitionMoveState();
    }
}

void EnemyBoss::ResetPosition()
{
    float offsetZ = 12.5f;
    const DirectX::XMFLOAT3 pos = { 4.5f,-1.0f, player->GetPosition().z - offsetZ };
    SetPosition(pos);
    //TODO::�Y�ꂸ��
#if GAME
    const DirectX::XMFLOAT3 speed = {0.0f,30.0f,0.0f};
    velocity = speed;
#endif
}

void EnemyBoss::ResetOffsetUpdate()
{
    //���Z�b�g���ɃX�s�[�h�A�b�v�Ɉڍs���āA
    //�n�ʔ���t���O����炸�ɍs�����Ƃ����邽��

    if (resetTimer <= 0) {
        if (!GetGroundJudgeFlag()) {
            const DirectX::XMFLOAT3 pos = { position.x,0.0f, position.z };
            SetPosition(pos);
            SetGroundJudgeFlag(true);
        }
    }
    else resetTimer--;
}