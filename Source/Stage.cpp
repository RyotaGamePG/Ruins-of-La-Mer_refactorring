#include <imgui.h>

#include "Stage.h"
#include "Graphics/Graphics.h"
#include "easing.h"




//------------------------------------

//-----Stage�N���X----------

//------------------------------------
//�R���X�g���N�^
Stage::Stage()
{
    //�X�e�[�W���f����ǂݍ���
    //model = new Model("Data/Model/Stage/back_ground.mdl");
    model = new Model("Data/Model/background.mdl");
    //model = new Model("Data/Model/select.mdl");
    scale.x = scale.y = scale.z = 0.01f;
    //scale.x = scale.y = scale.z = 0.1f;

    type = Type::Stage;
    
}

Stage::~Stage()
{
    //�X�e�[�W���f����j��
    delete model;
}

//�X�V����
void Stage::Update(float elapsedTime)
{
    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    model->UpdateTransform(transform);
}

void Stage::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);
}

//void Stage::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
//    shader->Draw(dc, model);
//
//   
//}

//�f�o�b�O�v���~�e�B�u�`��
void Stage::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    ////�Փ˔���p�̃f�o�b�O����`��
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

}


//------------------------------------

//-----Signboard�N���X----------

//------------------------------------

Signboard::Signboard(int index)
{
    switch (index)
    {
    case 0://�_�b�V���`���[�g���A��
        model = new Model("Data/Model/Text/tutorial01_dush.mdl");
        scale.x = scale.y = scale.z = 0.05f;
        angle.y = DirectX::XMConvertToRadians(90);
        break;
    case 1://�W�����v�`���[�g���A��
        model = new Model("Data/Model/Text/tutorial02_Jump.mdl");
        scale.x = scale.y = scale.z = 0.05f;
        angle.y = DirectX::XMConvertToRadians(90);
        break;
    case 2://�ǉ󂷃`���[�g���A��
        model = new Model("Data/Model/Text/tutorial03_wallbreak.mdl");
        scale.x = scale.y = scale.z = 0.05f;
        angle.y = DirectX::XMConvertToRadians(90);
        break;
    case 3://�z�^�e�`���[�g���A��
        model = new Model("Data/Model/Text/tutorial04_hotatejump.mdl");
        scale.x = scale.y = scale.z = 0.05f;
        angle.y = DirectX::XMConvertToRadians(90);
        break;
    case 4://���̃X�e�[�W
        model = new Model("Data/Model/Stage/exit.mdl");
        scale.x = scale.y = scale.z = 0.02f;
        angle.y = DirectX::XMConvertToRadians(90);
        break;
    }


    angle.x = DirectX::XMConvertToRadians(-15);
   
    /*
    //�X�e�[�W���f����ǂݍ���
    model = new Model("Data/Model/Stage/wall.mdl");
    //model = new Model("Data/Model/select.mdl");
    scale.x = scale.y = scale.z = 0.001f;
    //scale.x = scale.y = scale.z = 0.01f;
    */
    

    //���f���s��X�V
    model->UpdateTransform(transform);

    type = Type::Signboard;
}

Signboard::~Signboard()
{
    delete model;
}

//�X�V����
void Signboard::Update(float elapsedTime)
{
    //�V��
    
    time++;
    /*
    float scale0 = Bounce::easeIn(time/30, 0.0f, 1.0f, 100.0f);
    scale.x = scale0;
    scale.y = scale0;
    scale.z = scale0;
    */

    /*
    float scale0 = sinf(time / 60) * 0.001f + 0.002;
    //float scale0 = sinf(time / 60) * 0.01f + 0.02;
    scale.x = scale0;
    scale.y = scale0;
    scale.z = scale0;
    */

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    model->UpdateTransform(transform);

}

void Signboard::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);
}

//void Signboard::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
//    shader->Draw(dc, model);
//}

//�f�o�b�O�v���~�e�B�u�`��
void Signboard::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    ////�Փ˔���p�̃f�o�b�O����`��
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}

//-----�Z���N�g��N�N���X----------

SelectSignBoard::SelectSignBoard(int index)
{
    //�X�e�[�W���f����ǂݍ���
    switch (index)
    {
    case 0://�^�C�g���w�i
        model = new Model("Data/Model/Select/title_background.mdl");
        scale.x = scale.y = scale.z = 0.03f;
        isBackGround = true;
        break;
    case 1: //�^�C�g������
        model = new Model("Data/Model/Select/title.mdl");
        scale.x = scale.y = scale.z = 0.1f;
        break;
    case 2://�X�e�[�W�P
        model = new Model("Data/Model/Select/icon1.mdl");
        scale.x = scale.y = scale.z = 0.1f;
        break;
    case 3://�X�e�[�W�Q
        model = new Model("Data/Model/Select/icon2.mdl");
        scale.x = scale.y = scale.z = 0.1f;
        break;

    case 4://�X�e�[�W3
        model = new Model("Data/Model/Select/icon3.mdl");
        scale.x = scale.y = scale.z = 0.1f;
        break;

    case 5: //�Z���N�g����
        model = new Model("Data/Model/Text/select.mdl");
        scale.x = scale.y = scale.z = 0.1f;
        break;

    case 6://���[�f�B���O����
        model = new Model("Data/Model/Text/loading.mdl");
        scale.x = scale.y = scale.z = 0.08f;
        isBackGround = true;
        model->PlayAnimation(0, true);
        break;
    }

    type = Type::SelectSignboard;
}

SelectSignBoard::~SelectSignBoard()
{
    delete model;
}

void SelectSignBoard::Update(float elapsedTime)
{
    if (!isBackGround)
    {
        if (isMove)
        {
            ScaleMove();
        }
        else
        {
            //�X�P�[�����Z�b�g
            scale.x = 0.1;
            scale.y = 0.1;
            scale.z = 0.1;
            time = 0.0f;
        }
    }

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���A�j���[�V�����X�V����
    model->UpdateAnimation(elapsedTime);

    //���f���s��X�V
    model->UpdateTransform(transform);

    time++;
}

void SelectSignBoard::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);
}

//void SelectSignBoard::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
//    shader->Draw(dc, model);
//
//}

void SelectSignBoard::ScaleMove()
{
    float scale0 = sinf(time/20)*0.02f;
    //float scale0 = sinf(time / 60) * 0.01f + 0.02;
    scale.x = 0.1f+scale0;
    scale.y = 0.1f+scale0;
    scale.z = 0.1f+scale0;
}


//-----�Q�[���̃Z���N�g�N���X----------

GameSelectSignBoard::GameSelectSignBoard(int index)
{
    //�X�e�[�W���f����ǂݍ���
    switch (index)
    {
    case 0://�N���A�̏ꍇ�̕���
        model = new Model("Data/Model/Text/clear.mdl");
        scale.x = scale.y = scale.z = 0.015f;
        text = true;
        break;
    case 1://�N���A�I����
        model = new Model("Data/Model/Text/next.mdl");
        scale.x = scale.y = scale.z = 0.01f;
        break;
    case 2://�N���A�I����
        model = new Model("Data/Model/Text/select.mdl");
        scale.x = scale.y = scale.z = 0.01f;
        break;
    case 3: // �Q�[���I�[�o�[�̏ꍇ�̕���
        model = new Model("Data/Model/Text/over.mdl");
        scale.x = scale.y = scale.z = 0.015f;
        text = true;
        break;
    case 4: // �Q�[���I�[�o�[�I����
        model = new Model("Data/Model/Text/retry.mdl");
        scale.x = scale.y = scale.z = 0.01f;
        break;
    case 5: //�@�Q�[���I�[�o�[�I����
        model = new Model("Data/Model/Text/select.mdl");
        scale.x = scale.y = scale.z = 0.01f;
        break;

    }

    type = Type::GameSelectSignboard;

    angle.y = DirectX::XMConvertToRadians(90);
}

GameSelectSignBoard::~GameSelectSignBoard()
{
    delete model;
}

void GameSelectSignBoard::Update(float elapsedTime)
{
    //�ړ�����
    if (isMove)
    {
        NextPosMove();
    }
    //�X�P�[���A�j���[�V����
    else if (isScaleMove)
    {
        ScaleMove();
    }
    //�����Ȃ��ꍇ
    else if(text)
    {
        //�X�P�[�����Z�b�g
        scale.x = 0.015f;
        scale.y = 0.015f;
        scale.z = 0.015f;
        time = 0.0f;
    }
     else
    {
    //�X�P�[�����Z�b�g
    scale.x = 0.01f;
    scale.y = 0.01f;
    scale.z = 0.01f;
    time = 0.0f;
    }

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    model->UpdateTransform(transform);

    time++;
}

void GameSelectSignBoard::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);
}

//void GameSelectSignBoard::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
//    shader->Draw(dc, model);
//
//}

void GameSelectSignBoard::ScaleMove()
{
    float scale0 = sinf(time / 20) * 0.002f;
    //float scale0 = sinf(time / 60) * 0.01f + 0.02;
    scale.x = 0.01f + scale0;
    scale.y = 0.01f + scale0;
    scale.z = 0.01f + scale0;
}

void GameSelectSignBoard::NextPosMove()
{
    DirectX::XMFLOAT3 old = oldPos;
    DirectX::XMFLOAT3 next = nextPos;

    float x = Sine::easeInOut(time, old.x, next.x - old.x, totalTime);//����
    float y = Sine::easeInOut(time, old.y, next.y - old.y, totalTime);//����
    float z = Sine::easeInOut(time, old.z, next.z - old.z, totalTime);//����

    SetPosition(DirectX::XMFLOAT3(x,y,z));
    //�ړ����Ԉȏ�Ȃ�
    if (time >= totalTime)
    {
        SetMove(false);
    }
}

//-----�X�e�[�W�I�u�W�F�N�g�̊��N���X----------

void StageObj::UpdateTransform()
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

void StageObj::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);
}

void GameTutorialSignBoard::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);
}
