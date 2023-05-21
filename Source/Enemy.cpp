#include "Enemy.h"
#include "EnemyManager.h"
#include "Graphics/Graphics.h"


//�f�o�b�O�v���~�e�B�u�`��
void Enemy::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    ////�Փ˔���p�̃f�o�b�O����`��
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    //�Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

void Enemy::LaunchedUpdate(float elapsedTime)
{
    if (IsLaunch())
    {
        if (velocity.z == 0.0f)//�͂��Ȃ��Ȃ�t���O�I�t
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

//�j��
void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
}

//�`�揈��
void Enemy::ModelRender(RenderContext rc, ModelShader* modelShader)
{
    if (modelShader == nullptr)
    {
        std::string debugOutput = "Enemy::ModelRender�G���[";
        
    }
    
    modelShader->Draw(rc, model);
}
