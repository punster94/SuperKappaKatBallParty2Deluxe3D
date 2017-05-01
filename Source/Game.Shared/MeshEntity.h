#pragma once

#include "Entity.h"
#include "MeshDirectX.h"
#include "WorldState.h"
#include "Factory.h"

namespace KatBall
{
	class MeshEntity : public FieaGameEngine::Entity
	{
		RTTI_DECLARATIONS(MeshEntity, FieaGameEngine::Entity)

	public:

		explicit MeshEntity(const std::string& name = "");

		virtual ~MeshEntity() = default;

		MeshEntity(const MeshEntity& otherMeshEntity);

		virtual void Initialize(FieaGameEngine::WorldState& worldState) override;

		virtual Scope* Copy() const override;

		void SetMeshGeometry(FieaGameEngine::MeshGeometry* geometry);

	protected:

		void InitializeSignatures();

	private:

		void CopyPrivateDataMembers(const MeshEntity& otherMeshEntity);

		void FixExternalAttributes();

		FieaGameEngine::MeshDirectX mMesh;

		std::string mMeshFileName;

		std::string mTextureFileName;

		std::string mVertexShaderFileName;

		std::string mPixelShaderFileName;

		std::int32_t mRenderPass;

		glm::vec4 mColor;

		static const std::string sMeshFileNameKey;

		static const std::string sTextureFileNameKey;

		static const std::string sVertexShaderFileNameKey;

		static const std::string sPixelShaderFileNameKey;

		static const std::string sRenderPassKey;

		static const std::string sColorKey;
	};

	ConcreteEntityFactory(MeshEntity);
}