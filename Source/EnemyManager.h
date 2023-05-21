#pragma once

#include <vector>
#include "Enemy.h"
#include "Effect.h"

//�G�l�~�[�}�l�[�W���[
class EnemyManager
{
private:
    EnemyManager() {}
    ~EnemyManager() {}

public:
    //�B��̃C���X�^���X�擾
    static EnemyManager& Instance() //�V���O���g��
    {
        
        static EnemyManager instance;

        return instance;
    }


    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    //void Render(ID3D11DeviceContext* dc, Shader* shader);
    //�`�揈��
    void ModelRender(RenderContext rc, ModelShader* modelShader);
    //�f�o�b�O�v���~�e�B�u�`��
    void EnemyManager::DrawDebugPrimitive();

    //�G�l�~�[�o�^
    void Register(Enemy* enemy);

    //�G�l�~�[�S�폜
    void Clear();

    //�G�l�~�[���擾
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    //�G�l�~�[�擾
    Enemy* GetEnemy(int index) { return enemies.at(index); };

    //�G�l�~�[�폜
    void Remove(Enemy* enemy);



    //�G�l�~�[�ݒu
    void SetEnemy(int stageNum);



    //�y�ʉ��p

    //�^�[�Q�b�g�ʒu�ݒ�
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    //�^�[�Q�b�g�Ƌ߂�����Ԃ�(�}�C�i�X�֌W�Ȃ�)
    bool IsNear(float pos0, float pos1, float length)
    {
        float P = pos0 - pos1;
        if (P < 0)P = -P;
        return P < length;
    }

    bool IsBehind(float pos0, float pos1, float length)
    {
        float P = pos0 - pos1;
        return P < -length;
    }

    float length = 30.0f;
    DirectX::XMFLOAT3   target = { 0,0,0 };
    


private:
    //�G�l�~�[���m�̏Փˏ���
    void CollisionEnemyVsEnemies();

    //�G�l�~�[�ƕǂ̓����蔻��
    void CollisionEnemyVsWall();

private:
    std::vector<Enemy*>  enemies;   //�����̃G�l�~�[�̃|�C���^�� std::vector�ŊǗ�
    std::vector<Enemy*>  removes;

private://�G�t�F�N�g
    Effect* hitEffect = nullptr;
    Effect* moveEffect = nullptr;

};
