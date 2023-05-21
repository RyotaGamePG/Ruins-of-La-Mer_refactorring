#pragma once

#include "Scene.h"

#include "Graphics/Sprite.h"


//シーンマネージャー
class SceneManager
{
private:
    SceneManager();
    ~SceneManager();

public:
    //唯一のインスタンス取得
    static SceneManager& Instance()
    {
        static SceneManager instance;
        return instance;
    }

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render();

    //シーンクリア
    void Clear();       //管理しているシーンの終了処理を行う

    //シーン切り替え
    void ChangeScene(Scene* scene);

private:
    Scene* currentScene = nullptr;

    //-----フェードインアウトに使用
    Sprite* sprite = nullptr;
    DirectX::XMFLOAT4 color = {};
};
