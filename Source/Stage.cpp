#include <imgui.h>

#include "Stage.h"
#include "Graphics/Graphics.h"
#include "easing.h"




//------------------------------------

//-----Stageクラス----------

//------------------------------------
//コンストラクタ
Stage::Stage()
{
    //ステージモデルを読み込み
    //model = new Model("Data/Model/Stage/back_ground.mdl");
    model = new Model("Data/Model/background.mdl");
    //model = new Model("Data/Model/select.mdl");
    scale.x = scale.y = scale.z = 0.01f;
    //scale.x = scale.y = scale.z = 0.1f;

    type = Type::Stage;
    
}

Stage::~Stage()
{
    //ステージモデルを破棄
    delete model;
}

//更新処理
void Stage::Update(float elapsedTime)
{
    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);
}

void Stage::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);
}

//void Stage::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    //シェーダーにモデルを描画してもらう
//    shader->Draw(dc, model);
//
//   
//}

//デバッグプリミティブ描画
void Stage::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    ////衝突判定用のデバッグ球を描画
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

}


//------------------------------------

//-----Signboardクラス----------

//------------------------------------

Signboard::Signboard(int index)
{
    switch (index)
    {
    case 0://ダッシュチュートリアル
        model = new Model("Data/Model/Text/tutorial01_dush.mdl");
        scale.x = scale.y = scale.z = 0.05f;
        angle.y = DirectX::XMConvertToRadians(90);
        break;
    case 1://ジャンプチュートリアル
        model = new Model("Data/Model/Text/tutorial02_Jump.mdl");
        scale.x = scale.y = scale.z = 0.05f;
        angle.y = DirectX::XMConvertToRadians(90);
        break;
    case 2://壁壊すチュートリアル
        model = new Model("Data/Model/Text/tutorial03_wallbreak.mdl");
        scale.x = scale.y = scale.z = 0.05f;
        angle.y = DirectX::XMConvertToRadians(90);
        break;
    case 3://ホタテチュートリアル
        model = new Model("Data/Model/Text/tutorial04_hotatejump.mdl");
        scale.x = scale.y = scale.z = 0.05f;
        angle.y = DirectX::XMConvertToRadians(90);
        break;
    case 4://次のステージ
        model = new Model("Data/Model/Stage/exit.mdl");
        scale.x = scale.y = scale.z = 0.02f;
        angle.y = DirectX::XMConvertToRadians(90);
        break;
    }


    angle.x = DirectX::XMConvertToRadians(-15);
   
    /*
    //ステージモデルを読み込み
    model = new Model("Data/Model/Stage/wall.mdl");
    //model = new Model("Data/Model/select.mdl");
    scale.x = scale.y = scale.z = 0.001f;
    //scale.x = scale.y = scale.z = 0.01f;
    */
    

    //モデル行列更新
    model->UpdateTransform(transform);

    type = Type::Signboard;
}

Signboard::~Signboard()
{
    delete model;
}

//更新処理
void Signboard::Update(float elapsedTime)
{
    //遊び
    
    time++;
    /*
    float scale0 = Bounce::easeIn(time/30, 0.0f, 1.0f, 100.0f);
    scale.x = scale0;
    scale.y = scale0;
    scale.z = scale0;
    */

    /*
    float scale0 = sinf(time / 60) * 0.001f + 0.002;
    //float scale0 = sinf(time / 60) * 0.01f + 0.02;
    scale.x = scale0;
    scale.y = scale0;
    scale.z = scale0;
    */

    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);

}

void Signboard::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);
}

//void Signboard::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    //シェーダーにモデルを描画してもらう
//    shader->Draw(dc, model);
//}

//デバッグプリミティブ描画
void Signboard::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    ////衝突判定用のデバッグ球を描画
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}

//-----セレクトのNクラス----------

SelectSignBoard::SelectSignBoard(int index)
{
    //ステージモデルを読み込み
    switch (index)
    {
    case 0://タイトル背景
        model = new Model("Data/Model/Select/title_background.mdl");
        scale.x = scale.y = scale.z = 0.03f;
        isBackGround = true;
        break;
    case 1: //タイトル文字
        model = new Model("Data/Model/Select/title.mdl");
        scale.x = scale.y = scale.z = 0.1f;
        break;
    case 2://ステージ１
        model = new Model("Data/Model/Select/icon1.mdl");
        scale.x = scale.y = scale.z = 0.1f;
        break;
    case 3://ステージ２
        model = new Model("Data/Model/Select/icon2.mdl");
        scale.x = scale.y = scale.z = 0.1f;
        break;

    case 4://ステージ3
        model = new Model("Data/Model/Select/icon3.mdl");
        scale.x = scale.y = scale.z = 0.1f;
        break;

    case 5: //セレクト文字
        model = new Model("Data/Model/Text/select.mdl");
        scale.x = scale.y = scale.z = 0.1f;
        break;

    case 6://ローディング文字
        model = new Model("Data/Model/Text/loading.mdl");
        scale.x = scale.y = scale.z = 0.08f;
        isBackGround = true;
        model->PlayAnimation(0, true);
        break;
    }

    type = Type::SelectSignboard;
}

SelectSignBoard::~SelectSignBoard()
{
    delete model;
}

void SelectSignBoard::Update(float elapsedTime)
{
    if (!isBackGround)
    {
        if (isMove)
        {
            ScaleMove();
        }
        else
        {
            //スケールリセット
            scale.x = 0.1;
            scale.y = 0.1;
            scale.z = 0.1;
            time = 0.0f;
        }
    }

    //オブジェクト行列を更新
    UpdateTransform();

    //モデルアニメーション更新処理
    model->UpdateAnimation(elapsedTime);

    //モデル行列更新
    model->UpdateTransform(transform);

    time++;
}

void SelectSignBoard::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);
}

//void SelectSignBoard::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    //シェーダーにモデルを描画してもらう
//    shader->Draw(dc, model);
//
//}

void SelectSignBoard::ScaleMove()
{
    float scale0 = sinf(time/20)*0.02f;
    //float scale0 = sinf(time / 60) * 0.01f + 0.02;
    scale.x = 0.1f+scale0;
    scale.y = 0.1f+scale0;
    scale.z = 0.1f+scale0;
}


//-----ゲームのセレクトクラス----------

GameSelectSignBoard::GameSelectSignBoard(int index)
{
    //ステージモデルを読み込み
    switch (index)
    {
    case 0://クリアの場合の文字
        model = new Model("Data/Model/Text/clear.mdl");
        scale.x = scale.y = scale.z = 0.015f;
        text = true;
        break;
    case 1://クリア選択肢
        model = new Model("Data/Model/Text/next.mdl");
        scale.x = scale.y = scale.z = 0.01f;
        break;
    case 2://クリア選択肢
        model = new Model("Data/Model/Text/select.mdl");
        scale.x = scale.y = scale.z = 0.01f;
        break;
    case 3: // ゲームオーバーの場合の文字
        model = new Model("Data/Model/Text/over.mdl");
        scale.x = scale.y = scale.z = 0.015f;
        text = true;
        break;
    case 4: // ゲームオーバー選択肢
        model = new Model("Data/Model/Text/retry.mdl");
        scale.x = scale.y = scale.z = 0.01f;
        break;
    case 5: //　ゲームオーバー選択肢
        model = new Model("Data/Model/Text/select.mdl");
        scale.x = scale.y = scale.z = 0.01f;
        break;

    }

    type = Type::GameSelectSignboard;

    angle.y = DirectX::XMConvertToRadians(90);
}

GameSelectSignBoard::~GameSelectSignBoard()
{
    delete model;
}

void GameSelectSignBoard::Update(float elapsedTime)
{
    //移動処理
    if (isMove)
    {
        NextPosMove();
    }
    //スケールアニメーション
    else if (isScaleMove)
    {
        ScaleMove();
    }
    //何もない場合
    else if(text)
    {
        //スケールリセット
        scale.x = 0.015f;
        scale.y = 0.015f;
        scale.z = 0.015f;
        time = 0.0f;
    }
     else
    {
    //スケールリセット
    scale.x = 0.01f;
    scale.y = 0.01f;
    scale.z = 0.01f;
    time = 0.0f;
    }

    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);

    time++;
}

void GameSelectSignBoard::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);
}

//void GameSelectSignBoard::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    //シェーダーにモデルを描画してもらう
//    shader->Draw(dc, model);
//
//}

void GameSelectSignBoard::ScaleMove()
{
    float scale0 = sinf(time / 20) * 0.002f;
    //float scale0 = sinf(time / 60) * 0.01f + 0.02;
    scale.x = 0.01f + scale0;
    scale.y = 0.01f + scale0;
    scale.z = 0.01f + scale0;
}

void GameSelectSignBoard::NextPosMove()
{
    DirectX::XMFLOAT3 old = oldPos;
    DirectX::XMFLOAT3 next = nextPos;

    float x = Sine::easeInOut(time, old.x, next.x - old.x, totalTime);//速さ
    float y = Sine::easeInOut(time, old.y, next.y - old.y, totalTime);//速さ
    float z = Sine::easeInOut(time, old.z, next.z - old.z, totalTime);//速さ

    SetPosition(DirectX::XMFLOAT3(x,y,z));
    //移動時間以上なら
    if (time >= totalTime)
    {
        SetMove(false);
    }
}

//-----ステージオブジェクトの基底クラス----------

void StageObj::UpdateTransform()
{
    //スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //回転行列を作成
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    //位置行列を作成
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    //  3つの行列を組合せ、ワールド行列を作成
    DirectX::XMMATRIX W = S * R * T;
    //計算したワールド行列を取り出す
    DirectX::XMStoreFloat4x4(&transform, W);//transformに書き込んでる...?
}

void StageObj::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);
}

void GameTutorialSignBoard::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    modelShader->Draw(rc, model);
}
