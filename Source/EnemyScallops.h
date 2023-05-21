#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

//���E���㉺�Ɉړ�����J�j
class EnemyScallops :public Enemy
{
public:
    EnemyScallops();
    ~EnemyScallops()override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    //void Render(ID3D11DeviceContext* dc, Shader* shader)override;
    void ModelRender(RenderContext rc, ModelShader* modelShader);

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

private:

    //���S�����Ƃ��ɌĂ΂��
    void OnDead() override;
    //�_���[�W����������Ƃ��ɌĂ΂��
    void OnDamaged()override;

    //�G�l�~�[�ƃ{�X�̓����蔻��
    void CollisionEnemyVsBoss();


    // �ҋ@�X�e�[�g�֑J��
    void TransitionIdleState();
    // �ҋ@�X�e�[�g�X�V����
    void UpdateIdleState(float elapsedTime);

    // ���S�X�e�[�g�֑J��
    void TransitionOpenState();
    // ���S�X�e�[�g�X�V����
    void UpdateOpenState(float elapsedTime);

private:
    // �X�e�[�g
    enum class State
    {
        Idle,
        Open
    };

private:
    // �A�j���[�V����
    enum Animation
    {
        Anim_Idle,
        Anim_Open,
    };

private:

    float   moveSpeed = 5.0f;
    float   turnSpeed = DirectX::XMConvertToRadians(720);


    Model* model = nullptr;

    State  state = State::Idle;

    Character* player = nullptr;

    float offsetX = 9.0f;

};

