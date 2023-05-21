#pragma once
#include <DirectXMath.h>
#include "Graphics/Shader.h"

#include "StageData.h"

class FlagArea
{
public:


    enum Type
    {
        None = -1,
        SpeedUp,
        Goal
    };


    FlagArea(
        DirectX::XMFLOAT3   pos0,
        DirectX::XMFLOAT3   pos1,
        Type type);

    ~FlagArea();

    void Update(float elapsedTime);

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    //Y�͊֌W�Ȃ�
    bool OverlapArea(DirectX::XMFLOAT3 position);
    //��ގ擾
    Type getType() { return type; }
    
private:
    //XZ���ʃG���A
    DirectX::XMFLOAT3   posRU = { 0,0,0 };//�E�� ��
    DirectX::XMFLOAT3   posLD = { 0,0,0 };//����

    Type type = Type::None;

    float radius = 0.1f;

};
