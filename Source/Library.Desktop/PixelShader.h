#pragma once

#include "Factory.h"
#include "Asset.h"
#include "Renderer.h"
#include <d3d11.h>

namespace FieaGameEngine
{
	class PixelShader final : public Asset
	{

		RTTI_DECLARATIONS(PixelShader, Asset)

	public:

		PixelShader();

		~PixelShader();

		virtual void Load(char* data, std::uint32_t size) override;

		void SetRenderingState(Renderer* renderer);

	private:

		ID3D11PixelShader* mPixelShader;
	};

	ConcreteFactory(Asset, PixelShader);
}