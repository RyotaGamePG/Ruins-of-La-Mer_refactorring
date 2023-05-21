#pragma once


#include <memory>
#include <wrl.h>
#include "Graphics/Shader.h"

class GaussianBlurShader : public SpriteShader
{
	struct CBFilter
	{
		DirectX::XMFLOAT4	weights[MaxKernelSize * MaxKernelSize];
		float	kernelSize;
		DirectX::XMFLOAT2	texcel;
		float	dummy;
	};

public:
	GaussianBlurShader(ID3D11Device* device);
	~GaussianBlurShader() override {}

	void Begin(const RenderContext& rc) override;
	void Draw(const RenderContext& rc, const Sprite* sprite) override;
	void End(const RenderContext& rc) override;

private:
	// フィルター値計算
	void CalcGaussianFilter(CBFilter& cbFilter, const GaussianFilterData& gaussianFilterData);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>				filterConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;
};
