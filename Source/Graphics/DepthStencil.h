#pragma once

#include <wrl.h>
#include <d3d11.h>

// �[�x�X�e���V���o�b�t�@
class DepthStencil
{
public:
	DepthStencil(UINT width, UINT height);
	~DepthStencil() {}

	// �V�F�[�_�[���\�[�X�r���[�擾
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() { return shaderResourceView; }

	// �[�x�X�e���V���r���[�擾
	const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetDepthStencilView() { return depthStencilView; }

	// �e�N�X�`���̏��擾
	D3D11_TEXTURE2D_DESC GetTexture2dDesc() { return texture2dDesc; }

	// �e�N�X�`�����擾
	inline int GetWidth() const { return texture2dDesc.Width; }

	// �e�N�X�`�������擾
	inline int GetHeight() const { return texture2dDesc.Height; }

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		depthStencilView;
	D3D11_TEXTURE2D_DESC								texture2dDesc;
};
