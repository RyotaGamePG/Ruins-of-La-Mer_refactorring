#include "SceneManager.h"
#include "Graphics/Graphics.h"

//�R���X�g���N�^
SceneManager::SceneManager()
{
    //sprite = new Sprite("Data/Sprite/black.png");
   // sprite = new Sprite("Data/Sprite/LoadingIcon.png");

}

//�f�X�g���N�^
SceneManager::~SceneManager()
{
    delete sprite;
}


//�X�V����
void SceneManager::Update(float elapsedTime)
{
    if (currentScene != nullptr)
    {
        currentScene->Update(elapsedTime);
    }
}

//�`�揈��
void SceneManager::Render()
{
    if (currentScene != nullptr)
    {
        currentScene->Render();
    }
}

//�V�[���N���A
void SceneManager::Clear()
{
    if (currentScene != nullptr)
    {
        currentScene->Finalize();
        delete currentScene;
        currentScene = nullptr;
    }
}

//�V�[���؂�ւ�
void SceneManager::ChangeScene(Scene* scene)
{
    //�Â��V�[�����I������
    Clear();

    //�V�����V�[����ݒ�
    currentScene = scene;

    //���������̏ꍇ�͏���������
    if (currentScene->IsReady()==false)
    {
        //�V�[������������
        currentScene->Initialize();
    }
}