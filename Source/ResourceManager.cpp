#include"Graphics/Graphics.h"
#include "ResourceManager.h"
#include"EnemySlime.h"

//���f�����\�[�X�ǂݍ���
std::shared_ptr<ModelResource> ResourceManager::LoadmodelResource(const char* filename)
{
	// ���f��������
	ModelMap::iterator it = models.find(filename);
	if (it != models.end())
	{
		// �����N�i�����j���؂�Ă��Ȃ����m�F
		if (!it->second.expired())
		{
			// �ǂݍ��ݍς݂̃��f�����\�[�X��Ԃ�
			return it->second.lock();
		}
	}

	// �V�K���f�����\�[�X�쐬���ǂݍ���
	ID3D11Device* device = Graphics::Instance().GetDevice();
	auto model = std::make_shared<ModelResource>();
	model->Load(device, filename);

	// �}�b�v�ɓo�^
	models[filename] = model;

	return model;
}
