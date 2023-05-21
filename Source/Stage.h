#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"


//ステージオブジェクト(基底クラス)
class StageObj
{
public:
    StageObj() {}
    virtual ~StageObj() {}

    //位置設定
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
    DirectX::XMFLOAT3 GetPosition()const { return position; }

    //半径取得
    float GetRadius() const { return radius; }

    //長さ取得
    float GetLength() const { return radius * 2; }


    //行列更新処理
    void UpdateTransform();

    //更新処理
    virtual void Update(float elapsedTime) = 0;//経過時間

    //描画処理
    //virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;
    virtual void ModelRender(RenderContext rc, ModelShader* modelShader) = 0;
    //デバッグプリミティブ描画
    virtual void DrawDebugPrimitive() {}


    //位置加算処理
    void AddPos(DirectX::XMFLOAT3 pos) { position.x += pos.x, position.y += pos.y, position.z += pos.z; }

    //動くか
    virtual void SetMove(bool move) { isMove = move; }
    virtual bool GetMove() { return isMove; }

    enum Type
    {
        None=-1,
        Stage,
        Signboard,
        SelectSignboard,
        GameSelectSignboard,
        GameTutorialSignboard
    };

    Type type = None;

protected:

    DirectX::XMFLOAT3   position = { 0,0,0 };
    DirectX::XMFLOAT3   angle = { 0,0,0 };
    DirectX::XMFLOAT3   scale = { 1,1,1 };
    DirectX::XMFLOAT4X4 transform = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    float  radius = 13.6f;

    Model* model = nullptr;

    //セレクトの動きに使用
    bool  isMove = false;
};

//ステージ
class Stage :public StageObj
{
public:
    Stage();
    ~Stage()override;


    //更新処理
    void Update(float elapsedTime);//経過時間

    //描画処理
    void ModelRender(RenderContext rc, ModelShader* modelShader);


    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

private:

    float  radius = 7.2f;

    Model* model = nullptr;

};

//看板
class Signboard :public StageObj
{
public:
    Signboard(int index);
    virtual ~Signboard()override;

    //更新処理
    virtual void Update(float elapsedTime);//経過時間

    //描画処理
    void ModelRender(RenderContext rc, ModelShader* modelShader);


    //デバッグプリミティブ描画
    void DrawDebugPrimitive();
private:

    float  radius = 5.0f;

    Model* model = nullptr;

    //お試し
    float time = 0.0f;
};

class SelectSignBoard :public StageObj
{
public:
    SelectSignBoard(int index);
    ~SelectSignBoard()override;

    //更新処理
    void Update(float elapsedTime);//経過時間

    //描画処理
    void ModelRender(RenderContext rc, ModelShader* modelShader);


    //動くか
    void SetMove(bool move) { isMove = move; }
    
    void ScaleMove();

private:

    float  radius = 5.0f;

    Model* model = nullptr;

    //お試し
    float time = 0.0f;

    //背景か
    bool  isBackGround = false;
};

//ゲーム画面のセレクト看板
class GameSelectSignBoard :public StageObj
{
public:
    GameSelectSignBoard(int index);
    ~GameSelectSignBoard()override;

    //更新処理
    void Update(float elapsedTime);//経過時間

    //描画処理
    void ModelRender(RenderContext rc, ModelShader* modelShader);


    //拡大収縮アニメーション
    void ScaleMove();

    //次の位置へのアニメーション
    void NextPosMove();

    //古い位置を設定
    void SetOldPos(DirectX::XMFLOAT3 pos) { oldPos = pos; }

    //次の位置を設定
    void SetNextPos(DirectX::XMFLOAT3 pos) { nextPos = pos; }

    //動くか
    void SetMove(bool move) { isMove = move; }

    //動くか
    void SetScaleMove(bool move) { isScaleMove = move; }
    bool GetScaleMove() { return isScaleMove; }

    bool text = false;
    

private:

    float time = 0.0f;

    bool  isMove = false;
    bool  isScaleMove = false;

    float  radius = 5.0f;

    Model* model = nullptr;

    //お試し(移動処理)
    float totalTime = 60.0f;

    DirectX::XMFLOAT3 oldPos = {};
    DirectX::XMFLOAT3 nextPos = {};
};

//ゲーム画面のセレクト看板
class GameTutorialSignBoard :public StageObj
{
public:
    GameTutorialSignBoard(int index);
    ~GameTutorialSignBoard()override;

    //更新処理
    void Update(float elapsedTime);//経過時間

    //描画処理
    void ModelRender(RenderContext rc, ModelShader* modelShader);


    //拡大収縮アニメーション
    void ScaleMove();

    //次の位置へのアニメーション
    void NextPosMove();

    //古い位置を設定
    void SetOldPos(DirectX::XMFLOAT3 pos) { oldPos = pos; }

    //次の位置を設定
    void SetNextPos(DirectX::XMFLOAT3 pos) { nextPos = pos; }

    //動くか
    void SetMove(bool move) { isMove = move; }

    //動くか
    void SetScaleMove(bool move) { isScaleMove = move; }
    bool GetScaleMove() { return isScaleMove; }

    bool text = false;


private:

    float time = 0.0f;

    bool  isMove = false;
    bool  isScaleMove = false;

    float  radius = 5.0f;

    Model* model = nullptr;

    //お試し(移動処理)
    float totalTime = 60.0f;

    DirectX::XMFLOAT3 oldPos = {};
    DirectX::XMFLOAT3 nextPos = {};
};