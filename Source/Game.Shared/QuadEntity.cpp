#include "pch.h"

using namespace FieaGameEngine;

namespace KatBall
{
	RTTI_DEFINITIONS(QuadEntity)

	QuadEntity::QuadEntity(const std::string& name) :
		Entity(name), mTextureFileName(""),
		mVertexShaderFileName(SHADER_QUAD_VERTEX), mPixelShaderFileName(SHADER_QUAD_PIXEL)
	{
		InitializeSignatures();
	}

	QuadEntity::QuadEntity(const QuadEntity& otherQuadEntity) :
		Entity(otherQuadEntity)
	{
		CopyPrivateDataMembers(otherQuadEntity);
	}

	void QuadEntity::Initialize(FieaGameEngine::WorldState& worldState)
	{
		Entity::Initialize(worldState);

		if (Asset::Get(mTextureFileName) == nullptr)
		{
			Asset::Load(mTextureFileName, mTextureFileName, Asset::TYPE_TEXTURE);
		}

		mQuad.SetTexture(Asset::Get(mTextureFileName)->As<Texture>());

		if (Asset::Get(mVertexShaderFileName) == nullptr)
		{
			Asset::Load(mVertexShaderFileName, mVertexShaderFileName, Asset::TYPE_VERTEX_SHADER);
		}

		if (Asset::Get(mPixelShaderFileName) == nullptr)
		{
			Asset::Load(mPixelShaderFileName, mPixelShaderFileName, Asset::TYPE_PIXEL_SHADER);
		}

		mQuad.SetShaders(Asset::Get(mVertexShaderFileName)->As<VertexShader>(),
			Asset::Get(mPixelShaderFileName)->As<PixelShader>());

		mQuad.SetRect(mRect.x, mRect.y, mRect.z, mRect.w);
		mQuad.SetColor(mColor);

		Renderer::Get()->AddViewRenderable(mQuad);
	}

	Scope* QuadEntity::Copy() const
	{
		return new QuadEntity(*this);
	}

	void QuadEntity::InitializeSignatures()
	{
		Entity::InitializeSignatures();

		AddExternalAttribute(sTextureFileNameKey, &mTextureFileName, 1);
		AddExternalAttribute(sVertexShaderFileNameKey, &mVertexShaderFileName, 1);
		AddExternalAttribute(sPixelShaderFileNameKey, &mPixelShaderFileName, 1);
		AddExternalAttribute(sColorKey, &mColor, 1);
		AddExternalAttribute(sRectKey, &mRect, 1);
	}

	void QuadEntity::CopyPrivateDataMembers(const QuadEntity& otherQuadEntity)
	{
		mTextureFileName = otherQuadEntity.mTextureFileName;
		mVertexShaderFileName = otherQuadEntity.mVertexShaderFileName;
		mPixelShaderFileName = otherQuadEntity.mPixelShaderFileName;
		mColor = otherQuadEntity.mColor;
		mRect = otherQuadEntity.mRect;
	}

	void QuadEntity::FixExternalAttributes()
	{
		Append(sTextureFileNameKey).SetStorage(&mTextureFileName, 1);
		Append(sVertexShaderFileNameKey).SetStorage(&mVertexShaderFileName, 1);
		Append(sPixelShaderFileNameKey).SetStorage(&mPixelShaderFileName, 1);
		Append(sColorKey).SetStorage(&mColor, 1);
		Append(sRectKey).SetStorage(&mRect, 1);
	}

	const std::string QuadEntity::sTextureFileNameKey = "texture filename";

	const std::string QuadEntity::sVertexShaderFileNameKey = "vertex shader filename";

	const std::string QuadEntity::sPixelShaderFileNameKey = "pixel shader filename";

	const std::string QuadEntity::sColorKey = "color";

	const std::string QuadEntity::sRectKey = "rect";
}