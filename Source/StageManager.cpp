#include "StageManager.h"
#include "StageData.h"
#include "Collision.h"

#include <Windows.h>
#include <sstream>


//更新処理
void StageManager::Update(float elapsedTime)
{
    //更新処理
    for (StageObj* stage : stages)
    {
        //ステージ背景 近いなら
        if (stage->type == StageObj::Type::Stage)
        {
            if (IsNear(stage->GetPosition().z, target.z, length))
                stage->Update(elapsedTime);
        }
        else
            stage->Update(elapsedTime);
    }

    //破棄処理
     //※enemyの範囲for文中でerase()すると不具合が発生してしまうため、
       //更新処理が終わった後に破棄リストに詰まれたオブジェクトを削除する。
    //for (StageObj* stage : removes)
    //{
    //    //std::vectorから要素を削除する場合はイテレーターで削除しなければならない(順番などが変動するので)
    //    // そのために探して消そうとしている。
    //    std::vector<StageObj*>::iterator it = std::find(stages.begin(), stages.end(), stage);

    //    if (it != stages.end())//最後の何もないものじゃないなら
    //    {
    //        stages.erase(it);  //弾リストの物を消す
    //    }

    //    //ステージの破棄処理
    //    delete stage;
    //}
    //// 破棄リストをクリア
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
    for (StageObj* stage : stages) // stageVecからstagesに変更
    {
        stage->ModelRender(rc, modelShader);
    }
}



//ステージ設置
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
    // stageNumが範囲外の場合は、関数を抜ける
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



//描画処理
//void StageManager::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    for (StageObj* stage : stages)
//    {
//        //ステージ背景　近いなら
//        if (stage->type == StageObj::Type::Stage) { //背景
//            if (IsNear(stage->GetPosition().z, target.z, length))
//            {
//                stage->Render(dc, shader);
//            }
//        }
//        else
//            stage->Render(dc, shader);
//    }
//}

//全体位置加算
void StageManager::AddPos(DirectX::XMFLOAT3 pos)
{
    for (StageObj* stage : stages)
    {
        stage->AddPos(pos);
    }

}

//全体位置設定
void StageManager::SetPosition(DirectX::XMFLOAT3 pos)
{
    for (StageObj* stage : stages)
    {
        stage->SetPosition(pos);
    }

}


//デバッグプリミティブ描画
void StageManager::DrawDebugPrimitive()
{
    //for (StageObj* stage : stages)
    //{
    //    stage->DrawDebugPrimitive();
    //}
}

//エネミー登録
void StageManager::Register(StageObj* stage)
{
    stages.emplace_back(stage);
}

//エネミー全削除
void StageManager::Clear()
{
    for (StageObj* stage : stages)
    {
        delete stage;
    }
    stages.clear();

}

//エネミー削除
void StageManager::Remove(StageObj* stage)
{
    //破棄リストに追加
    removes.emplace_back(stage);
}