#pragma once

#include <vector>
#include "FlagArea.h"
#include "Player.h"
#include "EnemyBoss.h"

//エネミーマネージャー
class FlagAreaManager
{
private:
    FlagAreaManager() {}
    ~FlagAreaManager() {}

public:
    //唯一のインスタンス取得
    static FlagAreaManager& Instance() //シングルトン
    {
        static FlagAreaManager instance;
        return instance;
    }


    //更新処理
    void Update(float elapsedTime);

    //描画処理
    //void Render(ID3D11DeviceContext* dc, Shader* shader);
    void ModelRender(RenderContext rc, ModelShader* modelShader);

    //デバッグプリミティブ描画
    void FlagAreaManager::DrawDebugPrimitive();

    //フラグエリア設置
    void SetFlagArea(int stageNum);

    //エリア登録
    void Register(FlagArea* flagArea);

    //エリア全削除
    void Clear();

    //エリア数取得
    int GetEnemyCount() const { return static_cast<int>(areas.size()); }

    //エリア取得
    FlagArea* GetArea(int index) { return areas.at(index); };

    //エリア削除
    void Remove(FlagArea* flagArea);

    //ボス・プレイヤー設定
    void SetBoss(EnemyBoss* boss) { this->boss = boss; }
    void SetPlayer(Player* player) { this->player =player; }

    //ボス・プレイヤー位置ゲット
    DirectX::XMFLOAT3 GetBossPos() { return player->GetPosition(); }
    DirectX::XMFLOAT3 GetPlayerPos() { return boss->GetPosition(); }

    //ボススピードアップ
    void BossSpeedUp() { boss->SpeedUp(); }
private:
    std::vector<FlagArea*>  areas;   //複数のエネミーのポインタを std::vectorで管理
    std::vector<FlagArea*>  removes;

    EnemyBoss*  boss = nullptr;
    Player*     player = nullptr;


    float length = 6.0f;

};
