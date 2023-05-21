#pragma once

#include <vector>
#include "Stage.h"
#include "Player.h"
#include "EnemyBoss.h"

//�X�e�[�W�}�l�[�W���[
class StageManager
{
private:
    StageManager() {}
    ~StageManager() {}

public:
    //�B��̃C���X�^���X�擾
    static StageManager& Instance() //�V���O���g��
    {
        static StageManager instance;
        return instance;
    }

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void ModelRender(RenderContext rc, ModelShader* modelShader);

    //�f�o�b�O�v���~�e�B�u�`��
    void StageManager::DrawDebugPrimitive();

    //�S�̈ʒu���Z����
    void AddPos(DirectX::XMFLOAT3 pos);

    //�S�̈ʒu����
    void SetPosition(DirectX::XMFLOAT3 pos);


    //�X�e�[�W�o�^
    void Register(StageObj* flagArea);

    //�X�e�[�W�S�폜
    void Clear();

    //�X�e�[�W���擾
    int GetStageCount() const { return static_cast<int>(stages.size()); }

    //�X�e�[�W�擾
    StageObj* GetStage(int index) { return stages.at(index); };

    //�X�e�[�W�폜
    void Remove(StageObj* flagArea);

    //�X�e�[�W����
    void SetStageObj(int stageNum);

    //�v���C���[�ݒ�
    void SetPlayer(Player* player) { this->player = player; }

    //�v���C���[�ʒu�Q�b�g
    DirectX::XMFLOAT3 GetBossPos() { return player->GetPosition(); }

    //�^�[�Q�b�g�ʒu�ݒ�
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    //�^�[�Q�b�g�Ƌ߂�����Ԃ�(�}�C�i�X�֌W�Ȃ�)
    bool IsNear(float pos0, float pos1, float length)
    {
        float P = pos0 - pos1;
        if (P < 0)P = -P;
        return P < length;
    }

    float length = 30.0f;
    DirectX::XMFLOAT3   target = { 0,0,0 };

private:
    std::vector<StageObj*>  stages;   //�����̃G�l�~�[�̃|�C���^�� std::vector�ŊǗ�
    std::vector<StageObj*>  removes;

    Player* player = nullptr;

    std::vector<Stage*> stageVec;
   // Model* model = nullptr;
};
