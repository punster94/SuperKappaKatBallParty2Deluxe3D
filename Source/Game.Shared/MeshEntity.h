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

		static const std::string sMeshFileNameKey;

		static const std::string sTextureFileNameKey;

		static const std::string sVertexShaderFileNameKey;

		static const std::string sPixelShaderFileNameKey;
	};

	ConcreteEntityFactory(MeshEntity);
}