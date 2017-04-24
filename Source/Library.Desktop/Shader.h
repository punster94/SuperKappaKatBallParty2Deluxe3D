#pragma once

#include "Factory.h"
#include "Asset.h"
#include "Renderer.h"
#include <d3d11.h>

namespace FieaGameEngine
{
	class Shader final : public Asset
	{

		RTTI_DECLARATIONS(Shader, Asset)

	public:

		Shader();

		~Shader();

		virtual void Load(char* data) override;

		void SetRenderingState(Renderer* renderer);

	private:

		ID3D11VertexShader* mVertexShader;
		ID3D11PixelShader* mPixelShader;
	};

	ConcreteFactory(Asset, Shader);
}