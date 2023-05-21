#include <imgui.h>
#include "EnemyScallops.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "Graphics/Graphics.h"

//�R���X�g���N�^
EnemyScallops::EnemyScallops()
{
    //model = new Model("Data/Model/Slime/Slime.mdl");
    model = new Model("Data/Model/Enemy/hotate.mdl");

    //���f�����傫���̂ŃX�P�[�����O
    //scale.x = scale.y = scale.z = 0.02f;
    scale.x = scale.y = scale.z = 0.01f;


    //���A�����ݒ�
    radius = 1.5f;
    height = 1.0f;

    //�̗͂̐ݒ�
    health = 3;

    //�����͂̐ݒ�
    acceleration = 3.0f;

    type = Type::Scallops;

    //������

    //TransitionIdleState();

}

//�f�X�g���N�^
EnemyScallops::~EnemyScallops()
{
    delete model;
}

//�X�V����
void EnemyScallops::Update(float elapsedTime)
{
    //���S�m�F
    if (IsDead())return;

    // �X�e�[�g���̏���
    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Open:
        UpdateOpenState(elapsedTime);
        break;
    }


    //�G�ƃ{�X�̏Փˏ���
    //CollisionEnemyVsBoss();



    //���͍X�V
    //UpdateVelocity(elapsedTime);

    //�������ړ�����
    offsetAreaWall(offsetX);


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
//void EnemyScallops::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    shader->Draw(dc, model);
//
//
//#if EDIT
//    if (ImGui::Begin("Scallops", nullptr, ImGuiWindowFlags_None))
//    {
//        //�g�����X�t�H�[��
//        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
//        {
//            //�ʒu
//            ImGui::InputFloat3("Position", &position.x);
//
//            ImGui::Text("Num:%d", num);
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
//
//#endif
//}

void EnemyScallops::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);
}

//���S�����Ƃ��ɌĂ΂��
void EnemyScallops::OnDead()
{
}

//�_���[�W����������Ƃ��ɌĂ΂��
void EnemyScallops::OnDamaged()
{
}

//�G�ƃ{�X�̏Փˏ���
void EnemyScallops::CollisionEnemyVsBoss()
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
void EnemyScallops::TransitionIdleState()
{
    state = State::Idle;

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, true);
}
// �ҋ@�X�e�[�g�X�V����
void EnemyScallops::UpdateIdleState(float elapsedTime)
{
    if (stopTimer > 0.0f)return;

    float move =velocity.x + velocity.y + velocity.z;
    //�O����Ռ��������Ȃ�
    if (move != 0.0f)
    {
        TransitionOpenState();
    }
}

// ���S�X�e�[�g�֑J��
void EnemyScallops::TransitionOpenState()
{
    state = State::Open;
    // ���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Open, false);
}
// ���S�X�e�[�g�X�V����
void EnemyScallops::UpdateOpenState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;
    velocity = {};      //�t���O����̏Ռ�������
    TransitionIdleState();
}


void EnemyScallops::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //�Փ˔���p�̃f�o�b�O�~����`��
    if(state==State::Idle)
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 1, 1, 1));
    else
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 1, 1));

}