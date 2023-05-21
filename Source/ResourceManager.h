#pragma once

#include<memory>
#include<string>
#include<map>
#include"Graphics/ModelResource.h"


//リソースマネージャー
class ResourceManager
{
private:
    ResourceManager() {}
    ~ResourceManager() {}

public:
    //唯一のインスタンス取得
    static ResourceManager& Instance()
    {
        static ResourceManager instance;
        return instance;
    }

    //モデルリソース読み込み
    std::shared_ptr<ModelResource> LoadmodelResource(const char* filename);

private:
    using ModelMap = std::map<std::string, std::weak_ptr<ModelResource>>;

    ModelMap models;
};