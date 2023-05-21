#include "Collision.h"

// 球と球の交差判定
bool Collision::InterSectSphereVsSphere(
    const DirectX::XMFLOAT3& positionA,
    float radiusA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB,
    DirectX::XMFLOAT3& outPositionB)
{
    //B→Aの単位ベクトルを算出
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB , PositionA);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);//2乗のまま(sq)
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    //距離判定(2乗のまま判定)
    float range = radiusA+ radiusB ;
    if (lengthSq>range*range)//離れている
    {
        return false;
    }

    // AがBを押し出す
    Vec = DirectX::XMVector3Normalize(Vec);//ここでは絶対に使わないと出ないので使う(削れるところは削る)(削り作業は後々)
    Vec = DirectX::XMVectorScale(Vec, range);
    DirectX::XMVECTOR OutPositionB = DirectX::XMVectorAdd(PositionA, Vec);//プレイヤーの位置からベクトル方向に
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
    //Aの足元がBの頭より上なら当たっていない
    if (positionA.y > positionB.y + heightB)
    {
        return false;
    }

    //Aの頭がBの足元より下なら当たっていない
    if (positionA.y+heightA < positionB.y)
    {
        return false;
    }
    //XZ平面での範囲チェック
    float vx = positionB.x - positionA.x;
    float vy = positionB.z - positionA.z;
    float lengthSq = (vx * vx) + (vy * vy);
    float range = radiusA + radiusB;
    if (lengthSq > range * range)
    {
        return false;
    }
    //単位ベクトル化
    float length = sqrtf(lengthSq);
    vx /= length;
    vy /= length;
    //AがBを押し出す
    outPositionB.x = positionA.x + vx * range;
    outPositionB.y = positionB.y;
    outPositionB.z = positionA.z + vy * range;
    
    return true;
}

//球と円柱の交差判定
bool Collision::IntersectSphereVsCylinder(
    const DirectX::XMFLOAT3& spherePosition,
    float sphereRadius,
    const DirectX::XMFLOAT3& cylinderPosition,
    float cylinderRadius,
    float cylinderHeight,
    DirectX::XMFLOAT3& outCylinderPosition
)
{
    //球の足元が円柱の頭より上なら当たっていない
    if (spherePosition.y > cylinderPosition.y + cylinderHeight)
    {
        return false;
    }

    //球の頭が円柱の足元より下なら当たっていない
    if (spherePosition.y + sphereRadius < cylinderPosition.y)
    {
        return false;
    }
    //XZ平面での範囲チェック
    float vx = cylinderPosition.x - spherePosition.x;
    float vy = cylinderPosition.z - spherePosition.z;
    float lengthSq = (vx * vx) + (vy * vy);
    float range = sphereRadius + cylinderRadius;
    if (lengthSq > range * range)
    {
        return false;
    }
    //単位ベクトル化
    float length = sqrtf(lengthSq);
    vx /= length;
    vy /= length;
    //球が円柱を押し出す
    outCylinderPosition.x = spherePosition.x + vx * range;
    outCylinderPosition.y = spherePosition.y;
    outCylinderPosition.z = spherePosition.z + vy * range;


    return true;
}
