#pragma once

#include <memory>
#include <wrl.h>
#include "Graphics/Shader.h"

class ColorGradingShader : public SpriteShader
{
public:
	ColorGradingShader(ID3D11Device* device);
	~ColorGradingShader() override {}

	void Begin(const RenderContext& rc) override;
	void Draw(const RenderContext& rc, const Sprite* sprite) override;
	void End(const RenderContext& rc) override;

private:
	struct CBColorGrading
	{
		float	hueShift;	// êFëäí≤êÆ
		float	saturation;	// ç ìxí≤êÆ
		float	brightness;	// ñæìxí≤êÆ
		float	dummy;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>				colorGradingConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;
};
