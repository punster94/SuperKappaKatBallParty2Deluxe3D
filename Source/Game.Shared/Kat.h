#pragma once

#include "MeshDirectX.h"
#include "Actor.h"

namespace KatBall
{
	class Kat : public FieaGameEngine::Actor
	{
		RTTI_DECLARATIONS(Kat, FieaGameEngine::Actor)

	public:

		Kat();

		~Kat();

	private:

		FieaGameEngine::MeshDirectX mMesh;
		FieaGameEngine::Texture mTexture;
		FieaGameEngine::Shader mShader;
	};

	
}
