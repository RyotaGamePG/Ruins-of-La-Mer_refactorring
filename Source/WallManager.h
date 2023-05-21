#pragma once

#pragma once

#include <vector>
#include "Wall.h"

//エネミーマネージャー
class WallManager
{
private:
    WallManager() {}
    ~WallManager() {}

public:
    //唯一のインスタンス取得
    static WallManager& Instance() //シングルトン
    {
        static WallManager instance;
        return instance;
    }

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    //void Render(ID3D11DeviceContext* dc, Shader* shader);
    void ModelRender(RenderContext rc, ModelShader* modelShader);

    //デバッグプリミティブ描画
    void WallManager::DrawDebugPrimitive();

    void SetCollisionFlag();

    //壁設置
    void SetWall(int stageNum);


    //エネミー登録
    void Register(Wall* enemy);

    //エネミー全削除
    void Clear();

    //エネミー数取得
    int GetWallCount() const { return static_cast<int>(walls.size()); }

    //エネミー取得
    Wall* GetWall(int index) { return walls.at(index); };

    //エネミー削除
    void Remove(Wall* enemy);

private:
    std::vector<Wall*>  walls;   //複数のエネミーのポインタを std::vectorで管理
    std::vector<Wall*>  removes;
};

