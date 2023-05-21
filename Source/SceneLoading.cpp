//#include <thread>
//#include "Graphics/Graphics.h"
//#include "Camera.h"
//#include "Input/Input.h"
//#include "SceneLoading.h"
//#include "SceneManager.h"
//
////初期化
//void SceneLoading::Initialize()
//{
//    //スプライト初期化
//    //sprite = new Sprite("Data/Sprite/LoadingIcon.png");
//    //sprite = new Sprite("Data/Sprite/clearIcon_1.png");
//
//
//
//    loadingText = new SelectSignBoard(6);
//    loadingText->SetPosition(DirectX::XMFLOAT3(0.0f, -10.0f, 8.0f));
//
//
//    // カメラコントローラー初期化
//    cameraController = new CameraController();
//
//    //カメラの初期設定
//    Graphics& graphics = Graphics::Instance();
//    Camera& camera = Camera::Instance();
//    camera.SetLookAt(
//        DirectX::XMFLOAT3(0, 0, -10),
//        DirectX::XMFLOAT3(0, 0, 0),
//        DirectX::XMFLOAT3(0, 1, 1)
//    );
//    camera.SetPerspectiveFov(
//        DirectX::XMConvertToRadians(45),
//        graphics.GetScreenWidth() / graphics.GetScreenHeight(),
//        0.1f,
//        1000.0f
//    );
//
//
//    //スレッド開始
//    std::thread thread(LoadingThread, this);
//
//    //スレッドの管理を放棄
//    thread.detach();
//}
//
////終了化
//void SceneLoading::Finalize()
//{
//    //スプライト終了化
//    /*
//    if (sprite != nullptr)
//    {
//        delete sprite;
//        sprite = nullptr;
//    }
//    */
//
//    //カメラコントローラー終了化
//    if (cameraController != nullptr)
//    {
//        delete cameraController;
//        cameraController = nullptr;
//    }
//
//
//}
//
////更新処理
//void SceneLoading::Update(float elapsedTime)
//{
//
//   
//    // 次のシーンの準備が完了したらシーンを切り替える
//    if (nextScene->IsReady())
//    {
//        SceneManager::Instance().ChangeScene(nextScene);
//    }
//    else
//    {
//        loadingText->Update(elapsedTime);
//
//        //カメラコントローラー更新処理
//        DirectX::XMFLOAT3 target = { 0,0,20 };
//        //target.y = 3.0f;
//        //target.z += 4.0f;
//        cameraController->SetTarget(target);
//        cameraController->SelectUpdate(elapsedTime);
//
//    }
//}
//
//void SceneLoading::Render()
//{
//    Graphics& graphics = Graphics::Instance();
//    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
//    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
//    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
//
//    //画面クリア＆レンダーターゲット設定
//    FLOAT color[] = { 0.0f,0.0f,0.0f,1.0f };    //RGBA(0.0~1.0)
//    dc->ClearRenderTargetView(rtv, color);
//    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//    dc->OMSetRenderTargets(1, &rtv, dsv);
//
//    // 描画処理
//    RenderContext rc;
//    //rc.lightDirection = { 0.0f, 0.0f, -1.0f, 0.0f };	// ライト方向（下方向）
//
//
//    //カメラパラメータ設定
//    Camera& camera = Camera::Instance();
//    rc.view = camera.GetView();
//    rc.projection = camera.GetProjection();
//
//    //3Dスプライト描画
//    {
//      /*  Shader* shader = graphics.GetShader();
//        shader->Begin(dc, rc);
//
//        loadingText->Render(dc, shader);
//
//        shader->End(dc);*/
//    }
//}
//
//// ローディングスレッド
//void SceneLoading::LoadingThread(SceneLoading* scene)
//{
//    // COM関連の初期化、スレッド毎に呼ぶ必要がある
//    CoInitialize(nullptr);
//
//    // 次のシーンの初期化を行う
//    scene->nextScene->Initialize();
//
//    // スレッドが終わる前にCOM関連の終了化
//    CoUninitialize();
//
//    // 次のシーンの準備完了設定
//    scene->nextScene->SetReady();
//}


#include<thread>
#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include "SceneLoading.h"
#include "SceneGame.h"
#include "SceneTitle.h"
#include "SceneManager.h"


//初期化
void SceneLoading::Initialize()
{
    //スプライト初期化
    //テクスチャを読み込む
    texture = std::make_unique<Texture>("Data/Texture/LoadingIcon.png");
    sprite = std::make_unique<Sprite>();
    sprite->SetShaderResourceView(texture->GetShaderResourceView(), texture->GetWidth(), texture->GetHeight());

    //スレッド開始
    std::thread thread(LoadingThread, this);

    //スレッドの管理を放棄
    thread.detach();
}

//終了化
void SceneLoading::Finalize()
{
}

//更新処理
void SceneLoading::Update(float elapsedTime)
{
    constexpr float speed = 180.0f;
    angle += speed * elapsedTime;

    float screenW = static_cast<float>(Graphics::Instance().GetScreenWidth());
    float screenH = static_cast<float>(Graphics::Instance().GetScreenHeight());
    float textureW = static_cast<float>(sprite->GetTextureWidth());
    float textureH = static_cast<float>(sprite->GetTextureHeight());
    float positionX = screenW - textureW;
    float positionY = screenH - textureH;

    sprite->Update(
        positionX, positionY,																							//描画場所
        textureW, textureH,		//大きさ
        0, 0,																							//テクスチャの描画箇所
        static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()),				//テクスチャの幅高さ
        angle,																								//アングル
        1, 1, 1, 1);																					//RGBA



    //次のシーンの準備が完了したらシーンを切り替える
    if (nextScene->IsReady())
    {
        SceneManager::Instance().ChangeScene(nextScene);
        nextScene = nullptr;
    }
}

//描画処理
void SceneLoading::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f,1.0f,0.5f,1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    SpriteShader* shader = graphics.GetShader(SpriteShaderId::Default);
    RenderContext rc;
    rc.deviceContext = dc;

    // 2Dスプライト描画
    {
        //SpriteShader* shader = graphics.GetShader(SpriteShaderId::Default);
        //SpriteShader* shader = graphics.GetShader(SpriteShaderId::UVScroll);
        //SpriteShader* shader = graphics.GetShader(SpriteShaderId::Mask);

        shader->Begin(rc);

        shader->Draw(rc, sprite.get());

        shader->End(rc);
    }
}

//ローディングスレッド
void SceneLoading::LoadingThread(SceneLoading* scene)
{
    //COM関連の初期化でスレッド毎に呼ぶ必要がある
    CoInitialize(nullptr);

    //次のシーンの初期化を行う
    scene->nextScene->Initialize();

    //スレッドが終わる前にCOM関連の終了化
    CoUninitialize();

    //次のシーンの準備完了設定
    scene->nextScene->SetReady();
}