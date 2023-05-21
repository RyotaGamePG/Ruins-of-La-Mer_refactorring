#include "Graphics/Graphics.h"
#include "SceneSelect.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"

//初期化
void SceneSelect::Initialize()
{
    //スプライト初期化
    //sprite = new Sprite("Data/Sprite/Title.png");
}

//終了化
void SceneSelect::Finalize()
{
    //スプライト終了化
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }
}

//更新処理
void SceneSelect::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //なにかボタンを押したらゲームシーンへ切り替え
    const GamePadButton anyButton =
        GamePad::BTN_A
        | GamePad::BTN_B
        | GamePad::BTN_X
        | GamePad::BTN_Y;
    if (gamePad.GetButtonDown() & anyButton)
    {
        SceneGame* game = new SceneGame;
        game->SetStageNum(0);
        SceneManager::Instance().ChangeScene(new SceneLoading(game));
    }
}

//描画処理
void SceneSelect::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };    //RGBA(0.0~1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    //2Dスプライト描画
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());
        //タイトルスプライト描画
        //sprite->Render(dc,
        //    0, 0, screenWidth, screenHeight,
        //    0, 0, textureWidth, textureHeight,
        //    0,
        //    1, 1, 0, 1);
    }
}