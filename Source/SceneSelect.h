#pragma once

#pragma once

#include "Graphics/Sprite.h"
#include "Graphics/Sprite_old.h"
#include "Scene.h"

//�^�C�g���V�[��
class SceneSelect :public Scene
{
public:
    SceneSelect() {}
    ~SceneSelect()override {}

    //������
    void Initialize() override;

    //�I����
    void Finalize() override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render()override;

private:
    SpriteOld* sprite = nullptr;
};
