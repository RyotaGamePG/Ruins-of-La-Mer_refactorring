//#pragma once
//
//#include "Graphics/Sprite.h"
//#include "Scene.h"
//#include "Stage.h"
//#include "CameraController.h"
//
////ローディングシーン
//class SceneLoading :public Scene
//{
//public:
//    SceneLoading(Scene* nextScene) :nextScene(nextScene) {}
//    ~SceneLoading()override {}
//
//    // 初期化
//    void Initialize()override;
//
//    // 終了化
//    void Finalize()override;
//
//    // 更新処理
//    void Update(float elapsedTime)override;
//
//    // 描画処理
//    void Render()override;
//
//    CameraController* cameraController = nullptr;
//
//    SelectSignBoard* loadingText = nullptr;
//
//private:
//    // ローディングスレッド
//    static void LoadingThread(SceneLoading* scene);
//
//private:
//    Sprite* sprite = nullptr;
//    Scene* nextScene = nullptr;
//};


#pragma once

#include"Graphics/Sprite.h"
#include "Graphics/Texture.h"
#include "Scene.h"

//ローディングシーン
class SceneLoading :public Scene
{
public:
    SceneLoading(Scene* nextScene) : nextScene(nextScene) {}
    ~SceneLoading() override {}

    //初期化
    void Initialize()override;

    //終了化
    void Finalize()override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render()override;

private:
    //ローディングスレッド
    static void LoadingThread(SceneLoading* scene);

private:
    std::unique_ptr<Texture> texture;
    std::unique_ptr<Sprite>	sprite;

    float angle = 0.0f;
    Scene* nextScene = nullptr;

};