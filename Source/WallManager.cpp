#include "WallManager.h"
#include "Collision.h"
#include "StageData.h"

const int STAGE_NUM = 3;

//更新処理
void WallManager::Update(float elapsedTime)
{
    //更新処理
    for (Wall* wall : walls)
    {
        wall->Update(elapsedTime);
    }

    //破棄処理
    // ※enemyの範囲for文中でerase()すると不具合が発生してしまうため、
    //   更新処理が終わった後に破棄リストに詰まれたオブジェクトを削除する。
    for (Wall* wall : removes)
    {
        //std::vectorから要素を削除する場合はイテレーターで削除しなければならない(順番などが変動するので)
        // そのために探して消そうとしている。
        std::vector<Wall*>::iterator it = std::find(walls.begin(), walls.end(), wall);

        if (it != walls.end())//最後の何もないものじゃないなら
        {
            walls.erase(it);  //弾リストの物を消す
        }

        //敵の破棄処理
        delete wall;
    }
    // 破棄リストをクリア
    removes.clear();


    //敵同士の衝突処理
    //CollisionEnemyVsEnemies();


}

//フラグ設置
void WallManager::SetWall(int stageNum)
{
    //if (stageNum < 0 || stageNum >= STAGE_NUM) // 範囲チェックを追加
    //{
    //    // 無効な stageNum の場合、エラーメッセージを出力し、関数を抜ける
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
            //半分壁なら
            if (obj->GetPosition().y < 0)obj->SetInvincibleFlag(true);
            WallManager::Instance().Register(obj);
            
        }
        break;
        }
    }
}


//描画処理
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

//デバッグプリミティブ描画
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

//エネミー登録
void WallManager::Register(Wall* enemy)
{
    walls.emplace_back(enemy);
}

//エネミー全削除
void WallManager::Clear()
{
    for (Wall* wall : walls)
    {
        delete wall;
    }
    walls.clear();

}

//エネミー削除
void WallManager::Remove(Wall* wall)
{
    //破棄リストに追加
    removes.emplace_back(wall);
}