#pragma once

#include <vector>
#include "Enemy.h"
#include "Effect.h"

//エネミーマネージャー
class EnemyManager
{
private:
    EnemyManager() {}
    ~EnemyManager() {}

public:
    //唯一のインスタンス取得
    static EnemyManager& Instance() //シングルトン
    {
        
        static EnemyManager instance;

        return instance;
    }


    //更新処理
    void Update(float elapsedTime);

    //描画処理
    //void Render(ID3D11DeviceContext* dc, Shader* shader);
    //描画処理
    void ModelRender(RenderContext rc, ModelShader* modelShader);
    //デバッグプリミティブ描画
    void EnemyManager::DrawDebugPrimitive();

    //エネミー登録
    void Register(Enemy* enemy);

    //エネミー全削除
    void Clear();

    //エネミー数取得
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    //エネミー取得
    Enemy* GetEnemy(int index) { return enemies.at(index); };

    //エネミー削除
    void Remove(Enemy* enemy);



    //エネミー設置
    void SetEnemy(int stageNum);



    //軽量化用

    //ターゲット位置設定
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    //ターゲットと近いかを返す(マイナス関係なし)
    bool IsNear(float pos0, float pos1, float length)
    {
        float P = pos0 - pos1;
        if (P < 0)P = -P;
        return P < length;
    }

    bool IsBehind(float pos0, float pos1, float length)
    {
        float P = pos0 - pos1;
        return P < -length;
    }

    float length = 30.0f;
    DirectX::XMFLOAT3   target = { 0,0,0 };
    


private:
    //エネミー同士の衝突処理
    void CollisionEnemyVsEnemies();

    //エネミーと壁の当たり判定
    void CollisionEnemyVsWall();

private:
    std::vector<Enemy*>  enemies;   //複数のエネミーのポインタを std::vectorで管理
    std::vector<Enemy*>  removes;

private://エフェクト
    Effect* hitEffect = nullptr;
    Effect* moveEffect = nullptr;

};
