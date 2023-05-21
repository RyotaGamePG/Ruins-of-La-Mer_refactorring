#pragma once
#include <wrl.h>
#include <d3d11.h>

// �e�N�X�`��
class Texture
{
public:
	Texture(const char* filename);
	~Texture() {}

	// �V�F�[�_�[���\�[�X�r���[�擾
	inline const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const { return shaderResourceView; }

	// �e�N�X�`���̏��擾
	inline const D3D11_TEXTURE2D_DESC& GetTexture2dDesc() const { return texture2dDesc; }

	// �e�N�X�`�����擾
	inline int GetWidth() const { return texture2dDesc.Width; }

	// �e�N�X�`�������擾
	inline int GetHeight() const { return texture2dDesc.Height; }

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView;
	D3D11_TEXTURE2D_DESC	texture2dDesc;
};
