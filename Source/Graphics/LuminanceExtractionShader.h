#pragma once

#include <memory>
#include <wrl.h>
#include "Graphics/Shader.h"

class LuminanceExtractionShader : public SpriteShader
{
public:
	LuminanceExtractionShader(ID3D11Device* device);
	~LuminanceExtractionShader() override {}

	void Begin(const RenderContext& rc) override;
	void Draw(const RenderContext& rc, const Sprite* sprite) override;
	void End(const RenderContext& rc) override;

private:
	struct CBLuminanceExtraction
	{
		float				threshold;	// 閾値
		float				intensity;	// ブルームの強度
		DirectX::XMFLOAT2	dummy2;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>				luminanceExtractionConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;
};
