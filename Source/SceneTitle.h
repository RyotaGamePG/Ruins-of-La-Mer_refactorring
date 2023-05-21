#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Stage.h"
#include "CameraController.h"
#include "Audio/Audio.h"
#include "Graphics/Texture.h"

#include "Graphics/Light.h"
#include "Graphics/LightManager.h"

//タイトルシーン(セレクトシーンもごっちゃ)
class SceneTitle :public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle()override {}

    //初期化
    void Initialize() override;

    //終了化
    void Finalize() override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render()override;



    //タイトル更新処理
    void TitleUpdate(float elapsedTime);
    //タイトル描画処理
    void TitleRender();

    //セレクト更新処置
    void SelectUpdate(float elapsedTime);
    //セレクト描画処理
    void SelectRender();



    //----------
    //  切り替え時に使うもの
    //----------
    void ResetTime() { time = 0; }

    void SetChangeScene() { moveFlag = true; }

    //ゲームからセレクトに移行するのに使う
    void SetSelectState();

private:

    //----------
    //  移動アニメーション
    //----------
    struct movePos
    {
        float orgP;     //元の位置
        float nextP;    //次の位置
    };
    movePos moveStagePos[5];
    movePos moveTitleStagePos;


    float totalMoveTime = 60.0f;   //合計移動時間
    float moveLength = 10.0f;      //移動距離

private:
    //Sprite* sprite = nullptr;

    //アップデート2023年5月15日
    std::unique_ptr<Texture> texture;

    //std::unique_ptr<Texture> TitleTex;

    CameraController* cameraController = nullptr;

    //タイトル文字
    SelectSignBoard* titleStage0 = nullptr;

    SelectSignBoard* backGround = nullptr;

    int selectNum = 0;  //ステージ番号


private:

    enum State
    {
        Title,
        Select
    };

    State state = State::Title;

    float       time = 0;
    float       soundTime = 30.0f;  //決定ボタンを鳴らすためのもの

    bool        moveFlag = false;   //セレクトとタイトルに変わるためのフラグ
    bool        sceneGameFlag = false; //ゲーム遷移フラグ

private:
    //音楽
    std::unique_ptr<AudioSource> music = nullptr;

    std::unique_ptr<AudioSource> select = nullptr;
    std::unique_ptr<AudioSource> decision = nullptr;

    Light* mainDirectionalLight = nullptr;
    std::unique_ptr<Sprite>	sprite;




};