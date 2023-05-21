#pragma once

#include <memory>
#include <wrl.h>
#include "Graphics/Shader.h"

class SkyboxShader : public SpriteShader
{
public:
	SkyboxShader(ID3D11Device* device);
	~SkyboxShader() override {}

	void Begin(const RenderContext& rc) override;
	void Draw(const RenderContext& rc, const Sprite* sprite) override;
	void End(const RenderContext& rc) override;

private:

	struct CbScene
	{
		DirectX::XMFLOAT4	viewPosition;
		DirectX::XMFLOAT4X4	inverseView;
		DirectX::XMFLOAT4X4	inverseProjection;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>				sceneConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;
};
