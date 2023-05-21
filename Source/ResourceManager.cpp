#include"Graphics/Graphics.h"
#include "ResourceManager.h"
#include"EnemySlime.h"

//モデルリソース読み込み
std::shared_ptr<ModelResource> ResourceManager::LoadmodelResource(const char* filename)
{
	// モデルを検索
	ModelMap::iterator it = models.find(filename);
	if (it != models.end())
	{
		// リンク（寿命）が切れていないか確認
		if (!it->second.expired())
		{
			// 読み込み済みのモデルリソースを返す
			return it->second.lock();
		}
	}

	// 新規モデルリソース作成＆読み込み
	ID3D11Device* device = Graphics::Instance().GetDevice();
	auto model = std::make_shared<ModelResource>();
	model->Load(device, filename);

	// マップに登録
	models[filename] = model;

	return model;
}
