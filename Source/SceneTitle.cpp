#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneSelect.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "Camera.h"
#include "easing.h"
#include "StageManager.h"

//初期化
void SceneTitle::Initialize()
{
    //スプライト初期化
    //sprite = new Sprite("Data/Sprite/Title.png");
    //sprite = new Sprite("Data/Sprite/title0.png");

    //texture = std::make_unique<Texture>("Data/Sprite/Title.png");

    sprite_render = new SpriteOld("Data/Sprite/Title.png");



    //テクスチャ

    //背景の初期化
    backGround= new SelectSignBoard(0);
    backGround->SetPosition(DirectX::XMFLOAT3(0, 0, 0));


    //セレクトの初期化
    for (int i = 0; i < 4; i++) {
        SelectSignBoard* stage = new SelectSignBoard(i+2);
        //stage[i]->SetPosition(DirectX::XMFLOAT3(5-i*4, -2, -10 ));
        stage->SetPosition(DirectX::XMFLOAT3(i, -2, 0 ));
        stage->AddPos(DirectX::XMFLOAT3(-40, 0, 0));
        StageManager::Instance().Register(stage);
    }

    //タイトルの初期化
    titleStage0 = new SelectSignBoard(1);
    titleStage0->SetPosition(DirectX::XMFLOAT3(0, -5, 0));


    //音楽のロードと再生
    music = Audio::Instance().LoadAudioSource("Data/Audio/BGM/Title.wav",true);
    music->SetVolume(0.3f);
    music->Play();

    select = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/kasol.wav",false);
    decision = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/kettei.wav",false);


    // カメラコントローラー初期化
    cameraController = new CameraController();

    //カメラの初期設定
    Graphics& graphics = Graphics::Instance();
    Camera& camera = Camera::Instance();
    camera.SetLookAt(
        DirectX::XMFLOAT3(0, 0, -20),
        DirectX::XMFLOAT3(0, 0, 0),
        DirectX::XMFLOAT3(0, 1, 1)
    );
    camera.SetPerspectiveFov(
        DirectX::XMConvertToRadians(45),
        graphics.GetScreenWidth() / graphics.GetScreenHeight(),
        0.1f,
        1000.0f
    );


    // 平行光源を追加
    {
        mainDirectionalLight = new Light(LightType::Directional);
        mainDirectionalLight->SetDirection({ -1.410, -0.246f, 0.878f });
        mainDirectionalLight->SetColor(DirectX::XMFLOAT4(1.00, 0.915, 0.915, 1.0f));
        LightManager::Instane().Register(mainDirectionalLight);
    }


    //タイトル描画用のテクスチャ読み込み
    
    
  /*  sprite = std::make_unique<Sprite>();
    sprite->SetShaderResourceView(TitleTex->GetShaderResourceView(), TitleTex->GetWidth(), TitleTex->GetHeight());
    sprite->Update(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
        0, 0, static_cast<float>(TitleTex->GetWidth()), static_cast<float>(TitleTex->GetHeight()),
        0,
        1, 1, 1, 1);*/

    //決定ボタン用タイマー
    soundTime = 30.0f;

}

//終了化
void SceneTitle::Finalize()
{

    //カメラコントローラー終了化
    if (cameraController != nullptr)
    {
        delete cameraController;
        cameraController = nullptr;
    }

    //タイトルステージ終了化
    if (titleStage0 != nullptr)
    {
        delete titleStage0;
        titleStage0 = nullptr;
    }

    StageManager::Instance().Clear();

    //背景終了化
    if (backGround != nullptr)
    {
        delete backGround;
        backGround = nullptr;
    }

    //スプライト終了化
    //if (sprite != nullptr)
    //{
    //    delete sprite;
    //    sprite = nullptr;
    //}
}

//更新処理
void SceneTitle::Update(float elapsedTime)
{
    backGround->Update(elapsedTime);

    switch (state)
    {
    case State::Title:
        TitleUpdate(elapsedTime);
        break;
    case State::Select:
        SelectUpdate(elapsedTime);
        break;
    }
}

//セレクトに移行
void SceneTitle::SetSelectState()
{
    state = State::Title;
    moveFlag = true;
    time = 59.0f;
}

//タイトル更新処理
void SceneTitle::TitleUpdate(float elapsedTime)
{

    GamePad& gamePad = Input::Instance().GetGamePad();

    //なにかボタンを押したらタイトルシーンへ切り替え
    const GamePadButton anyButton =
        GamePad::BTN_A
        | GamePad::BTN_B
        | GamePad::BTN_X
        | GamePad::BTN_Y;
    if (gamePad.GetButtonDown() & anyButton)
    {
        moveFlag = true;

        /*
        state = State::Select;
        StageManager::Instance().AddPos(DirectX::XMFLOAT3(-10,0,0));
        */

        /*
        //短縮
        SceneGame* game = new SceneGame;
        game->SetStageNum(0);
        SceneManager::Instance().ChangeScene(new SceneLoading(game));
        */
    }


    //セレクトに移行
    if (moveFlag)
    {
        if (time >= 60)
        {
            //セレクトに移行
            state = State::Select;
            moveFlag = false;
            time = 0;

        }
        else
        {
            //移動
            //float pos = Back::easeInOut(time, 0.0f, 30.0f, 60);//速さ
            float pos = Back::easeInOut(time, -5.0f, 30.0f, 60);//速さ
            titleStage0->SetPosition(DirectX::XMFLOAT3(0, pos, 0));
            //StageManager::Instance().AddPos(DirectX::XMFLOAT3(pos+10.0f, 0, 0));
            float pos1 = Back::easeInOut(time, -45.0f, 60.0f, 60);//速さ
            StageManager::Instance().GetStage(0)->SetPosition(DirectX::XMFLOAT3(pos1 , 0, 0));
            StageManager::Instance().GetStage(1)->SetPosition(DirectX::XMFLOAT3(pos1 - 15.0f, 0, 0));
            StageManager::Instance().GetStage(2)->SetPosition(DirectX::XMFLOAT3(pos1 - 30.0f, 0, 0));
            float pos2 = Back::easeInOut(time, -45.0f, 60.0f, 60);//速さ
            StageManager::Instance().GetStage(3)->SetPosition(DirectX::XMFLOAT3(pos2, 10, 0));
            time++;
        }
    }


    titleStage0->Update(elapsedTime);
    StageManager::Instance().Update(elapsedTime);

    //カメラコントローラー更新処理
    DirectX::XMFLOAT3 target = { 0,0,20 };
    //target.y = 3.0f;
    //target.z += 4.0f;
    cameraController->SetTarget(target);	
    cameraController->SelectUpdate(elapsedTime);

}

//セレクト更新処理
void SceneTitle::SelectUpdate(float elapsedTime)
{
    //タイトルに移行
    if (moveFlag)
    {
        if (time >= 60)
        {
            state = State::Title;
            moveFlag = false;
            time = 0;
        }
        else
        {
            //移動
            //float pos = Back::easeInOut(time, 30.0f, -30.0f, 60);//速さ
            float pos = Back::easeInOut(time, 30.0f, -35.0f, 60);//速さ
            titleStage0->SetPosition(DirectX::XMFLOAT3(0, pos, 0));
            float pos1 = Back::easeInOut(time, 15.0f, -60.0f, 60);//速さ
            StageManager::Instance().GetStage(0)->SetPosition(DirectX::XMFLOAT3(pos1, 0, 0));
            StageManager::Instance().GetStage(1)->SetPosition(DirectX::XMFLOAT3(pos1 - 15.0f, 0, 0));
            StageManager::Instance().GetStage(2)->SetPosition(DirectX::XMFLOAT3(pos1 - 30.0f, 0, 0));
            float pos2 = Back::easeInOut(time, 15.0f, -60.0f, 60);//速さ
            StageManager::Instance().GetStage(3)->SetPosition(DirectX::XMFLOAT3(pos2, 10, 0));
            time++;
        }


    }
    //最初にスケールアニメーション
    if (!StageManager::Instance().GetStage(selectNum)->GetMove())
        StageManager::Instance().GetStage(selectNum)->SetMove(true);

    GamePad& gamePad = Input::Instance().GetGamePad();

    //ゲームシーンじゃないなら更新
    if (!sceneGameFlag) {
        //選択入力処理
        if (selectNum < 2) {
            if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT)
            {
                //前を動かさない
                StageManager::Instance().GetStage(selectNum)->SetMove(false);
                selectNum++;
                //動かす
                StageManager::Instance().GetStage(selectNum)->SetMove(true);

                //効果音
                select = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/kasol.wav", false);
                select->Play();
            }
        }

        //選択入力処理
        if (selectNum > 0) {
            if (gamePad.GetButtonDown() & GamePad::BTN_LEFT)
            {
                //前を動かさない
                StageManager::Instance().GetStage(selectNum)->SetMove(false);
                selectNum--;
                //動かす
                StageManager::Instance().GetStage(selectNum)->SetMove(true);

                //効果音
                select = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/kasol.wav", false);
                select->Play();
            }
        }



        //カメラコントローラー更新処理
        DirectX::XMFLOAT3 target = { 0,0,20 };
        cameraController->SetTarget(target);	//プレイヤーの腰あたりをターゲットに設定
        cameraController->SelectUpdate(elapsedTime);

        //看板更新処理
        titleStage0->Update(elapsedTime);
        StageManager::Instance().Update(elapsedTime);
    }

    if (!moveFlag)
    {
        //ゲームシーンへのフラグがある
        if (sceneGameFlag)
        {
            if (soundTime <= 0)
            {
                //シーンゲームへ遷移
                SceneGame* game = new SceneGame;
                game->SetStageNum(selectNum);
                SceneManager::Instance().ChangeScene(new SceneLoading(game));
            }
            soundTime--;
        }
        else
        {
            //なにかボタンを押したらゲームシーンへ切り替え
            const GamePadButton anyButton =
                GamePad::BTN_A
                | GamePad::BTN_B;
            if (gamePad.GetButtonDown() & anyButton)
            {
                switch (gamePad.GetButtonDown() & anyButton)
                {

                    //SceneManager::Instance().ChangeScene(new SceneSelect);

                case  GamePad::BTN_A:   //Z
                    //ゲームシーンへのフラグ
                    sceneGameFlag = true;
                    //決定音
                    decision = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/kettei.wav", false);
                    decision->Play();
                    break;

                case  GamePad::BTN_B:

                    moveFlag = true;
                    break;

                }
            }
        }
    }

}



//描画処理
void SceneTitle::Render()
{
    /*
    switch (state)
    {
    case State::Title:
        TitleRender();
        break;
    case State::Select:
        SelectRender();
        break;
    }
    */

    
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f,0.0f,0.0f,1.0f };    //RGBA(0.0~1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // 描画処理
    RenderContext rc;
   // rc.lightDirection = { 0.0f, 0.0f, -1.0f, 0.0f };	// ライト方向（下方向）
    rc.deviceContext = dc;
    //カメラパラメータ設定
    Camera& camera = Camera::Instance();
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();



    //2Dスプライト描画
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite_render->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite_render->GetTextureHeight());
        //タイトルスプライト描画
        sprite_render->Render(dc,
            0, 0, screenWidth, screenHeight,
            0, 0, textureWidth, textureHeight,
            0,
            1, 1, 1, 1);
    }


    // 3Dモデル描画
    {
        ModelShader* shader = graphics.GetShader(ModelShaderId::Default);
        shader->Begin(rc);
        
        //backGround->Render(dc, shader);
        //StageManager::Instance().Render(dc, shader);
        StageManager::Instance().ModelRender(rc, shader);
        titleStage0->ModelRender(rc, shader);

        shader->End(rc);

        
    }

    float lookZ= camera.GetEye().z;

#if EDIT
    if (ImGui::Begin("camera", nullptr, ImGuiWindowFlags_None))
    {
        //トランスフォーム
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //位置
            ImGui::InputFloat("PositionZ", &lookZ);
            ImGui::InputFloat("time", &time);

            //看板の位置
            float tPos = titleStage0->GetPosition().y;
            ImGui::InputFloat("titleY", &tPos);
            float s0 = StageManager::Instance().GetStage(0)->GetPosition().x;
            float s1 = StageManager::Instance().GetStage(1)->GetPosition().x;
            float s2 = StageManager::Instance().GetStage(2)->GetPosition().x;
            ImGui::InputFloat("select0X", &s0);
            ImGui::InputFloat("select1X", &s1);
            ImGui::InputFloat("select2X", &s2);
            /*
            ImGui::InputFloat3("title", &titleStage0.z);
            ImGui::InputFloat3("title", &titleStage0.z);
            */

        }
    }
    ImGui::End();

#endif

    //変えられているなら

    if (camera.GetEye().z != lookZ&&lookZ!=0)
    {
        camera.SetLookAt(
            DirectX::XMFLOAT3(0, 0, lookZ),
            DirectX::XMFLOAT3(0, 0, 0),
            DirectX::XMFLOAT3(0, 1, 1)
        );
    }

}

//タイトル描画処理
void SceneTitle::TitleRender()
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

    // 描画処理
    RenderContext rc;
    //rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

    //カメラパラメータ設定
    Camera& camera = Camera::Instance();
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();

    // 3Dモデル描画
    {
        ModelShader* shader = graphics.GetShader(ModelShaderId::Default);
        shader->Begin(rc);

        titleStage0->ModelRender(rc, shader);


        shader->End(rc);
    }
}

//セレクト描画処理
void SceneTitle::SelectRender()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 1.0f,0.0f,0.5f,1.0f };    //RGBA(0.0~1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // 描画処理
    RenderContext rc;
    rc.deviceContext = dc;
    //rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

    LightManager::Instane().PushRenderContext(rc);

    //カメラパラメータ設定
    Camera& camera = Camera::Instance();
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();

    // スカイボックスの描画
    {
        //SpriteShader* shader = graphics.GetShader(SpriteShaderId::Default);
        //shader->Begin(rc);

        //shader->Draw(rc, sprite.get());

        //shader->End(rc);
    }
    // 3Dモデル描画
    {
        //Shader* shader = graphics.GetShader();
        //shader->Begin(dc, rc);

        //StageManager::Instance().Render(dc, shader);

        //shader->End(dc);

        ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
        shader->Begin(rc);

        StageManager::Instance().ModelRender(rc, shader);

        shader->End(rc);
        
    }


}