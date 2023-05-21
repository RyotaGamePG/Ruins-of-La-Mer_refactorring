#include "Misc.h"
#include "Graphics/Graphics.h"

#include "Graphics/DefaultModelShader.h"
#include "Graphics/DefaultSpriteShader.h"

#include "Graphics/UVScrollShader.h"

//#include "Graphics/MaskShader.h"
#include "Graphics/Shader/PhongShader.h"
#include "Graphics/Shader/ToonShader.h"
#include "Graphics/ColorGradingShader.h"
#include "Graphics/GaussianBlurShader.h"
#include "Graphics/LuminanceExtractionShader.h"
#include "Graphics/FinalpassShader.h"
#include "Graphics/ShadowmapCasterShader.h"
#include "Graphics/SkyboxShader.h"

Graphics* Graphics::instance = nullptr;

// �R���X�g���N�^
Graphics::Graphics(HWND hWnd)
{
	// �C���X�^���X�ݒ�
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;

	// ��ʂ̃T�C�Y���擾����B
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT screenWidth = rc.right - rc.left;
	UINT screenHeight = rc.bottom - rc.top;

	this->screenWidth = static_cast<float>(screenWidth);
	this->screenHeight = static_cast<float>(screenHeight);

	HRESULT hr = S_OK;

	// �f�o�C�X���X���b�v�`�F�[���̐���
	{
		UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		// �X���b�v�`�F�[�����쐬���邽�߂̐ݒ�I�v�V����
		DXGI_SWAP_CHAIN_DESC swapchainDesc;
		{
			swapchainDesc.BufferDesc.Width = screenWidth;
			swapchainDesc.BufferDesc.Height = screenHeight;
			swapchainDesc.BufferDesc.RefreshRate.Numerator = 60;
			swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
			swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 1�s�N�Z��������̊e�F(RGBA)��8bit(0�`255)�̃e�N�X�`��(�o�b�N�o�b�t�@)���쐬����B
			swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			swapchainDesc.SampleDesc.Count = 1;
			swapchainDesc.SampleDesc.Quality = 0;
			swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapchainDesc.BufferCount = 1;		// �o�b�N�o�b�t�@�̐�
			swapchainDesc.OutputWindow = hWnd;	// DirectX�ŕ`�������\������E�C���h�E
			swapchainDesc.Windowed = TRUE;		// �E�C���h�E���[�h���A�t���X�N���[���ɂ��邩�B
			swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapchainDesc.Flags = 0; // DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
		}

		D3D_FEATURE_LEVEL featureLevel;

		// �f�o�C�X���X���b�v�`�F�[���̐���
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,						// �ǂ̃r�f�I�A�_�v�^���g�p���邩�H����Ȃ��nullptr�ŁAIDXGIAdapter�̃A�h���X��n���B
			D3D_DRIVER_TYPE_HARDWARE,		// �h���C�o�̃^�C�v��n���BD3D_DRIVER_TYPE_HARDWARE �ȊO�͊�{�I�Ƀ\�t�g�E�F�A�����ŁA���ʂȂ��Ƃ�����ꍇ�ɗp����B
			nullptr,						// ��L��D3D_DRIVER_TYPE_SOFTWARE�ɐݒ肵���ۂɁA���̏������s��DLL�̃n���h����n���B����ȊO���w�肵�Ă���ۂɂ͕K��nullptr��n���B
			createDeviceFlags,				// ���炩�̃t���O���w�肷��B�ڂ�����D3D11_CREATE_DEVICE�񋓌^�Ō����B
			featureLevels,					// D3D_FEATURE_LEVEL�񋓌^�̔z���^����Bnullptr�ɂ��邱�Ƃł���Lfeature�Ɠ����̓��e�̔z�񂪎g�p�����B
			ARRAYSIZE(featureLevels),		// featureLevels�z��̗v�f����n���B
			D3D11_SDK_VERSION,				// SDK�̃o�[�W�����B�K�����̒l�B
			&swapchainDesc,					// �����Őݒ肵���\���̂ɐݒ肳��Ă���p�����[�^��SwapChain���쐬�����B
			swapchain.GetAddressOf(),		// �쐬�����������ꍇ�ɁASwapChain�̃A�h���X���i�[����|�C���^�ϐ��ւ̃A�h���X�B�����Ŏw�肵���|�C���^�ϐ��o�R��SwapChain�𑀍삷��B
			device.GetAddressOf(),			// �쐬�����������ꍇ�ɁADevice�̃A�h���X���i�[����|�C���^�ϐ��ւ̃A�h���X�B�����Ŏw�肵���|�C���^�ϐ��o�R��Device�𑀍삷��B
			&featureLevel,					// �쐬�ɐ�������D3D_FEATURE_LEVEL���i�[���邽�߂�D3D_FEATURE_LEVEL�񋓌^�ϐ��̃A�h���X��ݒ肷��B
			immediateContext.GetAddressOf()	// �쐬�����������ꍇ�ɁAContext�̃A�h���X���i�[����|�C���^�ϐ��ւ̃A�h���X�B�����Ŏw�肵���|�C���^�ϐ��o�R��Context�𑀍삷��B
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �����_�[�^�[�Q�b�g�r���[�̐���
	{
		// �X���b�v�`�F�[������o�b�N�o�b�t�@�e�N�X�`�����擾����B
		// ���X���b�v�`�F�[���ɓ����Ă���o�b�N�o�b�t�@�e�N�X�`����'�F'���������ރe�N�X�`���B
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// �o�b�N�o�b�t�@�e�N�X�`���ւ̏������݂̑����ƂȂ郌���_�[�^�[�Q�b�g�r���[�𐶐�����B
		hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �[�x�X�e���V���r���[�̐���
	{
		// �[�x�X�e���V�������������ނ��߂̃e�N�X�`�����쐬����B
		D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
		depthStencilBufferDesc.Width = screenWidth;
		depthStencilBufferDesc.Height = screenHeight;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.ArraySize = 1;
		depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// 1�s�N�Z��������A�[�x����24Bit / �X�e���V������8bit�̃e�N�X�`�����쐬����B
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;		// �[�x�X�e���V���p�̃e�N�X�`�����쐬����B
		depthStencilBufferDesc.CPUAccessFlags = 0;
		depthStencilBufferDesc.MiscFlags = 0;
		hr = device->CreateTexture2D(&depthStencilBufferDesc, nullptr, depthStencilBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// �[�x�X�e���V���e�N�X�`���ւ̏������݂ɑ����ɂȂ�[�x�X�e���V���r���[���쐬����B
		hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, depthStencilView.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �r���[�|�[�g�̐ݒ�
	{
		// ��ʂ̂ǂ̗̈��DirectX�ŕ`�������\�����邩�̐ݒ�B
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(screenWidth);
		viewport.Height = static_cast<float>(screenHeight);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		immediateContext->RSSetViewports(1, &viewport);
	}

	// ���f���V�F�[�_�[
	{
		modelShaders[static_cast<int>(ModelShaderId::Default)] = std::make_unique<DefaultModelShader>(device.Get());
		modelShaders[static_cast<int>(ModelShaderId::Phong)] = std::make_unique<PhongShader>(device.Get());
		//modelShaders[static_cast<int>(ModelShaderId::Toon)] = std::make_unique<ToonShader>(device.Get());		//�G���[������s�s��
		modelShaders[static_cast<int>(ModelShaderId::ShadowmapCaster)] = std::make_unique<ShadowmapCasterShader>(device.Get());
	}

	// �X�v���C�g�V�F�[�_�[
	{
		spriteShaders[static_cast<int>(SpriteShaderId::Default)] = std::make_unique<DefaultSpriteShader>(device.Get());
		spriteShaders[static_cast<int>(SpriteShaderId::UVScroll)] = std::make_unique<UVScrollShader>(device.Get());
		//spriteShaders[static_cast<int>(SpriteShaderId::Mask)] = std::make_unique<MaskShader>(device.Get());
		spriteShaders[static_cast<int>(SpriteShaderId::ColorGrading)] = std::make_unique<ColorGradingShader>(device.Get());
		spriteShaders[static_cast<int>(SpriteShaderId::GaussianBlur)] = std::make_unique<GaussianBlurShader>(device.Get());
		spriteShaders[static_cast<int>(SpriteShaderId::LuminanceExtraction)] = std::make_unique<LuminanceExtractionShader>(device.Get());
		spriteShaders[static_cast<int>(SpriteShaderId::Finalpass)] = std::make_unique<FinalpassShader>(device.Get());
		spriteShaders[static_cast<int>(SpriteShaderId::Skybox)] = std::make_unique<SkyboxShader>(device.Get());
	}

	// �����_��
	{
		debugRenderer = std::make_unique<DebugRenderer>(device.Get());
		lineRenderer = std::make_unique<LineRenderer>(device.Get(), 1024);
		imguiRenderer = std::make_unique<ImGuiRenderer>(hWnd, device.Get());
	}
}

// �f�X�g���N�^
Graphics::~Graphics()
{
}
