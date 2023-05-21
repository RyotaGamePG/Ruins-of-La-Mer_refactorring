#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "EnemyBoss.h"

#include "Audio/Audio.h"

#include "EffectManager.h"
#include "Effect.h"

//�v���C���[
class Player :public Character
{
protected:
    //���n�����Ƃ��ɌĂ΂��
    void OnLanding() override;

public:
    Player();
    ~Player()override;

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    //void Render(ID3D11DeviceContext* dc, Shader* shader);
    void ModelRender(RenderContext rc, ModelShader* modelShader);
    //�f�o�b�O�pGUI�`��
    void DrawDebugGUI();

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugprimitive();

    //�ړ�����
    void offsetAreaWall(float offsetX);


    //�Q�[���I�[�o�[����Ԃ�
    bool IsGameOver() { return gameOverFlag; }
    //�Q�[���N���A����Ԃ�
    bool IsGameClear() { return gameClearFlag; }

public:

    //�{�X��ݒ�
    //TODO::�Ȃ��肪����
    void BossSet(EnemyBoss* boss) { this->boss = boss; }


private:

    //�X�e�B�b�N���͒l����ړ��x�N�g�����擾
    DirectX::XMFLOAT3 GetMoveVec() const;

    //�v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
    void CollisionPlayerVsEnemies();

    //�v���C���[�ƃ{�X�̏Փˏ���
    void CollisionPlayerVsBoss();

    //�v���C���[�ƕǂ̏Փˏ���
    void CollisionPlayerVsWall();

    //�ړ����͏���
    bool InputMove(float elapsedTime);

    //�W�����v���͏���
    bool InputJump();



    //�Q�[���I�[�o�[��ݒ�
    void SetGameOver() { gameOverFlag = true; }

    //�Q�[���N���A��ݒ�
    void SetGameClear() { gameClearFlag = true; }


    // �ҋ@�X�e�[�g�֑J��
    void TransitionIdleState();
    // �ҋ@�X�e�[�g�X�V����
    void UpdateIdleState(float elapsedTime);

    // �ړ��X�e�[�g�֑J��
    void TransitionMoveState();
    // �ړ��X�e�[�g�X�V����
    void UpdateMoveState(float elapsedTim);

    // �W�����v�X�e�[�g�֑J��
    void TransitionJumpState();
    // �W�����v�X�e�[�g�X�V����
    void UpdateJumpState(float elapsedTime);

    // ���n�X�e�[�g�֑J��
    void TransitionLandState();
    // ���n�X�e�[�g�X�V����
    void UpdateLandState(float elapsedTime);

    // ���S�X�e�[�g�֑J��
    void TransitionDeadState();
    // ���S�X�e�[�g�X�V����
    void UpdateDeadState(float elapsedTime);

public:
    // �N���A�X�e�[�g�֑J��
    void TransitionClearState();
private:
    // �N���A�X�e�[�g�X�V����
    void UpdateClearState(float elapsedTime);

public:

    // �X�e�[�g
    enum class State
    {
        Idle,
        Move,
        Jump,
        Land,
        Dead,
        Clear,
    };
public:

    State GetState() { return state; }
    //TODO::�����̗͉��t���O�ŊǗ����ăA�j���[�V�����I�����ɂ���s
    bool IsDead() { return state == State::Dead ? true : false; }

private:
    // �A�j���[�V����
    enum Animation
    {
        Anim_Idle,
        Anim_Running,
        Anim_Jump,
        Anim_Landing,
        Anim_Death,
    };

private:

    Model* model = nullptr;

    float   moveSpeed = 6.0f;

    float   turnSpeed = DirectX::XMConvertToRadians(1080);

    float   jumpSpeed = 20.0f;

    int     jumpCount = 0;
    int     jumpLimit = 1;

    float offsetX = 9.0f;

    State   state = State::Idle;

    EnemyBoss* boss = nullptr;

    //�Q�[���I�[�o�[�t���O
    bool gameOverFlag = false;
    //�Q�[���N���A�t���O
    bool gameClearFlag = false;

private:
    //���y
    std::unique_ptr<AudioSource> jump = nullptr;
    std::unique_ptr<AudioSource> stepOn = nullptr;
    std::unique_ptr<AudioSource> death = nullptr;
    std::unique_ptr<AudioSource> hotateOn = nullptr;


public:
    //TODO::�G�f�B�^�[���ǂ�
    int enemyType = 0;


private:
    float timer = 0;
    float timerSet = 0.2f;
    Effect* hitEffect = nullptr;
    Effect* damageEffect = nullptr;
    
};
