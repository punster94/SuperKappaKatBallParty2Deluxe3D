#pragma once
#pragma once

#include "Entity.h"
#include "Quad.h"
#include "WorldState.h"
#include "Factory.h"

namespace KatBall
{
	class QuadEntity : public FieaGameEngine::Entity
	{
		RTTI_DECLARATIONS(MeshEntity, FieaGameEngine::Entity)

	public:

		explicit QuadEntity(const std::string& name = "");

		virtual ~QuadEntity() = default;

		QuadEntity(const QuadEntity& otherQuadEntity);

		virtual void Initialize(FieaGameEngine::WorldState& worldState) override;

		virtual Scope* Copy() const override;

	protected:

		void InitializeSignatures();

	private:

		void CopyPrivateDataMembers(const QuadEntity& otherQuadEntity);

		void FixExternalAttributes();

		FieaGameEngine::Quad mQuad;

		std::string mTextureFileName;

		std::string mVertexShaderFileName;

		std::string mPixelShaderFileName;

		glm::vec4 mColor;

		glm::vec4 mRect;

		static const std::string sTextureFileNameKey;

		static const std::string sVertexShaderFileNameKey;

		static const std::string sPixelShaderFileNameKey;

		static const std::string sColorKey;

		static const std::string sRectKey;
	};

	ConcreteEntityFactory(QuadEntity);
}