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
		// Attempt to load any required assets
		Asset::Load(ASSET_DIRECTORY_MESHES MESH_KAT, MESH_KAT, Asset::TYPE_MESH);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_KAT, TEXTURE_KAT, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_SHADERS SHADER_MESH_VERTEX, SHADER_MESH_VERTEX, Asset::TYPE_VERTEX_SHADER);
		Asset::Load(ASSET_DIRECTORY_SHADERS SHADER_MESH_PIXEL, SHADER_MESH_PIXEL, Asset::TYPE_PIXEL_SHADER);


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
