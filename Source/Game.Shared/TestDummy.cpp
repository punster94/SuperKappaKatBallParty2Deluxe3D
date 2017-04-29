#include "pch.h"
#include "Constants.h"
#include "Asset.h"
#include "TestDummy.h"
#include "WorldState.h"

using namespace FieaGameEngine;

namespace KatBall
{
	RTTI_DEFINITIONS(TestDummy)

	TestDummy::TestDummy()
	{
		mMesh.SetOwner(*this);
		mMesh.SetMeshGeometry(Asset::Get(MESH_CUBE)->As<MeshGeometry>());
		mMesh.SetTexture(Asset::Get(TEXTURE_MANKEY_BALL_PNG)->As<Texture>());
		mMesh.SetShaders(Asset::Get(SHADER_MESH_VERTEX)->As<VertexShader>(),
			Asset::Get(SHADER_MESH_PIXEL)->As<PixelShader>());
		AddRenderable(mMesh);

		mOffsetMesh.SetOwner(*this);
		mOffsetMesh.SetMeshGeometry(Asset::Get(MESH_KAT)->As<MeshGeometry>());
		mOffsetMesh.SetTexture(Asset::Get(TEXTURE_KAT)->As<Texture>());
		mOffsetMesh.SetShaders(Asset::Get(SHADER_MESH_VERTEX)->As<VertexShader>(),
			Asset::Get(SHADER_MESH_PIXEL)->As<PixelShader>());
		AddRenderable(mOffsetMesh);

		mOffsetMesh.SetRelativePosition(glm::vec3(4.0f, 3.0f, 2.0f));
	}

	TestDummy::~TestDummy()
	{

	}

	void TestDummy::Update(WorldState& worldState)
	{
		float deltaTime = worldState.DeltaTime();

		const float offsetMeshRotSpeed = 4.0f;
		const float entityRotationSpeed = 0.3f;

		mOffsetMesh.AddRelativeRotation(glm::vec3(0.0f, offsetMeshRotSpeed * deltaTime, 0.0f));
		
		Rotate(glm::vec3(0.0f, entityRotationSpeed * deltaTime, 0.0f));
	}
}
