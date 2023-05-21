#include "WallManager.h"
#include "Collision.h"
#include "StageData.h"

const int STAGE_NUM = 3;

//�X�V����
void WallManager::Update(float elapsedTime)
{
    //�X�V����
    for (Wall* wall : walls)
    {
        wall->Update(elapsedTime);
    }

    //�j������
    // ��enemy�͈̔�for������erase()����ƕs����������Ă��܂����߁A
    //   �X�V�������I�������ɔj�����X�g�ɋl�܂ꂽ�I�u�W�F�N�g���폜����B
    for (Wall* wall : removes)
    {
        //std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�(���ԂȂǂ��ϓ�����̂�)
        // ���̂��߂ɒT���ď������Ƃ��Ă���B
        std::vector<Wall*>::iterator it = std::find(walls.begin(), walls.end(), wall);

        if (it != walls.end())//�Ō�̉����Ȃ����̂���Ȃ��Ȃ�
        {
            walls.erase(it);  //�e���X�g�̕�������
        }

        //�G�̔j������
        delete wall;
    }
    // �j�����X�g���N���A
    removes.clear();


    //�G���m�̏Փˏ���
    //CollisionEnemyVsEnemies();


}

//�t���O�ݒu
void WallManager::SetWall(int stageNum)
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
        case StageObjType::enemyWall:
        {
            
            Wall* obj = new Wall;
            obj->SetPosition(stage->pos);
            //�����ǂȂ�
            if (obj->GetPosition().y < 0)obj->SetInvincibleFlag(true);
            WallManager::Instance().Register(obj);
            
        }
        break;
        }
    }
}


//�`�揈��
//void WallManager::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    for (Wall* wall : walls)
//    {
//        wall->Render(dc, shader);
//    }
//
//}

//void WallManager::ModelRender(RenderContext rc, ModelShader* modelShader)
//{
//    modelShader->Draw(rc, );
//}

void WallManager::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    for (Wall* wall : walls)
    {
        wall->ModelRender(rc, modelShader);
    }
}

//�f�o�b�O�v���~�e�B�u�`��
void WallManager::DrawDebugPrimitive()
{
    for (Wall* wall : walls)
    {
        wall->DrawDebugPrimitive();
    }
}

void WallManager::SetCollisionFlag()
{
    for (Wall* wall : walls)
    {
        wall->SetCollisionFlag(!wall->GetCollisionFlag());
    }

}

//�G�l�~�[�o�^
void WallManager::Register(Wall* enemy)
{
    walls.emplace_back(enemy);
}

//�G�l�~�[�S�폜
void WallManager::Clear()
{
    for (Wall* wall : walls)
    {
        delete wall;
    }
    walls.clear();

}

//�G�l�~�[�폜
void WallManager::Remove(Wall* wall)
{
    //�j�����X�g�ɒǉ�
    removes.emplace_back(wall);
}