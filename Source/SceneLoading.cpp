//#include <thread>
//#include "Graphics/Graphics.h"
//#include "Camera.h"
//#include "Input/Input.h"
//#include "SceneLoading.h"
//#include "SceneManager.h"
//
////������
//void SceneLoading::Initialize()
//{
//    //�X�v���C�g������
//    //sprite = new Sprite("Data/Sprite/LoadingIcon.png");
//    //sprite = new Sprite("Data/Sprite/clearIcon_1.png");
//
//
//
//    loadingText = new SelectSignBoard(6);
//    loadingText->SetPosition(DirectX::XMFLOAT3(0.0f, -10.0f, 8.0f));
//
//
//    // �J�����R���g���[���[������
//    cameraController = new CameraController();
//
//    //�J�����̏����ݒ�
//    Graphics& graphics = Graphics::Instance();
//    Camera& camera = Camera::Instance();
//    camera.SetLookAt(
//        DirectX::XMFLOAT3(0, 0, -10),
//        DirectX::XMFLOAT3(0, 0, 0),
//        DirectX::XMFLOAT3(0, 1, 1)
//    );
//    camera.SetPerspectiveFov(
//        DirectX::XMConvertToRadians(45),
//        graphics.GetScreenWidth() / graphics.GetScreenHeight(),
//        0.1f,
//        1000.0f
//    );
//
//
//    //�X���b�h�J�n
//    std::thread thread(LoadingThread, this);
//
//    //�X���b�h�̊Ǘ������
//    thread.detach();
//}
//
////�I����
//void SceneLoading::Finalize()
//{
//    //�X�v���C�g�I����
//    /*
//    if (sprite != nullptr)
//    {
//        delete sprite;
//        sprite = nullptr;
//    }
//    */
//
//    //�J�����R���g���[���[�I����
//    if (cameraController != nullptr)
//    {
//        delete cameraController;
//        cameraController = nullptr;
//    }
//
//
//}
//
////�X�V����
//void SceneLoading::Update(float elapsedTime)
//{
//
//   
//    // ���̃V�[���̏���������������V�[����؂�ւ���
//    if (nextScene->IsReady())
//    {
//        SceneManager::Instance().ChangeScene(nextScene);
//    }
//    else
//    {
//        loadingText->Update(elapsedTime);
//
//        //�J�����R���g���[���[�X�V����
//        DirectX::XMFLOAT3 target = { 0,0,20 };
//        //target.y = 3.0f;
//        //target.z += 4.0f;
//        cameraController->SetTarget(target);
//        cameraController->SelectUpdate(elapsedTime);
//
//    }
//}
//
//void SceneLoading::Render()
//{
//    Graphics& graphics = Graphics::Instance();
//    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
//    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
//    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
//
//    //��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
//    FLOAT color[] = { 0.0f,0.0f,0.0f,1.0f };    //RGBA(0.0~1.0)
//    dc->ClearRenderTargetView(rtv, color);
//    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//    dc->OMSetRenderTargets(1, &rtv, dsv);
//
//    // �`�揈��
//    RenderContext rc;
//    //rc.lightDirection = { 0.0f, 0.0f, -1.0f, 0.0f };	// ���C�g�����i�������j
//
//
//    //�J�����p�����[�^�ݒ�
//    Camera& camera = Camera::Instance();
//    rc.view = camera.GetView();
//    rc.projection = camera.GetProjection();
//
//    //3D�X�v���C�g�`��
//    {
//      /*  Shader* shader = graphics.GetShader();
//        shader->Begin(dc, rc);
//
//        loadingText->Render(dc, shader);
//
//        shader->End(dc);*/
//    }
//}
//
//// ���[�f�B���O�X���b�h
//void SceneLoading::LoadingThread(SceneLoading* scene)
//{
//    // COM�֘A�̏������A�X���b�h���ɌĂԕK�v������
//    CoInitialize(nullptr);
//
//    // ���̃V�[���̏��������s��
//    scene->nextScene->Initialize();
//
//    // �X���b�h���I���O��COM�֘A�̏I����
//    CoUninitialize();
//
//    // ���̃V�[���̏��������ݒ�
//    scene->nextScene->SetReady();
//}


#include<thread>
#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include "SceneLoading.h"
#include "SceneGame.h"
#include "SceneTitle.h"
#include "SceneManager.h"


//������
void SceneLoading::Initialize()
{
    //�X�v���C�g������
    //�e�N�X�`����ǂݍ���
    texture = std::make_unique<Texture>("Data/Texture/LoadingIcon.png");
    sprite = std::make_unique<Sprite>();
    sprite->SetShaderResourceView(texture->GetShaderResourceView(), texture->GetWidth(), texture->GetHeight());

    //�X���b�h�J�n
    std::thread thread(LoadingThread, this);

    //�X���b�h�̊Ǘ������
    thread.detach();
}

//�I����
void SceneLoading::Finalize()
{
}

//�X�V����
void SceneLoading::Update(float elapsedTime)
{
    constexpr float speed = 180.0f;
    angle += speed * elapsedTime;

    float screenW = static_cast<float>(Graphics::Instance().GetScreenWidth());
    float screenH = static_cast<float>(Graphics::Instance().GetScreenHeight());
    float textureW = static_cast<float>(sprite->GetTextureWidth());
    float textureH = static_cast<float>(sprite->GetTextureHeight());
    float positionX = screenW - textureW;
    float positionY = screenH - textureH;

    sprite->Update(
        positionX, positionY,																							//�`��ꏊ
        textureW, textureH,		//�傫��
        0, 0,																							//�e�N�X�`���̕`��ӏ�
        static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()),				//�e�N�X�`���̕�����
        angle,																								//�A���O��
        1, 1, 1, 1);																					//RGBA



    //���̃V�[���̏���������������V�[����؂�ւ���
    if (nextScene->IsReady())
    {
        SceneManager::Instance().ChangeScene(nextScene);
        nextScene = nullptr;
    }
}

//�`�揈��
void SceneLoading::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f,1.0f,0.5f,1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    SpriteShader* shader = graphics.GetShader(SpriteShaderId::Default);
    RenderContext rc;
    rc.deviceContext = dc;

    // 2D�X�v���C�g�`��
    {
        //SpriteShader* shader = graphics.GetShader(SpriteShaderId::Default);
        //SpriteShader* shader = graphics.GetShader(SpriteShaderId::UVScroll);
        //SpriteShader* shader = graphics.GetShader(SpriteShaderId::Mask);

        shader->Begin(rc);

        shader->Draw(rc, sprite.get());

        shader->End(rc);
    }
}

//���[�f�B���O�X���b�h
void SceneLoading::LoadingThread(SceneLoading* scene)
{
    //COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
    CoInitialize(nullptr);

    //���̃V�[���̏��������s��
    scene->nextScene->Initialize();

    //�X���b�h���I���O��COM�֘A�̏I����
    CoUninitialize();

    //���̃V�[���̏��������ݒ�
    scene->nextScene->SetReady();
}