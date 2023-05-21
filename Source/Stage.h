#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"


//�X�e�[�W�I�u�W�F�N�g(���N���X)
class StageObj
{
public:
    StageObj() {}
    virtual ~StageObj() {}

    //�ʒu�ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
    DirectX::XMFLOAT3 GetPosition()const { return position; }

    //���a�擾
    float GetRadius() const { return radius; }

    //�����擾
    float GetLength() const { return radius * 2; }


    //�s��X�V����
    void UpdateTransform();

    //�X�V����
    virtual void Update(float elapsedTime) = 0;//�o�ߎ���

    //�`�揈��
    //virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;
    virtual void ModelRender(RenderContext rc, ModelShader* modelShader) = 0;
    //�f�o�b�O�v���~�e�B�u�`��
    virtual void DrawDebugPrimitive() {}


    //�ʒu���Z����
    void AddPos(DirectX::XMFLOAT3 pos) { position.x += pos.x, position.y += pos.y, position.z += pos.z; }

    //������
    virtual void SetMove(bool move) { isMove = move; }
    virtual bool GetMove() { return isMove; }

    enum Type
    {
        None=-1,
        Stage,
        Signboard,
        SelectSignboard,
        GameSelectSignboard,
        GameTutorialSignboard
    };

    Type type = None;

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

    float  radius = 13.6f;

    Model* model = nullptr;

    //�Z���N�g�̓����Ɏg�p
    bool  isMove = false;
};

//�X�e�[�W
class Stage :public StageObj
{
public:
    Stage();
    ~Stage()override;


    //�X�V����
    void Update(float elapsedTime);//�o�ߎ���

    //�`�揈��
    void ModelRender(RenderContext rc, ModelShader* modelShader);


    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

private:

    float  radius = 7.2f;

    Model* model = nullptr;

};

//�Ŕ�
class Signboard :public StageObj
{
public:
    Signboard(int index);
    virtual ~Signboard()override;

    //�X�V����
    virtual void Update(float elapsedTime);//�o�ߎ���

    //�`�揈��
    void ModelRender(RenderContext rc, ModelShader* modelShader);


    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();
private:

    float  radius = 5.0f;

    Model* model = nullptr;

    //������
    float time = 0.0f;
};

class SelectSignBoard :public StageObj
{
public:
    SelectSignBoard(int index);
    ~SelectSignBoard()override;

    //�X�V����
    void Update(float elapsedTime);//�o�ߎ���

    //�`�揈��
    void ModelRender(RenderContext rc, ModelShader* modelShader);


    //������
    void SetMove(bool move) { isMove = move; }
    
    void ScaleMove();

private:

    float  radius = 5.0f;

    Model* model = nullptr;

    //������
    float time = 0.0f;

    //�w�i��
    bool  isBackGround = false;
};

//�Q�[����ʂ̃Z���N�g�Ŕ�
class GameSelectSignBoard :public StageObj
{
public:
    GameSelectSignBoard(int index);
    ~GameSelectSignBoard()override;

    //�X�V����
    void Update(float elapsedTime);//�o�ߎ���

    //�`�揈��
    void ModelRender(RenderContext rc, ModelShader* modelShader);


    //�g����k�A�j���[�V����
    void ScaleMove();

    //���̈ʒu�ւ̃A�j���[�V����
    void NextPosMove();

    //�Â��ʒu��ݒ�
    void SetOldPos(DirectX::XMFLOAT3 pos) { oldPos = pos; }

    //���̈ʒu��ݒ�
    void SetNextPos(DirectX::XMFLOAT3 pos) { nextPos = pos; }

    //������
    void SetMove(bool move) { isMove = move; }

    //������
    void SetScaleMove(bool move) { isScaleMove = move; }
    bool GetScaleMove() { return isScaleMove; }

    bool text = false;
    

private:

    float time = 0.0f;

    bool  isMove = false;
    bool  isScaleMove = false;

    float  radius = 5.0f;

    Model* model = nullptr;

    //������(�ړ�����)
    float totalTime = 60.0f;

    DirectX::XMFLOAT3 oldPos = {};
    DirectX::XMFLOAT3 nextPos = {};
};

//�Q�[����ʂ̃Z���N�g�Ŕ�
class GameTutorialSignBoard :public StageObj
{
public:
    GameTutorialSignBoard(int index);
    ~GameTutorialSignBoard()override;

    //�X�V����
    void Update(float elapsedTime);//�o�ߎ���

    //�`�揈��
    void ModelRender(RenderContext rc, ModelShader* modelShader);


    //�g����k�A�j���[�V����
    void ScaleMove();

    //���̈ʒu�ւ̃A�j���[�V����
    void NextPosMove();

    //�Â��ʒu��ݒ�
    void SetOldPos(DirectX::XMFLOAT3 pos) { oldPos = pos; }

    //���̈ʒu��ݒ�
    void SetNextPos(DirectX::XMFLOAT3 pos) { nextPos = pos; }

    //������
    void SetMove(bool move) { isMove = move; }

    //������
    void SetScaleMove(bool move) { isScaleMove = move; }
    bool GetScaleMove() { return isScaleMove; }

    bool text = false;


private:

    float time = 0.0f;

    bool  isMove = false;
    bool  isScaleMove = false;

    float  radius = 5.0f;

    Model* model = nullptr;

    //������(�ړ�����)
    float totalTime = 60.0f;

    DirectX::XMFLOAT3 oldPos = {};
    DirectX::XMFLOAT3 nextPos = {};
};