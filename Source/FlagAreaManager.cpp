#include "FlagAreaManager.h"
#include "FlagArea.h"
#include "Collision.h"

#include "SceneManager.h"
#include "SceneTitle.h"
const int STAGE_NUM = 3;


//更新処理
void FlagAreaManager::Update(float elapsedTime)
{
    //TODO::忘れずに

#if GAME
    //更新処理
    for (FlagArea* area : areas)
    {
        switch (area->getType()) {
        case FlagArea::Type::SpeedUp:
            if (area->OverlapArea(boss->GetPosition()))
            {
                boss->SpeedUp();
                Remove(area);   //消す
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
                    Remove(area);   //消す
                }
            }
            break;
        }
    }
#endif
    
    //破棄処理
    for (FlagArea* area : removes)
    {
        //std::vectorから要素を削除する場合はイテレーターで削除しなければならない(順番などが変動するので)
        // そのために探して消そうとしている。
        std::vector<FlagArea*>::iterator it = std::find(areas.begin(), areas.end(), area);

        if (it != areas.end())//最後の何もないものじゃないなら
        {
            areas.erase(it);  //弾リストの物を消す
        }

        //エリアの破棄処理
        delete area;
    }
    // 破棄リストをクリア
    removes.clear();


}

//描画処理
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

//デバッグプリミティブ描画
void FlagAreaManager::DrawDebugPrimitive()
{
    for (FlagArea* area : areas)
    {
        area->DrawDebugPrimitive();
    }

}


//フラグ設置
void FlagAreaManager::SetFlagArea(int stageNum)
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
        case StageObjType::flagSpeedUp:
        {
            //float x = stage->pos.x - length;
            float x = 0;
            float z = stage->pos.z + length;
            DirectX::XMFLOAT3 posRU = { x,0,z };	//右上
            //x = stage->pos.x + length;
            x = 10;
            z = stage->pos.z - length;
            DirectX::XMFLOAT3 posLD = { x,0,z };   //左下
            FlagArea* obj = new FlagArea(posRU,posLD,FlagArea::Type::SpeedUp);
            FlagAreaManager::Instance().Register(obj);
        }
        break;
        case StageObjType::flagGoal:
        {
            float x = 0;
            float z = stage->pos.z + length;
            DirectX::XMFLOAT3 posRU = { x,0,z };	//右上
            x = 10;
            z = stage->pos.z - length;
            DirectX::XMFLOAT3 posLD = { x,0,z };   //左下
            FlagArea* obj = new FlagArea(posRU, posLD, FlagArea::Type::Goal);
            FlagAreaManager::Instance().Register(obj);
        }
        break;
        }
    }
}

//エリア登録
void FlagAreaManager::Register(FlagArea* flagArea)
{
    areas.emplace_back(flagArea);
}

//エリア全削除
void FlagAreaManager::Clear()
{
    for (FlagArea* flagArea : areas)
    {
        delete flagArea;
    }
    areas.clear();

}

//エリア削除
void FlagAreaManager::Remove(FlagArea* flagArea)
{
    for (FlagArea* remove : removes)
    {
        if (remove == flagArea)return;
    }

    //破棄リストに追加
    removes.emplace_back(flagArea);
}