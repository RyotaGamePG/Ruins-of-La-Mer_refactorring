#pragma once

#include <vector>
#include "FlagArea.h"
#include "Player.h"
#include "EnemyBoss.h"

//�G�l�~�[�}�l�[�W���[
class FlagAreaManager
{
private:
    FlagAreaManager() {}
    ~FlagAreaManager() {}

public:
    //�B��̃C���X�^���X�擾
    static FlagAreaManager& Instance() //�V���O���g��
    {
        static FlagAreaManager instance;
        return instance;
    }


    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    //void Render(ID3D11DeviceContext* dc, Shader* shader);
    void ModelRender(RenderContext rc, ModelShader* modelShader);

    //�f�o�b�O�v���~�e�B�u�`��
    void FlagAreaManager::DrawDebugPrimitive();

    //�t���O�G���A�ݒu
    void SetFlagArea(int stageNum);

    //�G���A�o�^
    void Register(FlagArea* flagArea);

    //�G���A�S�폜
    void Clear();

    //�G���A���擾
    int GetEnemyCount() const { return static_cast<int>(areas.size()); }

    //�G���A�擾
    FlagArea* GetArea(int index) { return areas.at(index); };

    //�G���A�폜
    void Remove(FlagArea* flagArea);

    //�{�X�E�v���C���[�ݒ�
    void SetBoss(EnemyBoss* boss) { this->boss = boss; }
    void SetPlayer(Player* player) { this->player =player; }

    //�{�X�E�v���C���[�ʒu�Q�b�g
    DirectX::XMFLOAT3 GetBossPos() { return player->GetPosition(); }
    DirectX::XMFLOAT3 GetPlayerPos() { return boss->GetPosition(); }

    //�{�X�X�s�[�h�A�b�v
    void BossSpeedUp() { boss->SpeedUp(); }
private:
    std::vector<FlagArea*>  areas;   //�����̃G�l�~�[�̃|�C���^�� std::vector�ŊǗ�
    std::vector<FlagArea*>  removes;

    EnemyBoss*  boss = nullptr;
    Player*     player = nullptr;


    float length = 6.0f;

};
