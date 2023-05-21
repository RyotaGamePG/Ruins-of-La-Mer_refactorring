#include "Enemy.h"
#include "EnemyManager.h"
#include "Graphics/Graphics.h"


//デバッグプリミティブ描画
void Enemy::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    ////衝突判定用のデバッグ球を描画
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    //衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

void Enemy::LaunchedUpdate(float elapsedTime)
{
    if (IsLaunch())
    {
        if (velocity.z == 0.0f)//力がないならフラグオフ
        {
            launch = false;
        }
    }
}

void Enemy::offsetAreaWall(float offsetX)
{
    if (position.x > offsetX)
    {
        position.x = offsetX;
        isAreaWall = true;
    }
    if (position.x < 0.0f)
    {
        position.x = 0.0f;
        isAreaWall = true;
    }
}

//破棄
void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
}

//描画処理
void Enemy::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    if (modelShader == nullptr)
    {
        std::string debugOutput = "Enemy::ModelRenderエラー";
        
    }
    
    modelShader->Draw(rc, model);
}
