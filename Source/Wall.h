#pragma once

#include "Graphics/Model.h"
#include "Graphics/Shader.h"
#include "Effect.h"
#include <DirectXMath.h>

#include "Audio/Audio.h"

#include "EffectManager.h"
//�L�����N�^�[
class Wall
{
public:
    Wall();
    ~Wall(); //�p����̃f�X�g���N�^���Ă΂�Ȃ��Ȃ��Ă��܂�

    //�s��X�V����
    void UpdateTransform();

    //�ʒu�擾
    DirectX::XMFLOAT3& GetPosition(){ return position; }
    //�ʒu����
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    //��]�擾
    const DirectX::XMFLOAT3& GetAngle() const { return angle; }
    //��]�ݒ�
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    //�X�P�[���擾
    const DirectX::XMFLOAT3& GetScale() const { return scale; }
    //�X�P�[���ݒ�
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    //���a�擾
    float GetRadius() const { return radius; }

    //�n�ʂɐݒu���Ă��邩
    bool IsGround() const { return isGround; }

    //�����擾
    float GetHeight()const { return height; }
    float GetHitHeight()const { return hitPosHeight; }

    int GetHealth() { return health; }

    bool GetCollisionFlag() { return CollisionFlag; }
    void SetCollisionFlag(bool flag) { CollisionFlag = flag; }

    //�����蔻��//------

    //�����蔻��̃t���O
    const bool& GetHitPositionCollision() const { return CollisionFlag; }
    //�ʒu�̐��擾
    const int& GetHitPositionNum() const { return hPosNum; }
    //�ʒu�擾
    const DirectX::XMFLOAT3& GetHitPosition(int i) const { return hitPosition[i]; }
    //���a�擾
    const float& GetHitPositionRadius() const { return hitPosRadius; }
    //�����擾
    const float& GetHitPositionHeight() const { return hitPosHeight; }

    //-------


    //�X�V����
    void Update(float elapsedTime);
    //�`�揈��
    //void Render(ID3D11DeviceContext* dc, Shader* shader);
    void ModelRender(RenderContext rc, ModelShader* modelShader);

    //�_���[�W��^����
    bool ApplyDamage(int damage, float invincibleTime);

    //���G���ԍX�V����
    void UpdateInvincibleTimer(float elapsedTime);

    //�j������
    //void Destroy();

    //���G�t���O��ݒ�
    void SetInvincibleFlag(bool flag) { invincible = flag; }
    //���G�t���O���擾
    bool GetInvincibleFlag() { return invincible; }

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

private:

    //�_���[�W���󂯂����ɌĂ΂��
    //void OnDamaged();
    //���S�����Ƃ��ɌĂ΂��
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

    //�����蔻��p-----
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

    //���ʉ�
    std::unique_ptr<AudioSource> damageSound = nullptr;
    std::unique_ptr<AudioSource> breakSound = nullptr;
};
