#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"

#include "easing.h"

//�X�V����
void CameraController::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();
    //�J�����̉�]���x
    float speed = rollSpeed * elapsedTime;


    
    //�X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
    angle.x += ay * speed;
    angle.y += ax * speed;

    /////��Ximgui�ŏo����悤�ɂ���//////
    
    
    //X���̃J������]�𐧌�
    if (angle.x > maxAngleX) { angle.x = maxAngleX; }
    if (angle.x < minAngleX) { angle.x = minAngleX; }

    //Y���̉�]�l��-3.14�`3.14�Ɏ��܂�悤�ɂ���
    if (angle.y < -DirectX::XM_PI)//�������傫���l�ɂȂ�Ȃ��悤��
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }
    

    //�J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //��]�s�񂩂牡���s�x�N�g�������o��
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //�����_������̃x�N�g�������Ɉ��������ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;

    eye.x = target.x + front.x * range;//��{�������X�J���[
    eye.y = target.y + front.y * range;
    eye.z = target.z + front.z * range;

    //�J�����̎��_�ƒ����_��ݒ�
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

//�Q�[���X�V����
void CameraController::GameUpdate(float elapsedTime)
{
    angle.y = DirectX::XMConvertToRadians(90);
    angle.x = DirectX::XMConvertToRadians(-30);

    //�J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //��]�s�񂩂牡���s�x�N�g�������o��
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //�����_������̃x�N�g�������Ɉ��������ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;

    eye.x = target.x + front.x * range;//��{�������X�J���[
    eye.y = target.y + front.y * range;
    eye.z = target.z + front.z * range;

    //�J�����̎��_�ƒ����_��ݒ�
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}
//�Z���N�g�X�V����
void CameraController::SelectUpdate(float elapsedTime)
{
    //�J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //��]�s�񂩂牡���s�x�N�g�������o��
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //�����_������̃x�N�g�������Ɉ��������ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;

    eye.x = target.x + front.x * range;//��{�������X�J���[
    eye.y = target.y + front.y * range;
    eye.z = target.z + front.z * range;

    //�J�����̎��_�ƒ����_��ݒ�
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

//�N���A�X�V����
void CameraController::ClearUpdate(float elapsedTime)
{

    angle.y = DirectX::XMConvertToRadians(90);
    angle.x = DirectX::XMConvertToRadians(0);

    //�J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //��]�s�񂩂牡���s�x�N�g�������o��
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //�����_������̃x�N�g�������Ɉ��������ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;

    eye.x = target.x + front.x * range;//��{�������X�J���[
    eye.y = target.y + front.y * range;
    eye.z = target.z + front.z * range;

    //�J�����̎��_�ƒ����_��ݒ�
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

//���^�[�Q�b�g�܂ł̈ړ�����
DirectX::XMFLOAT3 CameraController::MoveNextTarget(float elapsedTime)
{

    DirectX::XMFLOAT3 old = oldTarget;
    DirectX::XMFLOAT3 next = nextTarget;

    float x = Sine::easeInOut(time, old.x, next.x-old.x, totalTime);//����
    float y = Sine::easeInOut(time, old.y, next.y-old.y, totalTime);//����
    float z = Sine::easeInOut(time, old.z, next.z-old.z, totalTime);//����

    time++;

    return DirectX::XMFLOAT3(x,y,z);
}