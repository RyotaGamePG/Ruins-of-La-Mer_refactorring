#include "Collision.h"

// ���Ƌ��̌�������
bool Collision::InterSectSphereVsSphere(
    const DirectX::XMFLOAT3& positionA,
    float radiusA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB,
    DirectX::XMFLOAT3& outPositionB)
{
    //B��A�̒P�ʃx�N�g�����Z�o
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB , PositionA);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);//2��̂܂�(sq)
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    //��������(2��̂܂ܔ���)
    float range = radiusA+ radiusB ;
    if (lengthSq>range*range)//����Ă���
    {
        return false;
    }

    // A��B�������o��
    Vec = DirectX::XMVector3Normalize(Vec);//�����ł͐�΂Ɏg��Ȃ��Əo�Ȃ��̂Ŏg��(����Ƃ���͍��)(����Ƃ͌�X)
    Vec = DirectX::XMVectorScale(Vec, range);
    DirectX::XMVECTOR OutPositionB = DirectX::XMVectorAdd(PositionA, Vec);//�v���C���[�̈ʒu����x�N�g��������
    DirectX::XMStoreFloat3(&outPositionB, OutPositionB);

    return true;
}
bool Collision::InterSectCylinderVsCylinder(
    const DirectX::XMFLOAT3& positionA,
    float radiusA,
    float heightA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB,
    float heightB,
    DirectX::XMFLOAT3& outPositionB
)
{
    //A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
    if (positionA.y > positionB.y + heightB)
    {
        return false;
    }

    //A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
    if (positionA.y+heightA < positionB.y)
    {
        return false;
    }
    //XZ���ʂł͈̔̓`�F�b�N
    float vx = positionB.x - positionA.x;
    float vy = positionB.z - positionA.z;
    float lengthSq = (vx * vx) + (vy * vy);
    float range = radiusA + radiusB;
    if (lengthSq > range * range)
    {
        return false;
    }
    //�P�ʃx�N�g����
    float length = sqrtf(lengthSq);
    vx /= length;
    vy /= length;
    //A��B�������o��
    outPositionB.x = positionA.x + vx * range;
    outPositionB.y = positionB.y;
    outPositionB.z = positionA.z + vy * range;
    
    return true;
}

//���Ɖ~���̌�������
bool Collision::IntersectSphereVsCylinder(
    const DirectX::XMFLOAT3& spherePosition,
    float sphereRadius,
    const DirectX::XMFLOAT3& cylinderPosition,
    float cylinderRadius,
    float cylinderHeight,
    DirectX::XMFLOAT3& outCylinderPosition
)
{
    //���̑������~���̓�����Ȃ瓖�����Ă��Ȃ�
    if (spherePosition.y > cylinderPosition.y + cylinderHeight)
    {
        return false;
    }

    //���̓����~���̑�����艺�Ȃ瓖�����Ă��Ȃ�
    if (spherePosition.y + sphereRadius < cylinderPosition.y)
    {
        return false;
    }
    //XZ���ʂł͈̔̓`�F�b�N
    float vx = cylinderPosition.x - spherePosition.x;
    float vy = cylinderPosition.z - spherePosition.z;
    float lengthSq = (vx * vx) + (vy * vy);
    float range = sphereRadius + cylinderRadius;
    if (lengthSq > range * range)
    {
        return false;
    }
    //�P�ʃx�N�g����
    float length = sqrtf(lengthSq);
    vx /= length;
    vy /= length;
    //�����~���������o��
    outCylinderPosition.x = spherePosition.x + vx * range;
    outCylinderPosition.y = spherePosition.y;
    outCylinderPosition.z = spherePosition.z + vy * range;


    return true;
}
