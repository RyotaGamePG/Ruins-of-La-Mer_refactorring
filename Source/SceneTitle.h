#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Stage.h"
#include "CameraController.h"
#include "Audio/Audio.h"
#include "Graphics/Texture.h"

#include "Graphics/Light.h"
#include "Graphics/LightManager.h"

//�^�C�g���V�[��(�Z���N�g�V�[������������)
class SceneTitle :public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle()override {}

    //������
    void Initialize() override;

    //�I����
    void Finalize() override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render()override;



    //�^�C�g���X�V����
    void TitleUpdate(float elapsedTime);
    //�^�C�g���`�揈��
    void TitleRender();

    //�Z���N�g�X�V���u
    void SelectUpdate(float elapsedTime);
    //�Z���N�g�`�揈��
    void SelectRender();



    //----------
    //  �؂�ւ����Ɏg������
    //----------
    void ResetTime() { time = 0; }

    void SetChangeScene() { moveFlag = true; }

    //�Q�[������Z���N�g�Ɉڍs����̂Ɏg��
    void SetSelectState();

private:

    //----------
    //  �ړ��A�j���[�V����
    //----------
    struct movePos
    {
        float orgP;     //���̈ʒu
        float nextP;    //���̈ʒu
    };
    movePos moveStagePos[5];
    movePos moveTitleStagePos;


    float totalMoveTime = 60.0f;   //���v�ړ�����
    float moveLength = 10.0f;      //�ړ�����

private:
    //Sprite* sprite = nullptr;

    //�A�b�v�f�[�g2023�N5��15��
    std::unique_ptr<Texture> texture;

    //std::unique_ptr<Texture> TitleTex;

    CameraController* cameraController = nullptr;

    //�^�C�g������
    SelectSignBoard* titleStage0 = nullptr;

    SelectSignBoard* backGround = nullptr;

    int selectNum = 0;  //�X�e�[�W�ԍ�


private:

    enum State
    {
        Title,
        Select
    };

    State state = State::Title;

    float       time = 0;
    float       soundTime = 30.0f;  //����{�^����炷���߂̂���

    bool        moveFlag = false;   //�Z���N�g�ƃ^�C�g���ɕς�邽�߂̃t���O
    bool        sceneGameFlag = false; //�Q�[���J�ڃt���O

private:
    //���y
    std::unique_ptr<AudioSource> music = nullptr;

    std::unique_ptr<AudioSource> select = nullptr;
    std::unique_ptr<AudioSource> decision = nullptr;

    Light* mainDirectionalLight = nullptr;
    std::unique_ptr<Sprite>	sprite;




};