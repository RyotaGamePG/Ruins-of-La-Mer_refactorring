#pragma once

#include "Scene.h"

#include "Graphics/Sprite.h"


//�V�[���}�l�[�W���[
class SceneManager
{
private:
    SceneManager();
    ~SceneManager();

public:
    //�B��̃C���X�^���X�擾
    static SceneManager& Instance()
    {
        static SceneManager instance;
        return instance;
    }

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render();

    //�V�[���N���A
    void Clear();       //�Ǘ����Ă���V�[���̏I���������s��

    //�V�[���؂�ւ�
    void ChangeScene(Scene* scene);

private:
    Scene* currentScene = nullptr;

    //-----�t�F�[�h�C���A�E�g�Ɏg�p
    Sprite* sprite = nullptr;
    DirectX::XMFLOAT4 color = {};
};
