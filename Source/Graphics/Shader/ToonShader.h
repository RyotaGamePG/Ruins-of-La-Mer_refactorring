#pragma once

#include <memory>
#include <wrl.h>
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

class ToonShader : public ModelShader
{
public:
	ToonShader(ID3D11Device* device);
	~ToonShader() override {}

	void Begin(const RenderContext& rc) override;
	void Draw(const RenderContext& rc, const Model* model) override;
	void End(const RenderContext& rc) override;

private:
	// 輪郭線用の初期化処理
	void	InitializeOutline(ID3D11Device* device);

	static const int MaxBones = 128;

	struct CbScene
	{
		DirectX::XMFLOAT4		viewPosition;
		DirectX::XMFLOAT4X4		viewProjection;
		DirectX::XMFLOAT4		ambientLightColor;
		DirectionalLightData	directionalLightData;
		PointLightData			pointLightData[PointLightMax];	// 点光源
		SpotLightData			spotLightData[SpotLightMax];	// スポットライト
		int						pointLightCount;				// 点光源数
		int						spotLightCount;					// スポットライト数
		DirectX::XMFLOAT2		dummy;
	};

	struct CbMesh
	{
		DirectX::XMFLOAT4X4	boneTransforms[MaxBones];
	};

	struct CbSubset
	{
		DirectX::XMFLOAT4	materialColor;
	};

	struct CbOutline
	{
		DirectX::XMFLOAT4X4		viewProjection;
		DirectX::XMFLOAT4		outlineColor;
		float					outlineSize;
		DirectX::XMFLOAT3		dummy;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			outlineConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		outlineVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		outlinePixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		outlineInputLayout;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	outlineRasterizerState;

	Microsoft::WRL::ComPtr<ID3D11Buffer>			sceneConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			meshConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subsetConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerState;
	std::unique_ptr<Texture>						toontexture;	// デフォルトのトゥーン用ランプテクスチャ
};
