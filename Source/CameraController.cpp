#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"

#include "easing.h"

//更新処理
void CameraController::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();
    //カメラの回転速度
    float speed = rollSpeed * elapsedTime;


    
    //スティックの入力値に合わせてX軸とY軸を回転
    angle.x += ay * speed;
    angle.y += ax * speed;

    /////後々imguiで出来るようにする//////
    
    
    //X軸のカメラ回転を制限
    if (angle.x > maxAngleX) { angle.x = maxAngleX; }
    if (angle.x < minAngleX) { angle.x = minAngleX; }

    //Y軸の回転値を-3.14～3.14に収まるようにする
    if (angle.y < -DirectX::XM_PI)//すごい大きい値にならないように
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }
    

    //カメラ回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //回転行列から横歩行ベクトルを取り出す
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //注視点から後ろのベクトル方向に一定方向離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye;

    eye.x = target.x + front.x * range;//基準＋方向＊スカラー
    eye.y = target.y + front.y * range;
    eye.z = target.z + front.z * range;

    //カメラの視点と注視点を設定
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

//ゲーム更新処理
void CameraController::GameUpdate(float elapsedTime)
{
    angle.y = DirectX::XMConvertToRadians(90);
    angle.x = DirectX::XMConvertToRadians(-30);

    //カメラ回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //回転行列から横歩行ベクトルを取り出す
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //注視点から後ろのベクトル方向に一定方向離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye;

    eye.x = target.x + front.x * range;//基準＋方向＊スカラー
    eye.y = target.y + front.y * range;
    eye.z = target.z + front.z * range;

    //カメラの視点と注視点を設定
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}
//セレクト更新処理
void CameraController::SelectUpdate(float elapsedTime)
{
    //カメラ回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //回転行列から横歩行ベクトルを取り出す
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //注視点から後ろのベクトル方向に一定方向離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye;

    eye.x = target.x + front.x * range;//基準＋方向＊スカラー
    eye.y = target.y + front.y * range;
    eye.z = target.z + front.z * range;

    //カメラの視点と注視点を設定
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

//クリア更新処理
void CameraController::ClearUpdate(float elapsedTime)
{

    angle.y = DirectX::XMConvertToRadians(90);
    angle.x = DirectX::XMConvertToRadians(0);

    //カメラ回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //回転行列から横歩行ベクトルを取り出す
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //注視点から後ろのベクトル方向に一定方向離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye;

    eye.x = target.x + front.x * range;//基準＋方向＊スカラー
    eye.y = target.y + front.y * range;
    eye.z = target.z + front.z * range;

    //カメラの視点と注視点を設定
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

//次ターゲットまでの移動処理
DirectX::XMFLOAT3 CameraController::MoveNextTarget(float elapsedTime)
{

    DirectX::XMFLOAT3 old = oldTarget;
    DirectX::XMFLOAT3 next = nextTarget;

    float x = Sine::easeInOut(time, old.x, next.x-old.x, totalTime);//速さ
    float y = Sine::easeInOut(time, old.y, next.y-old.y, totalTime);//速さ
    float z = Sine::easeInOut(time, old.z, next.z-old.z, totalTime);//速さ

    time++;

    return DirectX::XMFLOAT3(x,y,z);
}