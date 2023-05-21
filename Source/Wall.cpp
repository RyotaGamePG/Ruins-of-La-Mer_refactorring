#include <imgui.h>
#include "Wall.h"
#include "WallManager.h"

#include "Graphics/Graphics.h"

Wall::Wall()
{

    //model = new Model("Data/model/Stage/wall.mdl");
    model = new Model("Data/model/Enemy/enemywall.mdl");

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;


    //���A�����ݒ�
    radius = 5.0f;
    height = 5.0f;

    //�����蔻��p�ݒ�
    hitPosRadius = 0.5f;
    hitPosHeight = 9.75f;

    //�p�x
    angle.y = DirectX::XMConvertToRadians(90);

    //���ʉ�
    damageSound = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/bossmove.wav", false);
    //breakSound = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/break.wav", false);

    //�G�t�F�N�g
    damageEffect = new Effect("Data/Effect/wallBreak.efkefc");
}

Wall::~Wall()
{
    delete model;

    delete damageEffect;
}

//�X�V����
void Wall::Update(float elapsedTime)
{
    for (int i = 0; i < hPosNum; i++)
    {
        float left = position.x - radius*2;
        float r = radius*2 / hPosNum;
        //float x = left + (r * (i+1));
        float x = left+i;
        float z = position.z+ hitPosRadius / 2;
        DirectX::XMFLOAT3   pos = { x,position.y,z };
        hitPosition[i] = pos;
    }

    //���G���ԍX�V����
    UpdateInvincibleTimer(elapsedTime);

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();


    //���f���s��X�V
    model->UpdateTransform(transform);
}

void Wall::ModelRender(RenderContext rc, ModelShader* modelShader)
{

    modelShader->Draw(rc,model);
}

//�`�揈��
//void Wall::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    //�����蔻�������Ă���Ȃ�`��
//    if (CollisionFlag)
//    {
//        shader->Draw(dc, model);
//    }
//
//#if EDIT
//
//    if (ImGui::Begin("Wall", nullptr, ImGuiWindowFlags_None))
//    {
//        //�g�����X�t�H�[��
//        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
//        {
//            //�ʒu
//            ImGui::InputFloat3("Position", &position.z);
//
//            //�̗�
//            ImGui::InputInt("Helth", &health);
//
//            unsigned int flag = (unsigned int)CollisionFlag;
//            ImGui::CheckboxFlags("flag", &flag, flag);
//            
//            /*
//            //�ʒu
//            for (int i = 0; i < hPosNum; i++)
//            {
//                ImGui::InputFloat("hitPos%d", &hitPosition[i].z,i);
//            }
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
//            //����
//            ImGui::InputFloat("acceleration", &acceleration);
//            */
//        }
//    }
//    ImGui::End();
//#endif
//}

//�s��X�V����
void Wall::UpdateTransform()
{
    //�X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //��]�s����쐬
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    //�ʒu�s����쐬
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    //  3�̍s���g�����A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;
    //�v�Z�������[���h�s������o��
    DirectX::XMStoreFloat4x4(&transform, W);//transform�ɏ�������ł�...?

}

//�f�o�b�O�v���~�e�B�u�`��
void Wall::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    ////�Փ˔���p�̃f�o�b�O����`��
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    //�Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    //�����蔻�薳��
    if (!CollisionFlag)return;

    for (int i = 0; i < hPosNum; i++)
    {
        debugRenderer->DrawCylinder(hitPosition[i], hitPosRadius, hitPosHeight, DirectX::XMFLOAT4(0.3f, 0, 0, 1));
    }
}

//�_���[�W��^����
bool Wall::ApplyDamage(int damage, float invincibleTime)
{
    //�_���[�W��0�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
    if (damage == 0)return false;

    //���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
    if (health <= 0)return false;

    //���G���Ԓ��̓_���[�W��^���Ȃ�
    if (invincibleTimer > 0.0f)return false;

    //���G�Ȃ�_���[�W��^���Ȃ�
    if (invincible)return false;

    // �_���[�W����
    //health -= damage;

    //���G���Ԑݒ�
    //invincibleTimer = invincibleTime;

    //���ɏ����ړ�
    position.y -= 3.3f;

    //���ʉ��ݒ�
    damageSound = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/bossmove.wav", false);
    damageSound->Play();

    //�G�t�F�N�g
    for (int i = 0; i < hPosNum; i++)
    {
        DirectX::XMFLOAT3 pos = hitPosition[i];
        damageEffect->Play(pos);
    }

    //���S�ʒm
    if (health <= 0)
    {
        OnDead();
    }
    //�_���[�W�ʒm
    else
    {
        //OnDamaged();
    }

    //���N��Ԃ��ύX�����ꍇ��true��Ԃ�
    return true;

}

void Wall::UpdateInvincibleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}


void Wall::OnDead()
{
    CollisionFlag = false;
    //WallManager::Instance().Remove(this);
}