#include "StageManager.h"
#include "StageData.h"
#include "Collision.h"

#include <Windows.h>
#include <sstream>


//�X�V����
void StageManager::Update(float elapsedTime)
{
    //�X�V����
    for (StageObj* stage : stages)
    {
        //�X�e�[�W�w�i �߂��Ȃ�
        if (stage->type == StageObj::Type::Stage)
        {
            if (IsNear(stage->GetPosition().z, target.z, length))
                stage->Update(elapsedTime);
        }
        else
            stage->Update(elapsedTime);
    }

    //�j������
     //��enemy�͈̔�for������erase()����ƕs����������Ă��܂����߁A
       //�X�V�������I�������ɔj�����X�g�ɋl�܂ꂽ�I�u�W�F�N�g���폜����B
    //for (StageObj* stage : removes)
    //{
    //    //std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�(���ԂȂǂ��ϓ�����̂�)
    //    // ���̂��߂ɒT���ď������Ƃ��Ă���B
    //    std::vector<StageObj*>::iterator it = std::find(stages.begin(), stages.end(), stage);

    //    if (it != stages.end())//�Ō�̉����Ȃ����̂���Ȃ��Ȃ�
    //    {
    //        stages.erase(it);  //�e���X�g�̕�������
    //    }

    //    //�X�e�[�W�̔j������
    //    delete stage;
    //}
    //// �j�����X�g���N���A
    //removes.clear();
}

void StageManager::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    //for (Stage* stage : stageVec)
    //{
    //    //std::ostringstream debugMessage;
    //    //debugMessage << "Rendering stage object at position (" << stage->GetPosition().x << ", " << stage->GetPosition().y << ", " << stage->GetPosition().z << ")\n";
    //    //OutputDebugStringA(debugMessage.str().c_str());

    //    stage->ModelRender(rc, modelShader);
    //}
    for (StageObj* stage : stages) // stageVec����stages�ɕύX
    {
        stage->ModelRender(rc, modelShader);
    }
}



//�X�e�[�W�ݒu
void StageManager::SetStageObj(int stageNum)
{

    //for (StageScript* stage = stageDataAll[stageNum]; stage->pos.y != -1; stage++)
    //{

    //    switch (stage->type)
    //    {
    //    case StageObjType::stage:
    //    {
    //        Stage* obj = new Stage;
    //        obj->SetPosition(stage->pos);
    //        StageManager::Instance().Register(obj);
    //    }
    //        break;
    //    case StageObjType::signboard:
    //    {
    //        Signboard* obj = new Signboard(stage->num);
    //        obj->SetPosition(stage->pos);
    //        StageManager::Instance().Register(obj);
    //    }
    //        break;
    //    }
    //}
    // 
    // 
    // stageNum���͈͊O�̏ꍇ�́A�֐��𔲂���
    //if (stageNum < 0 || stageNum >= 3)
    //{
    //    return;
    //}


    StageScript* stage = stageDataAll[stageNum];
    while (stage->pos.y != -1)
    {
        //std::ostringstream debugMessage;
        //debugMessage << "Stage object type: " << static_cast<int>(stage->type) << ", position: (" << stage->pos.x << ", " << stage->pos.y << ", " << stage->pos.z << ")\n";
        //OutputDebugStringA(debugMessage.str().c_str());
        switch (stage->type)
        {
        case StageObjType::stage:
        {
            Stage* obj = new Stage;
            obj->SetPosition(stage->pos);
            StageManager::Instance().Register(obj);
        }
        break;
        case StageObjType::signboard:
        {
            Signboard* obj = new Signboard(stage->num);
            obj->SetPosition(stage->pos);
            StageManager::Instance().Register(obj);
        }
        break;
        }

        ++stage;
    }
}



//�`�揈��
//void StageManager::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    for (StageObj* stage : stages)
//    {
//        //�X�e�[�W�w�i�@�߂��Ȃ�
//        if (stage->type == StageObj::Type::Stage) { //�w�i
//            if (IsNear(stage->GetPosition().z, target.z, length))
//            {
//                stage->Render(dc, shader);
//            }
//        }
//        else
//            stage->Render(dc, shader);
//    }
//}

//�S�̈ʒu���Z
void StageManager::AddPos(DirectX::XMFLOAT3 pos)
{
    for (StageObj* stage : stages)
    {
        stage->AddPos(pos);
    }

}

//�S�̈ʒu�ݒ�
void StageManager::SetPosition(DirectX::XMFLOAT3 pos)
{
    for (StageObj* stage : stages)
    {
        stage->SetPosition(pos);
    }

}


//�f�o�b�O�v���~�e�B�u�`��
void StageManager::DrawDebugPrimitive()
{
    //for (StageObj* stage : stages)
    //{
    //    stage->DrawDebugPrimitive();
    //}
}

//�G�l�~�[�o�^
void StageManager::Register(StageObj* stage)
{
    stages.emplace_back(stage);
}

//�G�l�~�[�S�폜
void StageManager::Clear()
{
    for (StageObj* stage : stages)
    {
        delete stage;
    }
    stages.clear();

}

//�G�l�~�[�폜
void StageManager::Remove(StageObj* stage)
{
    //�j�����X�g�ɒǉ�
    removes.emplace_back(stage);
}