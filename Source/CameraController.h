#pragma once

#include <DirectXMath.h>

//�J�����R���g���[���[
class CameraController
{
public:
    CameraController() {}
    ~CameraController() {}

    //�X�V����
    void Update(float elapsedTime);

    //�^�[�Q�b�g�ʒu�ݒ�
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }
    
    //���ԏ�����
    void ResetTime() { time = 0; }

    //�Q�[����p�X�V����
    void GameUpdate(float elapsedTime);
    //�Z���N�g��p�X�V����
    void SelectUpdate(float elapsedTime);
    //�N���A��p�X�V����
    void ClearUpdate(float elapsedTime);

private:
    DirectX::XMFLOAT3   target = { 0,0,0 };
    DirectX::XMFLOAT3   angle = { 0,0,0 };
    float               rollSpeed = DirectX::XMConvertToRadians(90);
    float               range = 20.0f;

    //�W���o�����b�N����̂���X���̍ő��]�l�ƍŏ���]�l�𐧌�
    float               maxAngleX = DirectX::XMConvertToRadians(45);
    float               minAngleX = DirectX::XMConvertToRadians(-45);


    //�ǉ���������
private:    //�J�����ړ�

    int time = 0;

    //�ړ����v����
    int totalTime = 60;

    DirectX::XMFLOAT3 oldTarget = {};
    DirectX::XMFLOAT3 nextTarget = {};
    
public:

    //���v���Ԉȏ㉻��Ԃ�
    bool isTimeEnd() { return time > totalTime; }

    //�O�^�[�Q�b�g�ʒu�ݒ�
    void SetOldTarget(const DirectX::XMFLOAT3& target) { this->oldTarget = target; }
    //���^�[�Q�b�g�ʒu�ݒ�
    void SetNextTarget(const DirectX::XMFLOAT3& target) { this->nextTarget = target; }


    //�Ԃ̃^�[�Q�b�g�����߂鏈��
    DirectX::XMFLOAT3 MoveNextTarget(float elapsedTime);

};
