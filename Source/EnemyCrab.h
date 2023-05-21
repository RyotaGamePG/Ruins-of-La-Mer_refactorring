#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

//���E���㉺�Ɉړ�����J�j
class EnemyCrab :public Enemy
{
public:
    EnemyCrab(bool type,bool move);
    ~EnemyCrab()override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    //void Render(ID3D11DeviceContext* dc, Shader* shader)override;
    void ModelRender(RenderContext rc, ModelShader* modelShader);

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    //�ҋ@���ԍX�V����
    void UpdateStopTimer(float elapsedTime);

    //���E�̈ړ���Z�b�g
    void setTatgetMovePosition();

    DirectX::XMFLOAT3 GetMoveVec() const;
    

    //TODO::�Ȃ��肪����
    void PlayerSet(Character* player) { this->player = player; }

private:

    //�n�ʂ̂߂肱�݋����Z�b�g
    void SetGroundJudgeFlag(bool b) { groundJudge = b; }

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

    // �E�ړ��X�e�[�g�֑J��
    void TransitionRightMoveState();
    // �E�ړ��X�e�[�g�X�V����
    void UpdateRightMoveState(float elapsedTim);

    // ���ړ��X�e�[�g�֑J��
    void TransitionLeftMoveState();
    // ���ړ��X�e�[�g�X�V����
    void UpdateLeftMoveState(float elapsedTim);

    // ��ړ��X�e�[�g�֑J��
    void TransitionUpMoveState();
    // ��ړ��X�e�[�g�X�V����
    void UpdateUpMoveState(float elapsedTim);

    // ���ړ��X�e�[�g�֑J��
    void TransitionDownMoveState();
    // ���ړ��X�e�[�g�X�V����
    void UpdateDownMoveState(float elapsedTim);

    // ���n�X�e�[�g�֑J��
    void TransitionLandState();
    // ���n�X�e�[�g�X�V����
    void UpdateLandState(float elapsedTime);

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
        Jump,
        Land,
        Damage,
        Dead,
        Reset,
        Right,
        Left,
        Up,
        Down
    };

private:
    // �A�j���[�V����
    enum Animation
    {
        Anim_Running,
        Anim_Idle,
    };


private:

    //�ړ��^�C�v---
    bool  crabType = true;      //�J�j�̎�ށBtrue:���ړ�  false:�c�ړ�

    //���E�ړ���---
    DirectX::XMFLOAT3  pos0 = {};   //�E�A��
    DirectX::XMFLOAT3  pos1 = {};   //���A��
    const float moveRange = 5.0f;   //�����ꏊ����ړ���ւ̔��a

    //���E�㉺��---
    bool  isPos1 = true;        //���A���ɂ��邩

    //�ҋ@����
    float stopTimer = 0.0f;     //�҂�����
    const float stopTime = 2.0f;//�҂����Ԓ萔


    float   moveSpeed = 5.0f;
    float   turnSpeed = DirectX::XMConvertToRadians(720);

    float offsetX = 9.0f;


    Model* model = nullptr;

    State  state = State::Idle;

    Character* player = nullptr;

};
