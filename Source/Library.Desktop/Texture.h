#pragma once

#include "Factory.h"
#include "Asset.h"

namespace FieaGameEngine
{
	class Texture final : public Asset
	{
		RTTI_DECLARATIONS(Texture, Asset)

	public:

		Texture();

		~Texture();

		virtual void Load(char* data) override;

		void SetRenderingState(class Renderer* renderer);

	};

	ConcreteFactory(Asset, Texture);
}
