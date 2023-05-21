#include "FlagAreaManager.h"
#include "FlagArea.h"
#include "Collision.h"

#include "SceneManager.h"
#include "SceneTitle.h"
const int STAGE_NUM = 3;


//�X�V����
void FlagAreaManager::Update(float elapsedTime)
{
    //TODO::�Y�ꂸ��

#if GAME
    //�X�V����
    for (FlagArea* area : areas)
    {
        switch (area->getType()) {
        case FlagArea::Type::SpeedUp:
            if (area->OverlapArea(boss->GetPosition()))
            {
                boss->SpeedUp();
                Remove(area);   //����
            }
            break;
        case FlagArea::Type::Goal:
            if (player->GetState() != Player::State::Clear)
            {
                //player->AddImpulse(DirectX::XMFLOAT3(3.0f, 0, 3.0f));
                //SceneManager::Instance().ChangeScene(new SceneTitle);
                if (area->OverlapArea(player->GetPosition()))
                {
                    player->TransitionClearState();
                    Remove(area);   //����
                }
            }
            break;
        }
    }
#endif
    
    //�j������
    for (FlagArea* area : removes)
    {
        //std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�(���ԂȂǂ��ϓ�����̂�)
        // ���̂��߂ɒT���ď������Ƃ��Ă���B
        std::vector<FlagArea*>::iterator it = std::find(areas.begin(), areas.end(), area);

        if (it != areas.end())//�Ō�̉����Ȃ����̂���Ȃ��Ȃ�
        {
            areas.erase(it);  //�e���X�g�̕�������
        }

        //�G���A�̔j������
        delete area;
    }
    // �j�����X�g���N���A
    removes.clear();


}

//�`�揈��
//void FlagAreaManager::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    /*
//    for (FlagArea* enemy : areas)
//    {
//        enemy->Render(dc, shader);
//    }
//    */
//}

void FlagAreaManager::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    //modelShader->Draw(rc, model);
}

//�f�o�b�O�v���~�e�B�u�`��
void FlagAreaManager::DrawDebugPrimitive()
{
    for (FlagArea* area : areas)
    {
        area->DrawDebugPrimitive();
    }

}


//�t���O�ݒu
void FlagAreaManager::SetFlagArea(int stageNum)
{
    //if (stageNum < 0 || stageNum >= STAGE_NUM) // �͈̓`�F�b�N��ǉ�
    //{
    //    // ������ stageNum �̏ꍇ�A�G���[���b�Z�[�W���o�͂��A�֐��𔲂���
    //    return;
    //}

    for (StageScript* stage = stageDataAll[stageNum]; stage->pos.y != -1; stage++)
    {
        switch (stage->type)
        {
        case StageObjType::flagSpeedUp:
        {
            //float x = stage->pos.x - length;
            float x = 0;
            float z = stage->pos.z + length;
            DirectX::XMFLOAT3 posRU = { x,0,z };	//�E��
            //x = stage->pos.x + length;
            x = 10;
            z = stage->pos.z - length;
            DirectX::XMFLOAT3 posLD = { x,0,z };   //����
            FlagArea* obj = new FlagArea(posRU,posLD,FlagArea::Type::SpeedUp);
            FlagAreaManager::Instance().Register(obj);
        }
        break;
        case StageObjType::flagGoal:
        {
            float x = 0;
            float z = stage->pos.z + length;
            DirectX::XMFLOAT3 posRU = { x,0,z };	//�E��
            x = 10;
            z = stage->pos.z - length;
            DirectX::XMFLOAT3 posLD = { x,0,z };   //����
            FlagArea* obj = new FlagArea(posRU, posLD, FlagArea::Type::Goal);
            FlagAreaManager::Instance().Register(obj);
        }
        break;
        }
    }
}

//�G���A�o�^
void FlagAreaManager::Register(FlagArea* flagArea)
{
    areas.emplace_back(flagArea);
}

//�G���A�S�폜
void FlagAreaManager::Clear()
{
    for (FlagArea* flagArea : areas)
    {
        delete flagArea;
    }
    areas.clear();

}

//�G���A�폜
void FlagAreaManager::Remove(FlagArea* flagArea)
{
    for (FlagArea* remove : removes)
    {
        if (remove == flagArea)return;
    }

    //�j�����X�g�ɒǉ�
    removes.emplace_back(flagArea);
}