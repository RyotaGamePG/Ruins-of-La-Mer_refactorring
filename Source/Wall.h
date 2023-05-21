#pragma once

#include "Graphics/Model.h"
#include "Graphics/Shader.h"
#include "Effect.h"
#include <DirectXMath.h>

#include "Audio/Audio.h"

#include "EffectManager.h"
//キャラクター
class Wall
{
public:
    Wall();
    ~Wall(); //継承先のデストラクタが呼ばれなくなってしまう

    //行列更新処理
    void UpdateTransform();

    //位置取得
    DirectX::XMFLOAT3& GetPosition(){ return position; }
    //位置特定
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    //回転取得
    const DirectX::XMFLOAT3& GetAngle() const { return angle; }
    //回転設定
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    //スケール取得
    const DirectX::XMFLOAT3& GetScale() const { return scale; }
    //スケール設定
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    //半径取得
    float GetRadius() const { return radius; }

    //地面に設置しているか
    bool IsGround() const { return isGround; }

    //高さ取得
    float GetHeight()const { return height; }
    float GetHitHeight()const { return hitPosHeight; }

    int GetHealth() { return health; }

    bool GetCollisionFlag() { return CollisionFlag; }
    void SetCollisionFlag(bool flag) { CollisionFlag = flag; }

    //当たり判定//------

    //当たり判定のフラグ
    const bool& GetHitPositionCollision() const { return CollisionFlag; }
    //位置の数取得
    const int& GetHitPositionNum() const { return hPosNum; }
    //位置取得
    const DirectX::XMFLOAT3& GetHitPosition(int i) const { return hitPosition[i]; }
    //半径取得
    const float& GetHitPositionRadius() const { return hitPosRadius; }
    //高さ取得
    const float& GetHitPositionHeight() const { return hitPosHeight; }

    //-------


    //更新処理
    void Update(float elapsedTime);
    //描画処理
    //void Render(ID3D11DeviceContext* dc, Shader* shader);
    void ModelRender(RenderContext rc, ModelShader* modelShader);

    //ダメージを与える
    bool ApplyDamage(int damage, float invincibleTime);

    //無敵時間更新処理
    void UpdateInvincibleTimer(float elapsedTime);

    //破棄処理
    //void Destroy();

    //無敵フラグを設定
    void SetInvincibleFlag(bool flag) { invincible = flag; }
    //無敵フラグを取得
    bool GetInvincibleFlag() { return invincible; }

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

private:

    //ダメージを受けた時に呼ばれる
    //void OnDamaged();
    //死亡したときに呼ばれる
    void OnDead();


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

    //当たり判定用-----
    bool                CollisionFlag = true;
    const static int    hPosNum = 14;
    DirectX::XMFLOAT3   hitPosition[hPosNum] = {};
    float               hitPosRadius = 0.0f;
    float               hitPosHeight = 0.0f;
    //--------

    float               radius = 6.0f;

    DirectX::XMFLOAT3   velocity = { 0,0,0 };

    bool                isGround = false;

    float               height = 2.0f;

    int                 health = 3;

    bool                invincible = false;
    float               invincibleTimer = 1.0f;

    float               friction = 0.5f;

    float               acceleration = 1.0f;
    float               maxMoveSpeed = 5.0f;
    float               moveVecX = 0.0f;
    float               moveVecZ = 0.0f;


private:
    Model* model = nullptr;

    Effect* damageEffect = nullptr;

    //効果音
    std::unique_ptr<AudioSource> damageSound = nullptr;
    std::unique_ptr<AudioSource> breakSound = nullptr;
};
