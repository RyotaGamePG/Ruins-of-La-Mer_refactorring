#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

#include "Audio/Audio.h"
#include "Effect.h"

//�X���C��
class EnemyBoss :public Enemy
{
public:
    EnemyBoss(int stageNum);
    ~EnemyBoss()override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    //void Render(ID3D11DeviceContext* dc, Shader* shader)override;
    void ModelRender(RenderContext rc, ModelShader* modelShader);
    //�f�o�b�N�pGUI�`��
    void DrawDebugGUI();

    //�ʒu���Z�b�g
    void ResetPosition();

    //�X�s�[�h�A�b�v
    void SpeedUp() { TransitionSpeedUpState(); }

    //�ړ�����
    void offsetAreaWall(float offsetX);

    //TODO::�Ȃ��肪����
    void PlayerSet(Character* player) { this->player =player; }

private:
    //���S�����Ƃ��ɌĂ΂��
    void OnDead() override;
    //���S��������Ԃ�
    bool IsDead() { return health < 0 ? true : false; }

    //�n�ʂƂ̔������邩
    void SetGroundJudgeFlag(bool b) { groundJudge = b; }
    bool GetGroundJudgeFlag() { return groundJudge; }

    //�_���[�W����������Ƃ��ɌĂ΂��
    void OnDamaged()override;


    //�G�l�~�[�ƃ{�X�̓����蔻��
    void CollisionEnemyVsBoss();

    // �_�E���X�e�[�g�֑J��
    void TransitionDownState();
    // �_�E���X�e�[�g�X�V����
    void UpdateDownState(float elapsedTime);

    // �ړ��X�e�[�g�֑J��
    void TransitionMoveState();
    // �ړ��X�e�[�g�X�V����
    void UpdateMoveState(float elapsedTim);

    // ���n�X�e�[�g�֑J��
    void TransitionLandState();
    // ���n�X�e�[�g�X�V����
    void UpdateLandState(float elapsedTime);

    // �_���[�W�X�e�[�g�֑J��
    void TransitionDamageState();
    // �_���[�W�X�e�[�g�X�V����
    void UpdateDamageState(float elapsedTime);

    // �N���オ��X�e�[�g�֑J��
    void TransitionGetUpState();
    // �N���オ��e�[�g�X�V����
    void UpdateGetUpState(float elapsedTime);

    // ���S�X�e�[�g�֑J��
    void TransitionDeadState();
    // ���S�X�e�[�g�X�V����
    void UpdateDeadState(float elapsedTime);

    // ���Z�b�g�X�e�[�g�֑J��
    void TransitionResetState();
    // ���Z�b�g�X�e�[�g�X�V����
    void UpdateResetState(float elapsedTime);

    // �X�s�[�h�A�b�v�X�e�[�g�֑J��
    void TransitionSpeedUpState();
    // �X�s�[�h�A�b�v�X�e�[�g�X�V����
    void UpdateSpeedUpState(float elapsedTime);


    //���Z�b�g�̍ۂ̒n�ʂ��蔲��������
    void ResetOffsetUpdate();

private:
    // �X�e�[�g
    enum class State
    {
        Down,
        Move,
        Jump,
        GetUp,
        Land,
        Damage,
        Dead,
        Reset,
        SpeedUp,
    };

private:
    // �A�j���[�V����
    enum Animation
    {
        Anim_Walk,
        Anim_Warp,
        Anim_Down,
        Anim_Stop,
        Anim_GetUp,
    };


private:
    Model* model = nullptr;

    State  state = State::Down;

    Character* player = nullptr;

    float  speedUpTimer = 0.0f;
    const float  speedUpTime = 3.0f;

    float stopTimer = 0.0f;
    const float stopTime = 5.0f*60.0f;

    float resetTimer = 0.0f;
    const float resetTime = 1.0f * 60.0f;

    float offsetX = 9.0f;

private:
    //���y

    //����
    std::unique_ptr<AudioSource> step = nullptr;

private://�G�t�F�N�g
    Effect* hitEffect = nullptr;
    Effect* moveEffect = nullptr;

};
