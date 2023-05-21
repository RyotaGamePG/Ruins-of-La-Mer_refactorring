#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Graphics/RenderContext.h"
#include "Graphics/Model.h"
#include "Graphics/Sprite.h"

class ModelShader
{
public:
	ModelShader() {}
	virtual ~ModelShader() {}

	// •`‰æŠJn
	virtual void Begin(const RenderContext& rc) = 0;

	// •`‰æ
	virtual void Draw(const RenderContext& rc, const Model* model) = 0;

	// •`‰æI—¹
	virtual void End(const RenderContext& rc) = 0;
};

class SpriteShader
{
public:
	SpriteShader() {}
	virtual ~SpriteShader() {}

	// •`‰æŠJn
	virtual void Begin(const RenderContext& rc) = 0;

	// •`‰æ
	virtual void Draw(const RenderContext& rc, const Sprite* sprite) = 0;

	// •`‰æI—¹
	virtual void End(const RenderContext& rc) = 0;
};
