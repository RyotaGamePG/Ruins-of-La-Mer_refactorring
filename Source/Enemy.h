#pragma once

#include "Graphics/Shader.h"
#include "Character.h"
#include "Effect.h"

//エネミー
class Enemy :public Character
{
public:
    Enemy() {}
    ~Enemy()override {}

    //更新処理
    virtual void Update(float elapsedTime) = 0;

    //描画処理
    //virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;
    //描画処理
    //え？バーチャルは？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？え？
    virtual void ModelRender(RenderContext rc, ModelShader* modelShader);
    //破棄処理
    virtual void Destroy();

    //デバッグプリミティブ描画
    virtual void DrawDebugPrimitive();

    //吹き飛ばされ更新処理
    void LaunchedUpdate(float elapsedTime);

    //エリアの移動制限処理
    void offsetAreaWall(float offsetX);

    //吹き飛ばされ設定
    void setLaunch() { launch = true; }

    //吹き飛ばされているか
    bool IsLaunch() { return launch; }

    //死亡したかを返す
    bool IsDead() { return health < 0 ? true : false; }


    //TODO::忘れずに
    //edit中に動かしたか
    int  num = -1;
    void SetEditMove() { editMove = true; }
    bool GetEditMove() { return editMove; }
    
    enum Type
    {
        none=-1,
        Boss,
        Uni,
        Crab,
        Scallops
    };

    Type getType() { return type; }

    // 敵オブジェクトが有効かどうかを判断するメソッド
    bool IsValid() const { return isValid; }

    // 敵オブジェクトの有効性を設定するメソッド
    void SetValid(bool valid) { isValid = valid; }

protected:
    bool   launch = false;  //吹き飛ばされ

    //TODO::忘れずに
    bool   editMove = false;  //edit中に動かしたか

    //上下移動に---
    bool isAreaWall = false;   //制限エリアに触れたか

    Type   type = Type::none;

    // 敵オブジェクトが有効かどうかを保持する変数
    bool isValid;

public:
    //エフェクト
    Effect* hitEffect = nullptr;

private:
    Model* model = nullptr;
};
