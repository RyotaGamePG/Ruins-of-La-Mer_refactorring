#pragma once

#include <memory>
#include <wrl.h>
#include "Graphics/Shader.h"

class UVScrollShader : public SpriteShader
{
public:
	UVScrollShader(ID3D11Device* device);
	~UVScrollShader() override {}

	void Begin(const RenderContext& rc) override;
	void Draw(const RenderContext& rc, const Sprite* sprite) override;
	void End(const RenderContext& rc) override;

private:

	struct CbScroll
	{
		DirectX::XMFLOAT2	uvScrollValue;	//	UVスクロール値
		DirectX::XMFLOAT2	dummy;			//	ダミー
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>				scrollConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;
};
