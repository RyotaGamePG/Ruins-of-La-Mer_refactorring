#pragma once

#include<memory>
#include<string>
#include<map>
#include"Graphics/ModelResource.h"


//���\�[�X�}�l�[�W���[
class ResourceManager
{
private:
    ResourceManager() {}
    ~ResourceManager() {}

public:
    //�B��̃C���X�^���X�擾
    static ResourceManager& Instance()
    {
        static ResourceManager instance;
        return instance;
    }

    //���f�����\�[�X�ǂݍ���
    std::shared_ptr<ModelResource> LoadmodelResource(const char* filename);

private:
    using ModelMap = std::map<std::string, std::weak_ptr<ModelResource>>;

    ModelMap models;
};