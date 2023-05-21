#pragma once

#include <vector>
#include "Light.h"

#include "Graphics/RenderContext.h"

// ���C�g�Ǘ��N���X
class LightManager
{
	LightManager() {}
	~LightManager();

public:
	static LightManager& Instane()
	{
		static	LightManager	lightManager;
		return	lightManager;
	}

	// ���C�g��o�^����
	void Register(Light* light);

	// ���C�g�̓o�^����������
	void Remove(Light* light);

	// �o�^�ς݂̃��C�g��S�폜����
	void Clear();

	// ���C�g����RenderContext�ɐς�
	void PushRenderContext(RenderContext& rc);

	// �f�o�b�O���̕\��
	void DrawDebugGUI();

	// �f�o�b�O�v���~�e�B�u�̕\��
	void DrawDebugPrimitive();

	// ����
	DirectX::XMFLOAT4	GetAmbientColor() { return	ambientColor; }
	void	SetAmbientColor(DirectX::XMFLOAT4 ambientColor) { this->ambientColor = ambientColor; }

	// ���C�g�̏��
	int GetLightCount() const { return static_cast<int>(lights.size()); }
	Light* GetLight(int index) const { return lights.at(index); }

private:
	std::vector<Light*>	lights;
	DirectX::XMFLOAT4	ambientColor = { 0.004, 0.004, 0.004, 1.0f };
};