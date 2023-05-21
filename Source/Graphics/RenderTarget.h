#pragma once

#include <wrl.h>
#include <d3d11.h>

// �`��^�[�Q�b�g�o�b�t�@
class RenderTarget
{
public:
	RenderTarget(UINT width, UINT height, DXGI_FORMAT format);
	~RenderTarget() {}

	// �V�F�[�_�[���\�[�X�r���[�擾
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() { return shaderResourceView; }

	// �����_�[�^�[�Q�b�g�r���[�擾
	const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetRenderTargetView() { return renderTargetView; }

	// �e�N�X�`���̏��擾
	D3D11_TEXTURE2D_DESC GetTexture2dDesc() { return texture2dDesc; }

	// �e�N�X�`�����擾
	inline int GetWidth() const { return texture2dDesc.Width; }

	// �e�N�X�`�������擾
	inline int GetHeight() const { return texture2dDesc.Height; }

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		renderTargetView;
	D3D11_TEXTURE2D_DESC								texture2dDesc;
};
