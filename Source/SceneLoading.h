//#pragma once
//
//#include "Graphics/Sprite.h"
//#include "Scene.h"
//#include "Stage.h"
//#include "CameraController.h"
//
////���[�f�B���O�V�[��
//class SceneLoading :public Scene
//{
//public:
//    SceneLoading(Scene* nextScene) :nextScene(nextScene) {}
//    ~SceneLoading()override {}
//
//    // ������
//    void Initialize()override;
//
//    // �I����
//    void Finalize()override;
//
//    // �X�V����
//    void Update(float elapsedTime)override;
//
//    // �`�揈��
//    void Render()override;
//
//    CameraController* cameraController = nullptr;
//
//    SelectSignBoard* loadingText = nullptr;
//
//private:
//    // ���[�f�B���O�X���b�h
//    static void LoadingThread(SceneLoading* scene);
//
//private:
//    Sprite* sprite = nullptr;
//    Scene* nextScene = nullptr;
//};


#pragma once

#include"Graphics/Sprite.h"
#include "Graphics/Texture.h"
#include "Scene.h"

//���[�f�B���O�V�[��
class SceneLoading :public Scene
{
public:
    SceneLoading(Scene* nextScene) : nextScene(nextScene) {}
    ~SceneLoading() override {}

    //������
    void Initialize()override;

    //�I����
    void Finalize()override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render()override;

private:
    //���[�f�B���O�X���b�h
    static void LoadingThread(SceneLoading* scene);

private:
    std::unique_ptr<Texture> texture;
    std::unique_ptr<Sprite>	sprite;

    float angle = 0.0f;
    Scene* nextScene = nullptr;

};