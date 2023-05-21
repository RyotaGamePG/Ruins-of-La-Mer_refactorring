#pragma once

#pragma once

#include <vector>
#include "Wall.h"

//�G�l�~�[�}�l�[�W���[
class WallManager
{
private:
    WallManager() {}
    ~WallManager() {}

public:
    //�B��̃C���X�^���X�擾
    static WallManager& Instance() //�V���O���g��
    {
        static WallManager instance;
        return instance;
    }

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    //void Render(ID3D11DeviceContext* dc, Shader* shader);
    void ModelRender(RenderContext rc, ModelShader* modelShader);

    //�f�o�b�O�v���~�e�B�u�`��
    void WallManager::DrawDebugPrimitive();

    void SetCollisionFlag();

    //�ǐݒu
    void SetWall(int stageNum);


    //�G�l�~�[�o�^
    void Register(Wall* enemy);

    //�G�l�~�[�S�폜
    void Clear();

    //�G�l�~�[���擾
    int GetWallCount() const { return static_cast<int>(walls.size()); }

    //�G�l�~�[�擾
    Wall* GetWall(int index) { return walls.at(index); };

    //�G�l�~�[�폜
    void Remove(Wall* enemy);

private:
    std::vector<Wall*>  walls;   //�����̃G�l�~�[�̃|�C���^�� std::vector�ŊǗ�
    std::vector<Wall*>  removes;
};

