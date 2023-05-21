#pragma once

#include "Graphics/Shader.h"
#include "Character.h"
#include "Effect.h"

//�G�l�~�[
class Enemy :public Character
{
public:
    Enemy() {}
    ~Enemy()override {}

    //�X�V����
    virtual void Update(float elapsedTime) = 0;

    //�`�揈��
    //virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;
    //�`�揈��
    //���H�o�[�`�����́H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H���H
    virtual void ModelRender(RenderContext rc, ModelShader* modelShader);
    //�j������
    virtual void Destroy();

    //�f�o�b�O�v���~�e�B�u�`��
    virtual void DrawDebugPrimitive();

    //������΂���X�V����
    void LaunchedUpdate(float elapsedTime);

    //�G���A�̈ړ���������
    void offsetAreaWall(float offsetX);

    //������΂���ݒ�
    void setLaunch() { launch = true; }

    //������΂���Ă��邩
    bool IsLaunch() { return launch; }

    //���S��������Ԃ�
    bool IsDead() { return health < 0 ? true : false; }


    //TODO::�Y�ꂸ��
    //edit���ɓ���������
    int  num = -1;
    void SetEditMove() { editMove = true; }
    bool GetEditMove() { return editMove; }
    
    enum Type
    {
        none=-1,
        Boss,
        Uni,
        Crab,
        Scallops
    };

    Type getType() { return type; }

    // �G�I�u�W�F�N�g���L�����ǂ����𔻒f���郁�\�b�h
    bool IsValid() const { return isValid; }

    // �G�I�u�W�F�N�g�̗L������ݒ肷�郁�\�b�h
    void SetValid(bool valid) { isValid = valid; }

protected:
    bool   launch = false;  //������΂���

    //TODO::�Y�ꂸ��
    bool   editMove = false;  //edit���ɓ���������

    //�㉺�ړ���---
    bool isAreaWall = false;   //�����G���A�ɐG�ꂽ��

    Type   type = Type::none;

    // �G�I�u�W�F�N�g���L�����ǂ�����ێ�����ϐ�
    bool isValid;

public:
    //�G�t�F�N�g
    Effect* hitEffect = nullptr;

private:
    Model* model = nullptr;
};
