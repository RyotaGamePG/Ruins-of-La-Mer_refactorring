#pragma once

#include <vector>
#include "Stage.h"
#include "Player.h"
#include "EnemyBoss.h"

//ステージマネージャー
class StageManager
{
private:
    StageManager() {}
    ~StageManager() {}

public:
    //唯一のインスタンス取得
    static StageManager& Instance() //シングルトン
    {
        static StageManager instance;
        return instance;
    }

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void ModelRender(RenderContext rc, ModelShader* modelShader);

    //デバッグプリミティブ描画
    void StageManager::DrawDebugPrimitive();

    //全体位置加算処理
    void AddPos(DirectX::XMFLOAT3 pos);

    //全体位置決定
    void SetPosition(DirectX::XMFLOAT3 pos);


    //ステージ登録
    void Register(StageObj* flagArea);

    //ステージ全削除
    void Clear();

    //ステージ数取得
    int GetStageCount() const { return static_cast<int>(stages.size()); }

    //ステージ取得
    StageObj* GetStage(int index) { return stages.at(index); };

    //ステージ削除
    void Remove(StageObj* flagArea);

    //ステージ生成
    void SetStageObj(int stageNum);

    //プレイヤー設定
    void SetPlayer(Player* player) { this->player = player; }

    //プレイヤー位置ゲット
    DirectX::XMFLOAT3 GetBossPos() { return player->GetPosition(); }

    //ターゲット位置設定
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    //ターゲットと近いかを返す(マイナス関係なし)
    bool IsNear(float pos0, float pos1, float length)
    {
        float P = pos0 - pos1;
        if (P < 0)P = -P;
        return P < length;
    }

    float length = 30.0f;
    DirectX::XMFLOAT3   target = { 0,0,0 };

private:
    std::vector<StageObj*>  stages;   //複数のエネミーのポインタを std::vectorで管理
    std::vector<StageObj*>  removes;

    Player* player = nullptr;

    std::vector<Stage*> stageVec;
   // Model* model = nullptr;
};
