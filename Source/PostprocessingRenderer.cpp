#include "PostprocessingRenderer.h"
#include "Graphics/Graphics.h"

PostprocessingRenderer::PostprocessingRenderer()
{
	Graphics& graphics = Graphics::Instance();

	renderSprite = std::make_unique<Sprite>();

	{
		UINT width = static_cast<UINT>(graphics.GetScreenWidth()) / 2;
		UINT height = static_cast<UINT>(graphics.GetScreenHeight()) / 2;

		// ���P�x���o�p�`��^�[�Q�b�g�𐶐�
		luminanceExtractRenderTarget = std::make_unique<RenderTarget>(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);

		// ���P�x���o�ڂ����p�`��^�[�Q�b�g�𐶐�
		luminanceExtractBokehRenderTarget = std::make_unique<RenderTarget>(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
		bloomData.gaussianFilterData.textureSize.x = static_cast<float>(luminanceExtractBokehRenderTarget->GetWidth());
		bloomData.gaussianFilterData.textureSize.y = static_cast<float>(luminanceExtractBokehRenderTarget->GetHeight());
	}


}

PostprocessingRenderer::~PostprocessingRenderer()
{
}

void PostprocessingRenderer::Render(ID3D11DeviceContext* deviceContext)
{
	Graphics& graphics = Graphics::Instance();

	// ���ݐݒ肳��Ă���o�b�t�@��ޔ����ď��������Ă���
	UINT			cachedViewportCount{ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE };
	D3D11_VIEWPORT	cachedViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	cachedRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	cachedDepthStencilView;
	{
		deviceContext->RSGetViewports(&cachedViewportCount, cachedViewports);
		deviceContext->OMGetRenderTargets(1
			, cachedRenderTargetView.ReleaseAndGetAddressOf()
			, cachedDepthStencilView.ReleaseAndGetAddressOf());
	}

	RenderContext	rc;
	rc.deviceContext = deviceContext;
	rc.luminanceExtractionData = bloomData.luminanceExtractionData;
	rc.gaussianFilterData = bloomData.gaussianFilterData;

	//	���P�x���o�p�o�b�t�@�ɕ`����ύX���č��P�x���o
	{
		//	�`����ύX
		ID3D11RenderTargetView* rtv = luminanceExtractRenderTarget->GetRenderTargetView().Get();
		FLOAT color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		rc.deviceContext->ClearRenderTargetView(rtv, color);
		rc.deviceContext->OMSetRenderTargets(1, &rtv, nullptr);
		D3D11_VIEWPORT	viewport{};
		viewport.Width = static_cast<float>(luminanceExtractRenderTarget->GetWidth());
		viewport.Height = static_cast<float>(luminanceExtractRenderTarget->GetHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		rc.deviceContext->RSSetViewports(cachedViewportCount, cachedViewports);

		//	���P�x���o����
		SpriteShader* shader = graphics.GetShader(SpriteShaderId::LuminanceExtraction);
		shader->Begin(rc);

		//	�`��Ώۂ�ύX
		renderSprite->SetShaderResourceView(sceneData.srv, sceneData.width, sceneData.height);
		renderSprite->Update(0, 0, viewport.Width, viewport.Height,
			0, 0, static_cast<float>(sceneData.width), static_cast<float>(sceneData.height),
			0,
			1, 1, 1, 1);
		shader->Draw(rc, renderSprite.get());
		shader->End(rc);
	}

	// ���o�������P�x�`��Ώۂ�򂵂ď�������
	{
		//	�`����ύX
		ID3D11RenderTargetView* rtv = luminanceExtractBokehRenderTarget->GetRenderTargetView().Get();
		FLOAT color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		rc.deviceContext->ClearRenderTargetView(rtv, color);
		rc.deviceContext->OMSetRenderTargets(1, &rtv, nullptr);
		D3D11_VIEWPORT	viewport{};
		viewport.Width = static_cast<float>(luminanceExtractBokehRenderTarget->GetWidth());
		viewport.Height = static_cast<float>(luminanceExtractBokehRenderTarget->GetHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		rc.deviceContext->RSSetViewports(cachedViewportCount, cachedViewports);

		//	���P�x���o����
		SpriteShader* shader = graphics.GetShader(SpriteShaderId::GaussianBlur);
		shader->Begin(rc);

		//	�`��Ώۂ�ύX
		renderSprite->SetShaderResourceView(luminanceExtractRenderTarget->GetShaderResourceView(),
			luminanceExtractRenderTarget->GetWidth(),
			luminanceExtractRenderTarget->GetHeight());
		renderSprite->Update(0, 0, viewport.Width, viewport.Height,
			0, 0, static_cast<float>(luminanceExtractRenderTarget->GetWidth()), static_cast<float>(luminanceExtractRenderTarget->GetHeight()),
			0,
			1, 1, 1, 1);
		shader->Draw(rc, renderSprite.get());
		shader->End(rc);
	}


	//	���̃o�b�t�@�ɖ߂�
	{
		deviceContext->RSSetViewports(cachedViewportCount, cachedViewports);
		deviceContext->OMSetRenderTargets(1, cachedRenderTargetView.GetAddressOf(), cachedDepthStencilView.Get());
	}

	// �ŏI�p�X
	{
		//	���P�x���o����
		SpriteShader* shader = graphics.GetShader(SpriteShaderId::Finalpass);
		shader->Begin(rc);

		//	�`��Ώۂ�ύX
		renderSprite->SetShaderResourceView(sceneData.srv, sceneData.width, sceneData.height);
		renderSprite->Update(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
			0, 0, static_cast<float>(sceneData.width), static_cast<float>(sceneData.height),
			0,
			1, 1, 1, 1);

		//	�V�F�[�_�[�ɓn���e�N�X�`����ݒ�
		rc.finalpassnData.bloomTexture = luminanceExtractBokehRenderTarget->GetShaderResourceView().Get();
		rc.colorGradingData = colorGradingData;

		shader->Draw(rc, renderSprite.get());

		shader->End(rc);
	}
}

//	�V�[�����̐ݒ�
void PostprocessingRenderer::SetSceneData(ShaderResourceViewData srvData)
{
	sceneData = srvData;
}

//	�f�o�b�O���̕\��
void PostprocessingRenderer::DrawDebugGUI()
{
	if (ImGui::TreeNode("PostProcess"))
	{
		if (ImGui::TreeNode("Bloom"))
		{
			ImGui::SliderFloat("threshold", &bloomData.luminanceExtractionData.threshold, 0.0f, 1.0f);
			ImGui::SliderFloat("intensity", &bloomData.luminanceExtractionData.intensity, 0.0f, 10.0f);
			ImGui::SliderInt("kernelSize", &bloomData.gaussianFilterData.kernelSize, 1, MaxKernelSize - 1);
			ImGui::SliderFloat("deviation", &bloomData.gaussianFilterData.deviation, 1.0f, 10.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ColorGrading"))
		{
			ImGui::SliderFloat("hueShift", &colorGradingData.hueShift, 0.0f, 360.0f);
			ImGui::SliderFloat("saturation", &colorGradingData.saturation, 0.0f, 2.0f);
			ImGui::SliderFloat("brightness", &colorGradingData.brightness, 0.0f, 2.0f);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}
