#pragma once

#include "MeshDirectX.h"
#include "Entity.h"

namespace KatBall
{
	class Kat : public FieaGameEngine::Entity
	{
		RTTI_DECLARATIONS(Kat, FieaGameEngine::Entity)

	public:

		Kat();

		~Kat();

	private:

		FieaGameEngine::MeshDirectX mMesh;
	};

	
}
