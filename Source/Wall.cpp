#include <imgui.h>
#include "Wall.h"
#include "WallManager.h"

#include "Graphics/Graphics.h"

Wall::Wall()
{

    //model = new Model("Data/model/Stage/wall.mdl");
    model = new Model("Data/model/Enemy/enemywall.mdl");

    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.01f;


    //幅、高さ設定
    radius = 5.0f;
    height = 5.0f;

    //当たり判定用設定
    hitPosRadius = 0.5f;
    hitPosHeight = 9.75f;

    //角度
    angle.y = DirectX::XMConvertToRadians(90);

    //効果音
    damageSound = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/bossmove.wav", false);
    //breakSound = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/break.wav", false);

    //エフェクト
    damageEffect = new Effect("Data/Effect/wallBreak.efkefc");
}

Wall::~Wall()
{
    delete model;

    delete damageEffect;
}

//更新処理
void Wall::Update(float elapsedTime)
{
    for (int i = 0; i < hPosNum; i++)
    {
        float left = position.x - radius*2;
        float r = radius*2 / hPosNum;
        //float x = left + (r * (i+1));
        float x = left+i;
        float z = position.z+ hitPosRadius / 2;
        DirectX::XMFLOAT3   pos = { x,position.y,z };
        hitPosition[i] = pos;
    }

    //無敵時間更新処理
    UpdateInvincibleTimer(elapsedTime);

    //オブジェクト行列を更新
    UpdateTransform();


    //モデル行列更新
    model->UpdateTransform(transform);
}

void Wall::ModelRender(RenderContext rc, ModelShader* modelShader)
{

    modelShader->Draw(rc,model);
}

//描画処理
//void Wall::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    //当たり判定を取っているなら描画
//    if (CollisionFlag)
//    {
//        shader->Draw(dc, model);
//    }
//
//#if EDIT
//
//    if (ImGui::Begin("Wall", nullptr, ImGuiWindowFlags_None))
//    {
//        //トランスフォーム
//        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
//        {
//            //位置
//            ImGui::InputFloat3("Position", &position.z);
//
//            //体力
//            ImGui::InputInt("Helth", &health);
//
//            unsigned int flag = (unsigned int)CollisionFlag;
//            ImGui::CheckboxFlags("flag", &flag, flag);
//            
//            /*
//            //位置
//            for (int i = 0; i < hPosNum; i++)
//            {
//                ImGui::InputFloat("hitPos%d", &hitPosition[i].z,i);
//            }
//            //回転
//            DirectX::XMFLOAT3 a;
//            a.x = DirectX::XMConvertToDegrees(angle.x);
//            a.y = DirectX::XMConvertToDegrees(angle.y);
//            a.z = DirectX::XMConvertToDegrees(angle.z);
//            ImGui::InputFloat3("Angle", &a.x);
//            angle.x = DirectX::XMConvertToRadians(a.x);
//            angle.y = DirectX::XMConvertToRadians(a.y);
//            angle.z = DirectX::XMConvertToRadians(a.z);
//            //スケール
//            ImGui::InputFloat3("Scale", &scale.x);
//
//            //速さ
//            ImGui::InputFloat("acceleration", &acceleration);
//            */
//        }
//    }
//    ImGui::End();
//#endif
//}

//行列更新処理
void Wall::UpdateTransform()
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

//デバッグプリミティブ描画
void Wall::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    ////衝突判定用のデバッグ球を描画
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    //衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    //当たり判定無し
    if (!CollisionFlag)return;

    for (int i = 0; i < hPosNum; i++)
    {
        debugRenderer->DrawCylinder(hitPosition[i], hitPosRadius, hitPosHeight, DirectX::XMFLOAT4(0.3f, 0, 0, 1));
    }
}

//ダメージを与える
bool Wall::ApplyDamage(int damage, float invincibleTime)
{
    //ダメージが0の場合は健康状態を変更する必要がない
    if (damage == 0)return false;

    //死亡している場合は健康状態を変更しない
    if (health <= 0)return false;

    //無敵時間中はダメージを与えない
    if (invincibleTimer > 0.0f)return false;

    //無敵ならダメージを与えない
    if (invincible)return false;

    // ダメージ処理
    //health -= damage;

    //無敵時間設定
    //invincibleTimer = invincibleTime;

    //下に少し移動
    position.y -= 3.3f;

    //効果音設定
    damageSound = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/bossmove.wav", false);
    damageSound->Play();

    //エフェクト
    for (int i = 0; i < hPosNum; i++)
    {
        DirectX::XMFLOAT3 pos = hitPosition[i];
        damageEffect->Play(pos);
    }

    //死亡通知
    if (health <= 0)
    {
        OnDead();
    }
    //ダメージ通知
    else
    {
        //OnDamaged();
    }

    //健康状態が変更した場合はtrueを返す
    return true;

}

void Wall::UpdateInvincibleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}


void Wall::OnDead()
{
    CollisionFlag = false;
    //WallManager::Instance().Remove(this);
}