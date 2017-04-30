#pragma once
#include "Entity.h"
#include "Gamepad.h"
#include "RigidBody.h"
#include "MeshEntity.h"

namespace KatBall
{
	class Player : public FieaGameEngine::Entity
	{
		RTTI_DECLARATIONS(Player, FieaGameEngine::Entity)

	public:

		explicit Player(const std::string& name = "");

		virtual ~Player() = default;

		Player(const Player& otherPlayer);

		virtual void Initialize(FieaGameEngine::WorldState& worldState) override;

		virtual Scope* Copy() const override;

		virtual void Update(FieaGameEngine::WorldState& worldState) override;

	protected:

		void InitializeSignatures();

	private:

		void CopyPrivateDataMembers(const Player& otherPlayer);

		void FixExternalAttributes();

		RigidBody* mRigidBody;

		MeshEntity* mMeshEntity;

		Gamepad* mGamepad;

		float mMovementForce;

		static const std::string sRigidBodyKey;

		static const std::string sMeshKey;

		static const std::string sMoveSpeedKey;

		static std::int32_t sPlayerId;

	};

	ConcreteEntityFactory(Player);
}