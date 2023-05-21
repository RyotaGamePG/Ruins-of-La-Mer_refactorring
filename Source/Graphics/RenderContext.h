#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

//	平行光源情報
struct DirectionalLightData
{
	DirectX::XMFLOAT4	direction;	// 向き
	DirectX::XMFLOAT4	color;		// 色
};

// 点光源情報
struct PointLightData
{
	DirectX::XMFLOAT4	position;	// 座標
	DirectX::XMFLOAT4	color;		// 色
	float			range;		// 範囲
	DirectX::XMFLOAT3	dummy;
};
// 点光源の最大数
static	constexpr	int	PointLightMax = 8;


// スポットライト情報
struct SpotLightData
{
	DirectX::XMFLOAT4	position;	// 座標
	DirectX::XMFLOAT4	direction;	// 向き
	DirectX::XMFLOAT4	color;		// 色
	float			range;		// 範囲
	float			innerCorn; 	// インナー角度範囲
	float			outerCorn; 	// アウター角度範囲
	float			dummy;
};
// スポットライトの最大数
static	constexpr	int	SpotLightMax = 8;



//	UVスクロール情報
struct UVScrollData
{
	DirectX::XMFLOAT2	uvScrollValue;	// UVスクロール値
};

//マスクデータ
struct MaskData
{
	ID3D11ShaderResourceView* maskTexture;
	float					  dissolveThreshold;
	float					  edgeThreshold;		//縁の閾値
	DirectX::XMFLOAT4		  edgeColor;			//縁の色
};

// 色調補正情報
struct ColorGradingData
{
	float	hueShift = 0;	// 色相調整
	float	saturation = 1.252f;	// 彩度調整
	float	brightness = 0.886f;	// 明度調整
	float	dummy;
};

// ガウスフィルター計算情報
struct GaussianFilterData
{
	int					kernelSize = 15;		// カーネルサイズ
	float				deviation = 9.451;		// 標準偏差
	DirectX::XMFLOAT2	textureSize;			// 暈すテクスチャのサイズ
};
// ガウスフィルターの最大カーネルサイズ
static const int MaxKernelSize = 16;

// 高輝度抽出用情報
struct LuminanceExtractionData
{
	float				threshold = 0.737f;	// 閾値
	float				intensity = 3.846f;	// ブルームの強度
	DirectX::XMFLOAT2	dummy2;
};

// ポストエフェクトの最終パス用情報
struct FinalpassnData
{
	//	ブルームテクスチャ
	ID3D11ShaderResourceView* bloomTexture;
};

//	シャドウマップ用情報
struct ShadowMapData
{
	ID3D11ShaderResourceView* shadowMap;				//	シャドウマップテクスチャ
	DirectX::XMFLOAT4X4			lightViewProjection;	//	ライトビュープロジェクション行列
	DirectX::XMFLOAT3			shadowColor;			//	影の色
	float						shadowBias;				//	深度比較用のオフセット値
};


// レンダーコンテキスト
struct RenderContext
{
	ID3D11DeviceContext* deviceContext;

	//	カメラ情報
	DirectX::XMFLOAT4		viewPosition;
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;

	//	ライト情報
	DirectX::XMFLOAT4		ambientLightColor;
	DirectionalLightData	directionalLightData;
	PointLightData			pointLightData[PointLightMax];	// 点光源情報
	SpotLightData			spotLightData[SpotLightMax];	// スポットライト情報
	int						pointLightCount = 0;			// 点光源数
	int						spotLightCount = 0;				// スポットライト数

	//	スクロールデータ
	UVScrollData			uvScrollData;
	//マスクデータ
	MaskData				maskData;
	//	色調補正情報
	ColorGradingData		colorGradingData;
	//	ガウスフィルター情報
	GaussianFilterData		gaussianFilterData;
	//	高輝度抽出用情報
	LuminanceExtractionData	luminanceExtractionData;
	//	最終パス情報
	FinalpassnData			finalpassnData;
	//	シャドウマップ情報
	ShadowMapData			shadowMapData;

};