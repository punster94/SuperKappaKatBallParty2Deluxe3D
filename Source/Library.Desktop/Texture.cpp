#include "pch.h"
#include "Texture.h"
#include "Renderer.h"
#include "RendererDirectX.h"
#include <stdlib.h>
#include <d3d11.h>

#define MAX_TEXTURE_SIZE 2048
#define BYTES_PER_PIXEL 3

using namespace DirectX;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Texture)

	Texture::Texture() :
		mTexture(nullptr),
		mSamplerState(nullptr)
	{

	}

	Texture::~Texture()
	{
		if (mResourceView != nullptr)
		{
			mResourceView->Release();
			mResourceView = nullptr;
		}

		if (mTexture != nullptr)
		{
			mTexture->Release();
			mTexture = nullptr;
		}

		if (mSamplerState != nullptr)
		{
			mSamplerState->Release();
			mSamplerState = nullptr;
		}
	}

	void Texture::Load(char* data, std::uint32_t size)
	{
		size;

		RendererDirectX* renderer = RendererDirectX::Get();
		assert(renderer != nullptr);

		unsigned char* pData = 0;
		unsigned char* pSrc = 0;

		int nWidth = 0;
		int nHeight = 0;
		unsigned short sBPP = 0;

		if (nWidth > MAX_TEXTURE_SIZE ||
			nHeight > MAX_TEXTURE_SIZE)
		{
			// Texture too big.
			assert(true);
		}

		// Grab the dimensions of texture
		nWidth = *reinterpret_cast<int*>(&data[18]);
		nHeight = *reinterpret_cast<int*>(&data[22]);
		sBPP = *reinterpret_cast<short*>(&data[28]);

		if (sBPP == 24)
		{
			pSrc = reinterpret_cast<unsigned char*>(&data[54]);

			pData = new unsigned char[nWidth * nHeight * 4];

			unsigned char* pDst = pData;

			int nPadding = (nWidth * 3) % 4;

			if (nPadding != 0)
				nPadding = 4 - nPadding;

			for (int i = 0; i < nHeight; i++)
			{
				for (int j = 0; j < nWidth; j++)
				{
					pDst[0] = pSrc[2];
					pDst[1] = pSrc[1];
					pDst[2] = pSrc[0];
					pDst[3] = 255;

					pDst += 4;
					pSrc += 3;
				}

				pSrc += nPadding;
			}

			// Create texture object
			D3D11_TEXTURE2D_DESC sTexDesc;
			sTexDesc.Width = nWidth;
			sTexDesc.Height = nHeight;
			sTexDesc.MipLevels = 1;
			sTexDesc.ArraySize = 1;
			sTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sTexDesc.SampleDesc.Count = 1;
			sTexDesc.SampleDesc.Quality = 0;
			sTexDesc.Usage = D3D11_USAGE_IMMUTABLE;
			sTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			sTexDesc.CPUAccessFlags = 0;
			sTexDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA* sSubData = new D3D11_SUBRESOURCE_DATA;
			sSubData->pSysMem = pData;
			sSubData->SysMemPitch = (UINT) nWidth * 4;
			sSubData->SysMemSlicePitch = (UINT)(nWidth * nHeight * 4);

			renderer->Device()->CreateTexture2D(&sTexDesc, sSubData, &mTexture);

			delete sSubData;

			// Create Shader resource view from texture resource
			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
			memset(&SRVDesc, 0, sizeof(SRVDesc));
			SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MipLevels = 1;

			renderer->Device()->CreateShaderResourceView(mTexture, &SRVDesc, &mResourceView);

			// Create sampler in default state
			D3D11_SAMPLER_DESC sampDesc;
			ZeroMemory(&sampDesc, sizeof(sampDesc));
			sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sampDesc.MinLOD = 0;
			sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
			renderer->Device()->CreateSamplerState(&sampDesc, &mSamplerState);

			delete[] pData;
			pData = nullptr;
		}
		else
		{
			// Unsupported bits per pixel!
			assert(true);
		}
	}

	void Texture::SetRenderingState(Renderer* renderer)
	{
		RendererDirectX* directX = static_cast<RendererDirectX*>(renderer);

		directX->Context()->PSSetShaderResources(0, 1, &mResourceView);
		directX->Context()->PSSetSamplers(0, 1, &mSamplerState);
	}
}