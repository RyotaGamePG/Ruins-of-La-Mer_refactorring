#pragma once

#include <DirectXMath.h>

#define GAME 1
#define EDIT 0
#define SPEEDUP 0

//ƒRƒŠƒWƒ‡ƒ“
class Collision
{
public:
    //‹…‚Æ‹…‚ÌŒğ·”»’è
    static bool InterSectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        DirectX::XMFLOAT3& outPositionB
    );

    //‰~’Œ‚Æ‰~’Œ‚ÌŒğ·”»’è
    static bool InterSectCylinderVsCylinder(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        float heightA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        float heightB,
        DirectX::XMFLOAT3& outPositionB
    );

    //‰~‚Æ‰~’Œ‚ÌŒğ·”»’è
    static bool IntersectSphereVsCylinder(
        const DirectX::XMFLOAT3& spherePosition,
        float sphereRadius,
        const DirectX::XMFLOAT3& cylinderPosition,
        float cylinderRadius,
        float cylinderHeight,
        DirectX::XMFLOAT3& outCylinderPosition
    );

};
