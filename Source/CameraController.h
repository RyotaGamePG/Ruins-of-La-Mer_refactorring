#pragma once

#include <DirectXMath.h>

//カメラコントローラー
class CameraController
{
public:
    CameraController() {}
    ~CameraController() {}

    //更新処理
    void Update(float elapsedTime);

    //ターゲット位置設定
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }
    
    //時間初期化
    void ResetTime() { time = 0; }

    //ゲーム専用更新処理
    void GameUpdate(float elapsedTime);
    //セレクト専用更新処理
    void SelectUpdate(float elapsedTime);
    //クリア専用更新処理
    void ClearUpdate(float elapsedTime);

private:
    DirectX::XMFLOAT3   target = { 0,0,0 };
    DirectX::XMFLOAT3   angle = { 0,0,0 };
    float               rollSpeed = DirectX::XMConvertToRadians(90);
    float               range = 20.0f;

    //ジンバルロック回避のためX軸の最大回転値と最小回転値を制限
    float               maxAngleX = DirectX::XMConvertToRadians(45);
    float               minAngleX = DirectX::XMConvertToRadians(-45);


    //追加したもの
private:    //カメラ移動

    int time = 0;

    //移動合計時間
    int totalTime = 60;

    DirectX::XMFLOAT3 oldTarget = {};
    DirectX::XMFLOAT3 nextTarget = {};
    
public:

    //合計時間以上化を返す
    bool isTimeEnd() { return time > totalTime; }

    //前ターゲット位置設定
    void SetOldTarget(const DirectX::XMFLOAT3& target) { this->oldTarget = target; }
    //次ターゲット位置設定
    void SetNextTarget(const DirectX::XMFLOAT3& target) { this->nextTarget = target; }


    //間のターゲットを求める処理
    DirectX::XMFLOAT3 MoveNextTarget(float elapsedTime);

};
