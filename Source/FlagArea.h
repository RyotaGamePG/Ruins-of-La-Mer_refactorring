#pragma once
#include <DirectXMath.h>
#include "Graphics/Shader.h"

#include "StageData.h"

class FlagArea
{
public:


    enum Type
    {
        None = -1,
        SpeedUp,
        Goal
    };


    FlagArea(
        DirectX::XMFLOAT3   pos0,
        DirectX::XMFLOAT3   pos1,
        Type type);

    ~FlagArea();

    void Update(float elapsedTime);

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

    //Yは関係なく
    bool OverlapArea(DirectX::XMFLOAT3 position);
    //種類取得
    Type getType() { return type; }
    
private:
    //XZ平面エリア
    DirectX::XMFLOAT3   posRU = { 0,0,0 };//右上 黒
    DirectX::XMFLOAT3   posLD = { 0,0,0 };//左下

    Type type = Type::None;

    float radius = 0.1f;

};
