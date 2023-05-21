
#include "EnemyManager.h"
#include "WallManager.h"
#include "Collision.h"

#include "EnemyData.h"

#include "Effect.h"
#include <Windows.h>
#include <iostream>
#include <string>

const int STAGE_NUM = 3;
//更新処理
void EnemyManager::Update(float elapsedTime)
{
    //更新処理
    for (Enemy* enemy : enemies)
    {

        //近いなら
        if(IsNear(enemy->GetPosition().z, target.z, length))
            enemy->Update(elapsedTime);

        ////ある程度の後ろなら
        //if (IsBehind(enemy->GetPosition().z, target.z, length))
        //    enemy->Destroy();
        //
    }

    //破棄処理
    // ※enemyの範囲for文中でerase()すると不具合が発生してしまうため、
    //   更新処理が終わった後に破棄リストに詰まれたオブジェクトを削除する。
    for (Enemy* enemy : removes)
    {
        //std::vectorから要素を削除する場合はイテレーターで削除しなければならない(順番などが変動するので)
        // そのために探して消そうとしている。
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);

        if (it != enemies.end())//最後の何もないものじゃないなら
        {
            enemies.erase(it);  //弾リストの物を消す
        }

        //敵の破棄処理
        delete enemy;
    }
    //// 破棄リストをクリア
    removes.clear();

    
    //敵同士の衝突処理
    //CollisionEnemyVsEnemies();

    //敵と壁の更新処理
    CollisionEnemyVsWall();

    
}




// ModelRender 処理
void EnemyManager::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    for (Enemy* enemy : enemies)
    {

        {
            //近いなら
            if (IsNear(enemy->GetPosition().z, target.z, length))
            enemy->ModelRender(rc, modelShader);

        }
        //std::string debugOutput = "Rendered enemies: " + std::to_string(debugCounter) + "\n";
        //OutputDebugStringA(debugOutput.c_str());
    }
}

//デバッグプリミティブ描画
void EnemyManager::DrawDebugPrimitive()
{
    //for (Enemy* enemy : enemies)
    //{
    //    enemy->DrawDebugPrimitive();
    //}

}

//エネミー登録
void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
    //std::string debugOutput = "Registered enemy, total enemies: " + std::to_string(enemies.size()) + "\n";
    //OutputDebugStringA(debugOutput.c_str());
}

//エネミー全削除
void EnemyManager::Clear()
{
    for (Enemy* enemy : enemies)
    {
        delete enemy;
    }
    enemies.clear();
}


//エネミー設置
void EnemyManager::SetEnemy(int stageNum)
{
#if EDIT
    int un = 0;//ウニ
    int cn = 0;//カニ
    int sn = 0;//ホタテ
#endif
    //if (stageNum < 0 || stageNum >= STAGE_NUM) // 範囲チェックを追加
    //{
    //    // 無効な stageNum の場合、エラーメッセージを出力し、関数を抜ける
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


//エネミー同士の衝突処理
void EnemyManager::CollisionEnemyVsEnemies()
{
    //すべての敵と総当たりで衝突処理
    for (int i = 0; i < enemies.size(); i++)
    {
        Enemy* enemyA = GetEnemy(i);

        for (int j = i + 1; j < enemies.size(); j++)
        {
            Enemy* enemyB = GetEnemy(j);

            //当たり判定なし
            if (!enemyB->GetHitPositionCollision())continue;

            //衝突処理
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
                //押し出し後の位置設定
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
                //押し出し後の位置設定
                enemyB->SetPosition(outPosition);
            }
            */
        }
    }
}

//エネミーと壁の当たり判定
void EnemyManager::CollisionEnemyVsWall()
{
    WallManager& wallManager = WallManager::Instance();

    
    //すべての敵と総当たりで衝突処理
    for (int i = 0; i < enemies.size(); i++)
    {
        //複数の当たり判定と判定を取らないため
        bool hit = false;

        Enemy* enemy = GetEnemy(i);

        //すべての壁と総当たりで衝突処理
        int wallCount = wallManager.GetWallCount();
        for (int i = 0; i < wallCount; i++)
        {
            Wall* wall = wallManager.GetWall(i);

            //当たり判定なし
            if (!wall->GetHitPositionCollision())continue;

            //衝突処理
            for (int j = 0; j < wall->GetHitPositionNum(); j++) {   //持っている当たり判定の位置全部と判定。

                DirectX::XMFLOAT3 outPosition;
                if (Collision::InterSectCylinderVsCylinder(     //プレイヤーを動かす
                    wall->GetHitPosition(j),
                    wall->GetHitPositionRadius(),
                    wall->GetHitPositionHeight(),
                    enemy->GetPosition(),
                    enemy->GetRadius(),
                    enemy->GetHeight(),
                    outPosition))
                {
                    //発射されたウニが普通の壁に一つも当たっていないなら
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

//エネミー削除
void EnemyManager::Remove(Enemy* enemy)
{
    for (Enemy* remove : removes)
    {
        if (remove == enemy)return;
    }

    ////破棄リストに追加
    removes.emplace_back(enemy);
}
