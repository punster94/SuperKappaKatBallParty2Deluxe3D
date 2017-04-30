#include "pch.h"

using namespace FieaGameEngine;

namespace KatBall
{
	RTTI_DEFINITIONS(MeshEntity)

	MeshEntity::MeshEntity(const std::string& name) :
		Entity(name), mMeshFileName(MESH_KAT), mTextureFileName(TEXTURE_KAT),
		mVertexShaderFileName(SHADER_MESH_VERTEX), mPixelShaderFileName(SHADER_MESH_PIXEL)
	{
		InitializeSignatures();
	}

	MeshEntity::MeshEntity(const MeshEntity& otherMeshEntity) :
		Entity(otherMeshEntity)
	{
		CopyPrivateDataMembers(otherMeshEntity);
	}

	void MeshEntity::Initialize(FieaGameEngine::WorldState& worldState)
	{
		Entity::Initialize(worldState);

		mMesh.SetOwner(*this);

		if (Asset::Get(mMeshFileName) == nullptr)
		{
			Asset::Load(ASSET_DIRECTORY_MESHES + mMeshFileName, mMeshFileName, Asset::TYPE_MESH);
		}

		mMesh.SetMeshGeometry(Asset::Get(mMeshFileName)->As<MeshGeometry>());

		if (Asset::Get(mTextureFileName) == nullptr)
		{
			Asset::Load(mTextureFileName, mTextureFileName, Asset::TYPE_TEXTURE);
		}

		mMesh.SetTexture(Asset::Get(mTextureFileName)->As<Texture>());

		if (Asset::Get(mVertexShaderFileName) == nullptr)
		{
			Asset::Load(mVertexShaderFileName, mVertexShaderFileName, Asset::TYPE_VERTEX_SHADER);
		}

		if (Asset::Get(mPixelShaderFileName) == nullptr)
		{
			Asset::Load(mPixelShaderFileName, mPixelShaderFileName, Asset::TYPE_PIXEL_SHADER);
		}

		mMesh.SetShaders(Asset::Get(mVertexShaderFileName)->As<VertexShader>(),
			Asset::Get(mPixelShaderFileName)->As<PixelShader>());

		AddRenderable(mMesh);
	}

	Scope* MeshEntity::Copy() const
	{
		return new MeshEntity(*this);
	}

	void MeshEntity::InitializeSignatures()
	{
		Entity::InitializeSignatures();

		AddExternalAttribute(sMeshFileNameKey, &mMeshFileName, 1);
		AddExternalAttribute(sTextureFileNameKey, &mTextureFileName, 1);
		AddExternalAttribute(sVertexShaderFileNameKey, &mVertexShaderFileName, 1);
		AddExternalAttribute(sPixelShaderFileNameKey, &mPixelShaderFileName, 1);
	}

	void MeshEntity::CopyPrivateDataMembers(const MeshEntity& otherMeshEntity)
	{
		mMesh = otherMeshEntity.mMesh;
		mMeshFileName = otherMeshEntity.mMeshFileName;
		mTextureFileName = otherMeshEntity.mTextureFileName;
		mVertexShaderFileName = otherMeshEntity.mVertexShaderFileName;
		mPixelShaderFileName = otherMeshEntity.mPixelShaderFileName;
	}

	void MeshEntity::FixExternalAttributes()
	{
		Append(sMeshFileNameKey).SetStorage(&mMeshFileName, 1);
		Append(sTextureFileNameKey).SetStorage(&mTextureFileName, 1);
		Append(sVertexShaderFileNameKey).SetStorage(&mVertexShaderFileName, 1);
		Append(sPixelShaderFileNameKey).SetStorage(&mPixelShaderFileName, 1);
	}

	const std::string MeshEntity::sMeshFileNameKey = "mesh filename";

	const std::string MeshEntity::sTextureFileNameKey = "texture filename";

	const std::string MeshEntity::sVertexShaderFileNameKey = "vertex shader filename";

	const std::string MeshEntity::sPixelShaderFileNameKey = "pixel shader filename";
}