#include <imgui.h>
#include "EnemySlime.h"
#include "WallManager.h"
#include "Collision.h"


//�R���X�g���N�^
EnemySlime::EnemySlime()
{
    //model = new Model("Data/model/Slime/Slime.mdl");
    model = new Model("Data/Model/Enemy/uni.mdl");

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;


    //���A�����ݒ�
    radius = 0.8f;
    height = 1.0f;

    this->type = Type::Uni;

    //������
    TransitionIdleState();
    angle.y = DirectX::XMConvertToRadians(90);


    //�G�t�F�N�g
    hitEffect = new Effect("Data/Effect/allDamage.efkefc");
}

//�f�X�g���N�^
EnemySlime::~EnemySlime()
{
    delete model;

    delete hitEffect;
}

//�X�V����
void EnemySlime::Update(float elapsedTime)
{
    //���S�m�F
    if (IsDead())return;

    // �X�e�[�g���̏���
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

    //���͍X�V
    UpdateVelocity(elapsedTime);

    //�������ړ�����
    offsetAreaWall(offsetX);

    //���G���ԍX�V
    UpdateInvincibleTimer(elapsedTime);

    //������΂�����
    LaunchedUpdate(elapsedTime);

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���A�j���[�V�����X�V����
    model->UpdateAnimation(elapsedTime);

    //���f���s��X�V
    model->UpdateTransform(transform);
}

void EnemySlime::ModelRender(RenderContext rc, ModelShader* modelShader)
{

    modelShader->Draw(rc, model);
}

//�`�揈��
//void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    shader->Draw(dc, model);
//
//#if EDIT
//    if (ImGui::Begin("enemy", nullptr, ImGuiWindowFlags_None))
//    {
//        //�g�����X�t�H�[��
//        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
//        {
//            ImGui::DragFloat3("Position", &position.x,0.5f);
//
//            //�ʒu
//            ImGui::Text("Num:%d", num);
//
//            if(editMove)
//            ImGui::Text("isMove");
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
//            int l = launch;
//            //������΂���Ă��邩
//            ImGui::InputInt("Launch", &l);
//            */
//
//        }
//    }
//    ImGui::End();
//#endif
//}

//���S�����Ƃ��ɌĂ΂��
void EnemySlime::OnDead()
{
    Destroy();
}

// �ҋ@�X�e�[�g�֑J��
void EnemySlime::TransitionIdleState()
{
    state = State::Idle;

    // ���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, true);


}
// �ҋ@�X�e�[�g�X�V����
void EnemySlime::UpdateIdleState(float elapsedTime)
{
    

}

// �_���[�W�X�e�[�g�֑J��
void EnemySlime::TransitionDamageState()
{
    state = State::Damage;

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, false);


}
// �_���[�W�X�e�[�g�X�V����
void EnemySlime::UpdateDamageState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;

}

// ���S�X�e�[�g�֑J��
void EnemySlime::TransitionDeadState()
{
    state = State::Dead;

    //���S�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, false);

}
// ���S�X�e�[�g�X�V����
void EnemySlime::UpdateDeadState(float elapsedTime)
{
    if (model->IsPlayAnimation())return;

}