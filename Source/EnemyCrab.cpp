#include <imgui.h>
#include "EnemyCrab.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "Graphics/Graphics.h"


//�R���X�g���N�^
EnemyCrab::EnemyCrab(bool type,bool move)
    :crabType(type)
{
    //model = new Model("Data/Model/Slime/Slime.mdl");
    model = new Model("Data/Model/Enemy/kani.mdl");

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.02f;


    //���A�����ݒ�
    radius = 1.0f;
    height = 1.5f;

    //�̗͂̐ݒ�
    health = 3;

    //�����͂̐ݒ�
    acceleration = 3.0f;

    this->type = Type::Crab;

    //������

    //�����̓����ݒ�
    if (move)isPos1 = true;
    else isPos1 = false;

    //TransitionIdleState();

    if(type)angle.y = DirectX::XMConvertToRadians(90);
    else    angle.y = DirectX::XMConvertToRadians(180);

}

//�f�X�g���N�^
EnemyCrab::~EnemyCrab()
{
    delete model;
}

//�X�V����
void EnemyCrab::Update(float elapsedTime)
{
    //���S�m�F
    if (IsDead())return;

    //TODO::�Y�ꂸ��
    //�G�f�B�^�[���ǂ��p
#if EDIT
    state = State::Idle;
#endif
    // �X�e�[�g���̏���
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


    //�G�ƃ{�X�̏Փˏ���
    //CollisionEnemyVsBoss();
    
    

    //���͍X�V
    UpdateVelocity(elapsedTime);

    //�������ړ�����
    offsetAreaWall(offsetX);
    

    //���G���ԍX�V
    UpdateInvincibleTimer(elapsedTime);

    //�ҋ@���ԍX�V
    UpdateStopTimer(elapsedTime);

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���A�j���[�V�����X�V����
    model->UpdateAnimation(elapsedTime);

    //���f���s��X�V
    model->UpdateTransform(transform);
}

//�`�揈��
//void EnemyCrab::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    shader->Draw(dc, model);
//
//#if EDIT
//    if (ImGui::Begin("Crab", nullptr, ImGuiWindowFlags_None))
//    {
//        //�g�����X�t�H�[��
//        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
//        {
//            //�ʒu
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
//            //��]
//            DirectX::XMFLOAT3 a;
//            a.x = DirectX::XMConvertToDegrees(angle.x);
//            a.y = DirectX::XMConvertToDegrees(angle.y);
//            a.z = DirectX::XMConvertToDegrees(angle.z);
//            ImGui::InputFloat3("Angle", &a.x);
//            angle.x = DirectX::XMConvertToRadians(a.x);
//            angle.y = DirectX::XMConvertToRadians(a.y);
//            angle.z = DirectX::XMConvertToRadians(a.z);
//            //�X�P�[��
//            ImGui::InputFloat3("Scale", &scale.x);
//
//            //�̗�
//            ImGui::InputInt("Helth", &health);
//
//            //����
//            ImGui::InputFloat("acceleration", &acceleration);
//
//            //�ҋ@����
//            ImGui::InputFloat("stopTimer", &stopTimer);
//
//
//            int s = (int)state;
//            //���
//            ImGui::InputInt("state", &s);
//            */
//
//        }
//    }
//    ImGui::End();
//#endif
//}

//���S�����Ƃ��ɌĂ΂��
void EnemyCrab::OnDead()
{
    TransitionDeadState();
}

//�_���[�W����������Ƃ��ɌĂ΂��
void EnemyCrab::OnDamaged()
{
    TransitionDamageState();
}

//�G�ƃ{�X�̏Փˏ���
void EnemyCrab::CollisionEnemyVsBoss()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //���ׂĂ̓G�Ƒ�������ŏՓˏ���
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

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
            ApplyDamage(1, 0.5f);
            enemy->SetPosition(outPosition);
            enemy->Destroy();
        }
    }

}


// �ҋ@�X�e�[�g�֑J��
void EnemyCrab::TransitionIdleState()
{
    state = State::Idle;

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, true);
}
// �ҋ@�X�e�[�g�X�V����
void EnemyCrab::UpdateIdleState(float elapsedTime)
{
    if (stopTimer > 0.0f)return;

    isAreaWall = false;


    //���ɐݒ肳�ꂽ����
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

// �E�ړ��X�e�[�g�֑J��
void EnemyCrab::TransitionRightMoveState()
{
    state = State::Right;

    // ����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Running, true);
}
// �E�ړ��X�e�[�g�X�V����
void EnemyCrab::UpdateRightMoveState(float elapsedTime)
{
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //moveVec.z *= 1.2f;
    moveVec.x = 0.0f;
    moveVec.y = 0.0f;
    moveVec.z +=1.0f;
    //�ړ�����
    Move(moveVec.x, moveVec.z, moveSpeed);

    if (position.z > pos0.z-radius)
    {
        position.z = pos0.z - radius;
        stopTimer = stopTime;
        isPos1 = true; //���͍�
        TransitionIdleState();
    }
}

// ���ړ��X�e�[�g�֑J��
void EnemyCrab::TransitionLeftMoveState()
{
    state = State::Left;

    // ����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Running, true);
}
// ���ړ��X�e�[�g�X�V����
void EnemyCrab::UpdateLeftMoveState(float elapsedTime)
{
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //moveVec.z *= 1.2f;
    moveVec.x = 0.0f;
    moveVec.y = 0.0f;
    moveVec.z -= 1.0f;
    //�ړ�����
    Move(moveVec.x, moveVec.z, moveSpeed);
    if (position.z < pos1.z+radius)
    {
        position.z = pos1.z + radius;
        stopTimer = stopTime;
        isPos1 = false;//���͉E
        TransitionIdleState();
    }
}


// ��ړ��X�e�[�g�֑J��
void EnemyCrab::TransitionUpMoveState()
{
    state = State::Up;

    // ����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Running, true);
}
// ��ړ��X�e�[�g�X�V����
void EnemyCrab::UpdateUpMoveState(float elapsedTime)
{
    //��]�������������߂�
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    moveVec.x += 1.0f;
    moveVec.y = 0.0f;
    moveVec.z = 0.0f;
    //�ړ�����
    Move(moveVec.x, moveVec.z, moveSpeed);
    if (isAreaWall||position.x > pos0.x - radius)
    {
        stopTimer = stopTime;
        isPos1 = true;//���͉�
        TransitionIdleState();
    }
}


// ���ړ��X�e�[�g�֑J��
void EnemyCrab::TransitionDownMoveState()
{
    state = State::Down;

    // ����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Running, true);
}
// ���ړ��X�e�[�g�X�V����
void EnemyCrab::UpdateDownMoveState(float elapsedTime)
{
    DirectX::XMFLOAT3 moveVec = GetMoveVec();
    moveVec.x -= 1.0f;
    moveVec.y = 0.0f;
    moveVec.z = 0.0f;
    //�ړ�����
    Move(moveVec.x, moveVec.z, moveSpeed);
    if (isAreaWall || position.x < pos1.x + radius)
    {
        stopTimer = stopTime;
        isPos1 = false;//���͏�
        TransitionIdleState();
    }
}


// ���n�X�e�[�g�֑J��
void EnemyCrab::TransitionLandState()
{
    state = State::Land;

    // ���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, false);
}
// ���n�X�e�[�g�X�V����
void EnemyCrab::UpdateLandState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;

    //�ړ�����

    // �ړ��X�e�[�g�ɐ؂�ւ�
    TransitionRightMoveState();

}

// �_���[�W�X�e�[�g�֑J��
void EnemyCrab::TransitionDamageState()
{
    state = State::Damage;

    // ���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, false);


}
// �_���[�W�X�e�[�g�X�V����
void EnemyCrab::UpdateDamageState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;

    // �ړ��X�e�[�g�ɐ؂�ւ�
    TransitionRightMoveState();
}


// ���S�X�e�[�g�֑J��
void EnemyCrab::TransitionDeadState()
{
    state = State::Dead;

    // ���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, false);
}
// ���S�X�e�[�g�X�V����
void EnemyCrab::UpdateDeadState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;
}

//���E�̈ړ���Z�b�g
void EnemyCrab::setTatgetMovePosition()
{
    if (crabType)
    {
        //�E�ʒu�ݒ�
        pos0 = { position.x,position.y,position.z + moveRange };
        //���ʒu�ݒ�
        pos1 = { position.x,position.y,position.z - moveRange };
    }
    else
    {
        //��ʒu�ݒ�
        pos0 = { position.x + moveRange ,position.y,position.z };
        //���ʒu�ݒ�
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
        if (!isPos1)//�E
            moveVec = { position.x,position.y,pos0.z - position.z };
        if (isPos1)//��
            moveVec = { position.x,position.y,pos1.z - position.z };
        
    }
    else
    {
        if (!isPos1)//��
            moveVec = { pos0.x - position.x,position.y,position.z };
        if (isPos1)//��
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

    //�Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
    debugRenderer->DrawCylinder(pos0, radius, height, DirectX::XMFLOAT4(0, 0, 0.7f, 1));
    debugRenderer->DrawCylinder(pos1, radius, height, DirectX::XMFLOAT4(0, 0.7f, 0, 1));
}