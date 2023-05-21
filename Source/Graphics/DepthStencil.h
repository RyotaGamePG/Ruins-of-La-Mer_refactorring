#pragma once

#include <wrl.h>
#include <d3d11.h>

// 深度ステンシルバッファ
class DepthStencil
{
public:
	DepthStencil(UINT width, UINT height);
	~DepthStencil() {}

	// シェーダーリソースビュー取得
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() { return shaderResourceView; }

	// 深度ステンシルビュー取得
	const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetDepthStencilView() { return depthStencilView; }

	// テクスチャの情報取得
	D3D11_TEXTURE2D_DESC GetTexture2dDesc() { return texture2dDesc; }

	// テクスチャ幅取得
	inline int GetWidth() const { return texture2dDesc.Width; }

	// テクスチャ高さ取得
	inline int GetHeight() const { return texture2dDesc.Height; }

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		depthStencilView;
	D3D11_TEXTURE2D_DESC								texture2dDesc;
};
