#pragma once

#include "Factory.h"
#include "Asset.h"
#include <d3d11.h>

namespace FieaGameEngine
{
	class Texture final : public Asset
	{
		RTTI_DECLARATIONS(Texture, Asset)

	public:

		Texture();

		~Texture();

		virtual void Load(char* data, std::uint32_t size, const std::string& path) override;

		void SetRenderingState(class Renderer* renderer);

	private:

		void LoadPNG(const std::string& path);

		void LoadBMP(char* data, std::uint32_t size);

		void GenerateVertexBuffer(unsigned char* data, std::uint32_t width, std::uint32_t height);

		ID3D11Texture2D* mTexture;
		ID3D11ShaderResourceView* mResourceView;
		ID3D11SamplerState* mSamplerState;
	};

	ConcreteFactory(Asset, Texture);
}
