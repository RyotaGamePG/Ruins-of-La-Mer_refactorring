#include "FlagArea.h"
#include "Graphics/Graphics.h"


//�R���X�g���N�^
FlagArea::FlagArea(
    DirectX::XMFLOAT3   posRU,
    DirectX::XMFLOAT3   posLD,
    Type type)
    :posRU(posRU),posLD(posLD),type(type)
{

}

FlagArea::~FlagArea()
{


}

void FlagArea::Update(float elapsedTime)
{
    
}

void FlagArea::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    ////�Փ˔���p�̃f�o�b�O����`��
    float r, g, b;
    r = g = b = 0.0f;
    if (type == Type::SpeedUp)r = 1.0f;
    if (type == Type::Goal)b = 1.0f;
    debugRenderer->DrawSphere(posRU, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    debugRenderer->DrawSphere(posLD, radius, DirectX::XMFLOAT4(r, g, b, 1));

#if EDIT

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("FlagArea", nullptr, ImGuiWindowFlags_None))
    {
        //�g�����X�t�H�[��
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //�ʒu
            ImGui::InputFloat3("PosRU", &posRU.x);
            ImGui::InputFloat3("PosLD", &posLD.x);
            //�^�C�v
            int t = (int)type;
            ImGui::InputInt("Type", &t);
        }
    }
    ImGui::End();
#endif
}



bool FlagArea::OverlapArea(DirectX::XMFLOAT3 position) {
    if (posLD.z <= position.z && position.z <= posRU.z &&
        posRU.x <= position.x && position.x <= posLD.x)
    {
        return true;
    }
    return false;
}