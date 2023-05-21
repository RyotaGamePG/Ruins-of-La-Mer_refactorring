
#include "EnemyManager.h"
#include "WallManager.h"
#include "Collision.h"

#include "EnemyData.h"

#include "Effect.h"
#include <Windows.h>
#include <iostream>
#include <string>

const int STAGE_NUM = 3;
//�X�V����
void EnemyManager::Update(float elapsedTime)
{
    //�X�V����
    for (Enemy* enemy : enemies)
    {

        //�߂��Ȃ�
        if(IsNear(enemy->GetPosition().z, target.z, length))
            enemy->Update(elapsedTime);

        ////������x�̌��Ȃ�
        //if (IsBehind(enemy->GetPosition().z, target.z, length))
        //    enemy->Destroy();
        //
    }

    //�j������
    // ��enemy�͈̔�for������erase()����ƕs����������Ă��܂����߁A
    //   �X�V�������I�������ɔj�����X�g�ɋl�܂ꂽ�I�u�W�F�N�g���폜����B
    for (Enemy* enemy : removes)
    {
        //std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�(���ԂȂǂ��ϓ�����̂�)
        // ���̂��߂ɒT���ď������Ƃ��Ă���B
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);

        if (it != enemies.end())//�Ō�̉����Ȃ����̂���Ȃ��Ȃ�
        {
            enemies.erase(it);  //�e���X�g�̕�������
        }

        //�G�̔j������
        delete enemy;
    }
    //// �j�����X�g���N���A
    removes.clear();

    
    //�G���m�̏Փˏ���
    //CollisionEnemyVsEnemies();

    //�G�ƕǂ̍X�V����
    CollisionEnemyVsWall();

    
}




// ModelRender ����
void EnemyManager::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    for (Enemy* enemy : enemies)
    {

        {
            //�߂��Ȃ�
            if (IsNear(enemy->GetPosition().z, target.z, length))
            enemy->ModelRender(rc, modelShader);

        }
        //std::string debugOutput = "Rendered enemies: " + std::to_string(debugCounter) + "\n";
        //OutputDebugStringA(debugOutput.c_str());
    }
}

//�f�o�b�O�v���~�e�B�u�`��
void EnemyManager::DrawDebugPrimitive()
{
    //for (Enemy* enemy : enemies)
    //{
    //    enemy->DrawDebugPrimitive();
    //}

}

//�G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
    //std::string debugOutput = "Registered enemy, total enemies: " + std::to_string(enemies.size()) + "\n";
    //OutputDebugStringA(debugOutput.c_str());
}

//�G�l�~�[�S�폜
void EnemyManager::Clear()
{
    for (Enemy* enemy : enemies)
    {
        delete enemy;
    }
    enemies.clear();
}


//�G�l�~�[�ݒu
void EnemyManager::SetEnemy(int stageNum)
{
#if EDIT
    int un = 0;//�E�j
    int cn = 0;//�J�j
    int sn = 0;//�z�^�e
#endif
    //if (stageNum < 0 || stageNum >= STAGE_NUM) // �͈̓`�F�b�N��ǉ�
    //{
    //    // ������ stageNum �̏ꍇ�A�G���[���b�Z�[�W���o�͂��A�֐��𔲂���
    //    return;
    //}
    for (EnemyScript* enemy = enemyDataAll[stageNum]; enemy->type != -1; enemy++)
    {

        switch (enemy->type)
        {
        case EnemyType::uni:
        {

            EnemySlime* uni = new EnemySlime();
            uni->SetPosition(enemy->pos);
            EnemyManager::Instance().Register(uni);

#if EDIT
            uni->num = un++;
#endif
            //std::string debugOutput = "EnemyUni created at: (" + std::to_string(enemy->pos.x) + ", " + std::to_string(enemy->pos.y) + ", " + std::to_string(enemy->pos.z) + ")\n";
            //OutputDebugStringA(debugOutput.c_str());
            
        }
        break;
        case EnemyType::crabLR:
        {
            EnemyCrab* crabLR = new EnemyCrab(true,enemy->move);
            crabLR->SetPosition(enemy->pos);
            crabLR->setTatgetMovePosition();
            EnemyManager::Instance().Register(crabLR);
#if EDIT
            crabLR->num = cn++;
#endif
            //std::string debugOutput = "EnemycrabLR created at: (" + std::to_string(enemy->pos.x) + ", " + std::to_string(enemy->pos.y) + ", " + std::to_string(enemy->pos.z) + ")\n";
            //OutputDebugStringA(debugOutput.c_str());
        }
        break;
        case EnemyType::crabUD:
        {
            EnemyCrab* crabUD = new EnemyCrab(false, enemy->move);
            crabUD->SetPosition(enemy->pos);
            crabUD->setTatgetMovePosition();
            EnemyManager::Instance().Register(crabUD);
#if EDIT
            crabUD->num = cn++;
#endif
            //std::string debugOutput = "EnemycrabUD created at: (" + std::to_string(enemy->pos.x) + ", " + std::to_string(enemy->pos.y) + ", " + std::to_string(enemy->pos.z) + ")\n";
           //OutputDebugStringA(debugOutput.c_str());
        }
        break;

        case EnemyType::scallops:
        {
            EnemyScallops* scallops = new EnemyScallops;
            scallops->SetPosition(enemy->pos);
            EnemyManager::Instance().Register(scallops);
#if EDIT
            scallops->num = sn++;
#endif
            //std::string debugOutput = "Enemyscallpos created at: (" + std::to_string(enemy->pos.x) + ", " + std::to_string(enemy->pos.y) + ", " + std::to_string(enemy->pos.z) + ")\n";
           //OutputDebugStringA(debugOutput.c_str());

        }
        break;
        }

    }
}


//�G�l�~�[���m�̏Փˏ���
void EnemyManager::CollisionEnemyVsEnemies()
{
    //���ׂĂ̓G�Ƒ�������ŏՓˏ���
    for (int i = 0; i < enemies.size(); i++)
    {
        Enemy* enemyA = GetEnemy(i);

        for (int j = i + 1; j < enemies.size(); j++)
        {
            Enemy* enemyB = GetEnemy(j);

            //�����蔻��Ȃ�
            if (!enemyB->GetHitPositionCollision())continue;

            //�Փˏ���
            DirectX::XMFLOAT3 outPosition;
            if (Collision::InterSectCylinderVsCylinder(
                enemyA->GetPosition(),
                enemyA->GetRadius(),
                enemyA->GetHeight(),
                enemyB->GetPosition(),
                enemyB->GetRadius(),
                enemyB->GetHeight(),
                outPosition))
            
            {
                //�����o����̈ʒu�ݒ�
                enemyB->SetPosition(outPosition);
            }
            /*
            if (Collision::InterSectSphereVsSphere(
                enemyA->GetPosition(),
                enemyA->GetRadius(),
                enemyB->GetPosition(),
                enemyB->GetRadius(),
                outPosition))
                
            {
                //�����o����̈ʒu�ݒ�
                enemyB->SetPosition(outPosition);
            }
            */
        }
    }
}

//�G�l�~�[�ƕǂ̓����蔻��
void EnemyManager::CollisionEnemyVsWall()
{
    WallManager& wallManager = WallManager::Instance();

    
    //���ׂĂ̓G�Ƒ�������ŏՓˏ���
    for (int i = 0; i < enemies.size(); i++)
    {
        //�����̓����蔻��Ɣ�������Ȃ�����
        bool hit = false;

        Enemy* enemy = GetEnemy(i);

        //���ׂĂ̕ǂƑ�������ŏՓˏ���
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
                    enemy->GetPosition(),
                    enemy->GetRadius(),
                    enemy->GetHeight(),
                    outPosition))
                {
                    //���˂��ꂽ�E�j�����ʂ̕ǂɈ���������Ă��Ȃ��Ȃ�
                    if (!hit&&enemy->IsLaunch()&&!wall->GetInvincibleFlag())
                    {
                        if (enemy->getType() == Enemy::Type::Uni)
                        {
                            wall->ApplyDamage(1, 0.1f);
                            hit = true;
                        }
                    }
                    enemy->SetPosition(outPosition);
                }
            }
            if (hit)
            {
                if (enemy->hitEffect)
                {
                    DirectX::XMFLOAT3 position = enemy->GetPosition();
                    enemy->hitEffect->Play(position);
                }
                Remove(enemy);
                hit = !hit;
            }


        }
    }
}

//�G�l�~�[�폜
void EnemyManager::Remove(Enemy* enemy)
{
    for (Enemy* remove : removes)
    {
        if (remove == enemy)return;
    }

    ////�j�����X�g�ɒǉ�
    removes.emplace_back(enemy);
}
