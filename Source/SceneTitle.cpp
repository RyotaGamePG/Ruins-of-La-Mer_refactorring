#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneSelect.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "Camera.h"
#include "easing.h"
#include "StageManager.h"

//������
void SceneTitle::Initialize()
{
    //�X�v���C�g������
    //sprite = new Sprite("Data/Sprite/Title.png");
    //sprite = new Sprite("Data/Sprite/title0.png");

    //texture = std::make_unique<Texture>("Data/Sprite/Title.png");

    sprite_render = new SpriteOld("Data/Sprite/Title.png");



    //�e�N�X�`��

    //�w�i�̏�����
    backGround= new SelectSignBoard(0);
    backGround->SetPosition(DirectX::XMFLOAT3(0, 0, 0));


    //�Z���N�g�̏�����
    for (int i = 0; i < 4; i++) {
        SelectSignBoard* stage = new SelectSignBoard(i+2);
        //stage[i]->SetPosition(DirectX::XMFLOAT3(5-i*4, -2, -10 ));
        stage->SetPosition(DirectX::XMFLOAT3(i, -2, 0 ));
        stage->AddPos(DirectX::XMFLOAT3(-40, 0, 0));
        StageManager::Instance().Register(stage);
    }

    //�^�C�g���̏�����
    titleStage0 = new SelectSignBoard(1);
    titleStage0->SetPosition(DirectX::XMFLOAT3(0, -5, 0));


    //���y�̃��[�h�ƍĐ�
    music = Audio::Instance().LoadAudioSource("Data/Audio/BGM/Title.wav",true);
    music->SetVolume(0.3f);
    music->Play();

    select = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/kasol.wav",false);
    decision = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/kettei.wav",false);


    // �J�����R���g���[���[������
    cameraController = new CameraController();

    //�J�����̏����ݒ�
    Graphics& graphics = Graphics::Instance();
    Camera& camera = Camera::Instance();
    camera.SetLookAt(
        DirectX::XMFLOAT3(0, 0, -20),
        DirectX::XMFLOAT3(0, 0, 0),
        DirectX::XMFLOAT3(0, 1, 1)
    );
    camera.SetPerspectiveFov(
        DirectX::XMConvertToRadians(45),
        graphics.GetScreenWidth() / graphics.GetScreenHeight(),
        0.1f,
        1000.0f
    );


    // ���s������ǉ�
    {
        mainDirectionalLight = new Light(LightType::Directional);
        mainDirectionalLight->SetDirection({ -1.410, -0.246f, 0.878f });
        mainDirectionalLight->SetColor(DirectX::XMFLOAT4(1.00, 0.915, 0.915, 1.0f));
        LightManager::Instane().Register(mainDirectionalLight);
    }


    //�^�C�g���`��p�̃e�N�X�`���ǂݍ���
    
    
  /*  sprite = std::make_unique<Sprite>();
    sprite->SetShaderResourceView(TitleTex->GetShaderResourceView(), TitleTex->GetWidth(), TitleTex->GetHeight());
    sprite->Update(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
        0, 0, static_cast<float>(TitleTex->GetWidth()), static_cast<float>(TitleTex->GetHeight()),
        0,
        1, 1, 1, 1);*/

    //����{�^���p�^�C�}�[
    soundTime = 30.0f;

}

//�I����
void SceneTitle::Finalize()
{

    //�J�����R���g���[���[�I����
    if (cameraController != nullptr)
    {
        delete cameraController;
        cameraController = nullptr;
    }

    //�^�C�g���X�e�[�W�I����
    if (titleStage0 != nullptr)
    {
        delete titleStage0;
        titleStage0 = nullptr;
    }

    StageManager::Instance().Clear();

    //�w�i�I����
    if (backGround != nullptr)
    {
        delete backGround;
        backGround = nullptr;
    }

    //�X�v���C�g�I����
    //if (sprite != nullptr)
    //{
    //    delete sprite;
    //    sprite = nullptr;
    //}
}

//�X�V����
void SceneTitle::Update(float elapsedTime)
{
    backGround->Update(elapsedTime);

    switch (state)
    {
    case State::Title:
        TitleUpdate(elapsedTime);
        break;
    case State::Select:
        SelectUpdate(elapsedTime);
        break;
    }
}

//�Z���N�g�Ɉڍs
void SceneTitle::SetSelectState()
{
    state = State::Title;
    moveFlag = true;
    time = 59.0f;
}

//�^�C�g���X�V����
void SceneTitle::TitleUpdate(float elapsedTime)
{

    GamePad& gamePad = Input::Instance().GetGamePad();

    //�Ȃɂ��{�^������������^�C�g���V�[���֐؂�ւ�
    const GamePadButton anyButton =
        GamePad::BTN_A
        | GamePad::BTN_B
        | GamePad::BTN_X
        | GamePad::BTN_Y;
    if (gamePad.GetButtonDown() & anyButton)
    {
        moveFlag = true;

        /*
        state = State::Select;
        StageManager::Instance().AddPos(DirectX::XMFLOAT3(-10,0,0));
        */

        /*
        //�Z�k
        SceneGame* game = new SceneGame;
        game->SetStageNum(0);
        SceneManager::Instance().ChangeScene(new SceneLoading(game));
        */
    }


    //�Z���N�g�Ɉڍs
    if (moveFlag)
    {
        if (time >= 60)
        {
            //�Z���N�g�Ɉڍs
            state = State::Select;
            moveFlag = false;
            time = 0;

        }
        else
        {
            //�ړ�
            //float pos = Back::easeInOut(time, 0.0f, 30.0f, 60);//����
            float pos = Back::easeInOut(time, -5.0f, 30.0f, 60);//����
            titleStage0->SetPosition(DirectX::XMFLOAT3(0, pos, 0));
            //StageManager::Instance().AddPos(DirectX::XMFLOAT3(pos+10.0f, 0, 0));
            float pos1 = Back::easeInOut(time, -45.0f, 60.0f, 60);//����
            StageManager::Instance().GetStage(0)->SetPosition(DirectX::XMFLOAT3(pos1 , 0, 0));
            StageManager::Instance().GetStage(1)->SetPosition(DirectX::XMFLOAT3(pos1 - 15.0f, 0, 0));
            StageManager::Instance().GetStage(2)->SetPosition(DirectX::XMFLOAT3(pos1 - 30.0f, 0, 0));
            float pos2 = Back::easeInOut(time, -45.0f, 60.0f, 60);//����
            StageManager::Instance().GetStage(3)->SetPosition(DirectX::XMFLOAT3(pos2, 10, 0));
            time++;
        }
    }


    titleStage0->Update(elapsedTime);
    StageManager::Instance().Update(elapsedTime);

    //�J�����R���g���[���[�X�V����
    DirectX::XMFLOAT3 target = { 0,0,20 };
    //target.y = 3.0f;
    //target.z += 4.0f;
    cameraController->SetTarget(target);	
    cameraController->SelectUpdate(elapsedTime);

}

//�Z���N�g�X�V����
void SceneTitle::SelectUpdate(float elapsedTime)
{
    //�^�C�g���Ɉڍs
    if (moveFlag)
    {
        if (time >= 60)
        {
            state = State::Title;
            moveFlag = false;
            time = 0;
        }
        else
        {
            //�ړ�
            //float pos = Back::easeInOut(time, 30.0f, -30.0f, 60);//����
            float pos = Back::easeInOut(time, 30.0f, -35.0f, 60);//����
            titleStage0->SetPosition(DirectX::XMFLOAT3(0, pos, 0));
            float pos1 = Back::easeInOut(time, 15.0f, -60.0f, 60);//����
            StageManager::Instance().GetStage(0)->SetPosition(DirectX::XMFLOAT3(pos1, 0, 0));
            StageManager::Instance().GetStage(1)->SetPosition(DirectX::XMFLOAT3(pos1 - 15.0f, 0, 0));
            StageManager::Instance().GetStage(2)->SetPosition(DirectX::XMFLOAT3(pos1 - 30.0f, 0, 0));
            float pos2 = Back::easeInOut(time, 15.0f, -60.0f, 60);//����
            StageManager::Instance().GetStage(3)->SetPosition(DirectX::XMFLOAT3(pos2, 10, 0));
            time++;
        }


    }
    //�ŏ��ɃX�P�[���A�j���[�V����
    if (!StageManager::Instance().GetStage(selectNum)->GetMove())
        StageManager::Instance().GetStage(selectNum)->SetMove(true);

    GamePad& gamePad = Input::Instance().GetGamePad();

    //�Q�[���V�[������Ȃ��Ȃ�X�V
    if (!sceneGameFlag) {
        //�I����͏���
        if (selectNum < 2) {
            if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT)
            {
                //�O�𓮂����Ȃ�
                StageManager::Instance().GetStage(selectNum)->SetMove(false);
                selectNum++;
                //������
                StageManager::Instance().GetStage(selectNum)->SetMove(true);

                //���ʉ�
                select = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/kasol.wav", false);
                select->Play();
            }
        }

        //�I����͏���
        if (selectNum > 0) {
            if (gamePad.GetButtonDown() & GamePad::BTN_LEFT)
            {
                //�O�𓮂����Ȃ�
                StageManager::Instance().GetStage(selectNum)->SetMove(false);
                selectNum--;
                //������
                StageManager::Instance().GetStage(selectNum)->SetMove(true);

                //���ʉ�
                select = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/kasol.wav", false);
                select->Play();
            }
        }



        //�J�����R���g���[���[�X�V����
        DirectX::XMFLOAT3 target = { 0,0,20 };
        cameraController->SetTarget(target);	//�v���C���[�̍���������^�[�Q�b�g�ɐݒ�
        cameraController->SelectUpdate(elapsedTime);

        //�ŔX�V����
        titleStage0->Update(elapsedTime);
        StageManager::Instance().Update(elapsedTime);
    }

    if (!moveFlag)
    {
        //�Q�[���V�[���ւ̃t���O������
        if (sceneGameFlag)
        {
            if (soundTime <= 0)
            {
                //�V�[���Q�[���֑J��
                SceneGame* game = new SceneGame;
                game->SetStageNum(selectNum);
                SceneManager::Instance().ChangeScene(new SceneLoading(game));
            }
            soundTime--;
        }
        else
        {
            //�Ȃɂ��{�^������������Q�[���V�[���֐؂�ւ�
            const GamePadButton anyButton =
                GamePad::BTN_A
                | GamePad::BTN_B;
            if (gamePad.GetButtonDown() & anyButton)
            {
                switch (gamePad.GetButtonDown() & anyButton)
                {

                    //SceneManager::Instance().ChangeScene(new SceneSelect);

                case  GamePad::BTN_A:   //Z
                    //�Q�[���V�[���ւ̃t���O
                    sceneGameFlag = true;
                    //���艹
                    decision = Audio::Instance().LoadAudioSource("Data/Audio/SE/WAV/kettei.wav", false);
                    decision->Play();
                    break;

                case  GamePad::BTN_B:

                    moveFlag = true;
                    break;

                }
            }
        }
    }

}



//�`�揈��
void SceneTitle::Render()
{
    /*
    switch (state)
    {
    case State::Title:
        TitleRender();
        break;
    case State::Select:
        SelectRender();
        break;
    }
    */

    
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f,0.0f,0.0f,1.0f };    //RGBA(0.0~1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // �`�揈��
    RenderContext rc;
   // rc.lightDirection = { 0.0f, 0.0f, -1.0f, 0.0f };	// ���C�g�����i�������j
    rc.deviceContext = dc;
    //�J�����p�����[�^�ݒ�
    Camera& camera = Camera::Instance();
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();



    //2D�X�v���C�g�`��
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite_render->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite_render->GetTextureHeight());
        //�^�C�g���X�v���C�g�`��
        sprite_render->Render(dc,
            0, 0, screenWidth, screenHeight,
            0, 0, textureWidth, textureHeight,
            0,
            1, 1, 1, 1);
    }


    // 3D���f���`��
    {
        ModelShader* shader = graphics.GetShader(ModelShaderId::Default);
        shader->Begin(rc);
        
        //backGround->Render(dc, shader);
        //StageManager::Instance().Render(dc, shader);
        StageManager::Instance().ModelRender(rc, shader);
        titleStage0->ModelRender(rc, shader);

        shader->End(rc);

        
    }

    float lookZ= camera.GetEye().z;

#if EDIT
    if (ImGui::Begin("camera", nullptr, ImGuiWindowFlags_None))
    {
        //�g�����X�t�H�[��
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //�ʒu
            ImGui::InputFloat("PositionZ", &lookZ);
            ImGui::InputFloat("time", &time);

            //�Ŕ̈ʒu
            float tPos = titleStage0->GetPosition().y;
            ImGui::InputFloat("titleY", &tPos);
            float s0 = StageManager::Instance().GetStage(0)->GetPosition().x;
            float s1 = StageManager::Instance().GetStage(1)->GetPosition().x;
            float s2 = StageManager::Instance().GetStage(2)->GetPosition().x;
            ImGui::InputFloat("select0X", &s0);
            ImGui::InputFloat("select1X", &s1);
            ImGui::InputFloat("select2X", &s2);
            /*
            ImGui::InputFloat3("title", &titleStage0.z);
            ImGui::InputFloat3("title", &titleStage0.z);
            */

        }
    }
    ImGui::End();

#endif

    //�ς����Ă���Ȃ�

    if (camera.GetEye().z != lookZ&&lookZ!=0)
    {
        camera.SetLookAt(
            DirectX::XMFLOAT3(0, 0, lookZ),
            DirectX::XMFLOAT3(0, 0, 0),
            DirectX::XMFLOAT3(0, 1, 1)
        );
    }

}

//�^�C�g���`�揈��
void SceneTitle::TitleRender()
{

    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };    //RGBA(0.0~1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // �`�揈��
    RenderContext rc;
    //rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

    //�J�����p�����[�^�ݒ�
    Camera& camera = Camera::Instance();
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();

    // 3D���f���`��
    {
        ModelShader* shader = graphics.GetShader(ModelShaderId::Default);
        shader->Begin(rc);

        titleStage0->ModelRender(rc, shader);


        shader->End(rc);
    }
}

//�Z���N�g�`�揈��
void SceneTitle::SelectRender()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 1.0f,0.0f,0.5f,1.0f };    //RGBA(0.0~1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // �`�揈��
    RenderContext rc;
    rc.deviceContext = dc;
    //rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

    LightManager::Instane().PushRenderContext(rc);

    //�J�����p�����[�^�ݒ�
    Camera& camera = Camera::Instance();
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();

    // �X�J�C�{�b�N�X�̕`��
    {
        //SpriteShader* shader = graphics.GetShader(SpriteShaderId::Default);
        //shader->Begin(rc);

        //shader->Draw(rc, sprite.get());

        //shader->End(rc);
    }
    // 3D���f���`��
    {
        //Shader* shader = graphics.GetShader();
        //shader->Begin(dc, rc);

        //StageManager::Instance().Render(dc, shader);

        //shader->End(dc);

        ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
        shader->Begin(rc);

        StageManager::Instance().ModelRender(rc, shader);

        shader->End(rc);
        
    }


}