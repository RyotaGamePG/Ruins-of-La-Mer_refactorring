#include <stdlib.h>
#include "Graphics/Graphics.h"
#include "Graphics/Texture.h"
#include <WICTextureLoader.h>

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Misc.h"
#include "Logger.h"

Texture::Texture(const char* filename)
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	// �p�X�𕪉�
	char drive[256], dirname[256], fname[256], ext[256];
	::_splitpath_s(filename, drive, sizeof(drive), dirname, sizeof(dirname), fname, sizeof(fname), ext, sizeof(ext));

	// �p�X������
	char filepath[256];
	::_makepath_s(filepath, 256, drive, dirname, fname, ext);

	// �}���`�o�C�g�������烏�C�h�����֕ϊ�
	wchar_t wfilepath[256];
	::MultiByteToWideChar(CP_ACP, 0, filepath, -1, wfilepath, 256);

	// �e�N�X�`���ǂݍ���
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	HRESULT hr = DirectX::CreateWICTextureFromFile(device, wfilepath, resource.GetAddressOf(), shaderResourceView.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		// WIC�ŃT�|�[�g����Ă��Ȃ��t�H�[�}�b�g�̏ꍇ�iTGA�Ȃǁj��
		// STB�ŉ摜�ǂݍ��݂����ăe�N�X�`���𐶐�����
		int width, height, bpp;
		unsigned char* pixels = stbi_load(filepath, &width, &height, &bpp, STBI_rgb_alpha);
		if (pixels != nullptr)
		{
			D3D11_TEXTURE2D_DESC desc = { 0 };
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			D3D11_SUBRESOURCE_DATA data;
			::memset(&data, 0, sizeof(data));
			data.pSysMem = pixels;
			data.SysMemPitch = width * 4;

			Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture2d;
			hr = device->CreateTexture2D(&desc, &data, texture2d.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

			hr = device->CreateShaderResourceView(texture2d.Get(), nullptr, shaderResourceView.ReleaseAndGetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

			// ��n��
			stbi_image_free(pixels);
		}
	}
	if (SUCCEEDED(hr))
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture2d;
		hr = resource.Get()->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		texture2d->GetDesc(&texture2dDesc);
	}
}

