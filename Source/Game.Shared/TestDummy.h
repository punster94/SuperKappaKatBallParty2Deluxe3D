#pragma once
#pragma once

#include "MeshDirectX.h"
#include "Entity.h"

namespace KatBall
{
	class TestDummy : public FieaGameEngine::Entity
	{
		RTTI_DECLARATIONS(TestDummy, FieaGameEngine::Entity)

	public:

		TestDummy();

		~TestDummy();

		virtual void Update(FieaGameEngine::WorldState& worldState) override;

	private:

		FieaGameEngine::MeshDirectX mMesh;

		FieaGameEngine::MeshDirectX mOffsetMesh;
	};


}
