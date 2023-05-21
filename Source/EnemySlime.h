#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

//�X���C��
class EnemySlime :public Enemy
{
public:
    EnemySlime();
    ~EnemySlime()override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    //void Render(ID3D11DeviceContext* dc, Shader* shader)override;
    void ModelRender(RenderContext rc, ModelShader* modelShader);



    // �ҋ@�X�e�[�g�֑J��
    void TransitionIdleState();
    // �ҋ@�X�e�[�g�X�V����
    void UpdateIdleState(float elapsedTime);

    // �_���[�W�X�e�[�g�֑J��
    void TransitionDamageState();
    // �_���[�W�X�e�[�g�X�V����
    void UpdateDamageState(float elapsedTime);

    // ���S�X�e�[�g�֑J��
    void TransitionDeadState();
    // ���S�X�e�[�g�X�V����
    void UpdateDeadState(float elapsedTime);

private:
    // �X�e�[�g
    enum class State
    {
        Idle,
        Move,
        Damage,
        Dead,
    };

private:
    // �A�j���[�V����
    enum Animation
    {
        Anim_Idle,
    };

protected:
    //���S�����Ƃ��ɌĂ΂��
    void OnDead() override;

private:

    float   moveSpeed = 5.0f;
    float   turnSpeed = DirectX::XMConvertToRadians(720);

    Model* model = nullptr;


    State  state = State::Idle;

    float offsetX = 9.0f;
};