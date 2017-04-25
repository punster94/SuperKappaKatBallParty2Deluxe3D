#pragma once


#include "Factory.h"
#include "Asset.h"
#include "Renderer.h"
#include <d3d11.h>

namespace FieaGameEngine
{
	class VertexShader final : public Asset
	{

		RTTI_DECLARATIONS(VertexShader, Asset)

	public:

		VertexShader();

		~VertexShader();

		virtual void Load(char* data, std::uint32_t size) override;

		void SetRenderingState(Renderer* renderer);

		const void* GetBinaryData() const;

		std::uint32_t GetBinarySize() const;

	private:

		ID3D11VertexShader* mVertexShader;

		char* mBinaryData;

		std::uint32_t mBinarySize;
	};

	ConcreteFactory(Asset, VertexShader);
}