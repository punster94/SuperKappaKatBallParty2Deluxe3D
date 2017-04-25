#include "pch.h"
#include "Kat.h"
#include "Constants.h"
#include "Asset.h"

using namespace FieaGameEngine;

namespace KatBall
{
	RTTI_DEFINITIONS(Kat)

	Kat::Kat()
	{
		mMesh.SetOwner(*this);
		mMesh.SetMeshGeometry(Asset::Get(MESH_KAT)->As<MeshGeometry>());
		mMesh.SetTexture(Asset::Get(TEXTURE_KAT)->As<Texture>());
		mMesh.SetShaders(Asset::Get(SHADER_MESH_VERTEX)->As<VertexShader>(),
						 Asset::Get(SHADER_MESH_PIXEL)->As<PixelShader>());
		AddRenderable(mMesh);
	}

	Kat::~Kat()
	{

	}
}
