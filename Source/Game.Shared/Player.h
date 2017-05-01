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

		void Player::ActivateLongBoi(float length);

		void Player::ActivateBigBoi(float scaleFactor);

	protected:

		void InitializeSignatures();

	private:

		void CopyPrivateDataMembers(const Player& otherPlayer);

		void FixExternalAttributes();

		void RotatePlayer(float x, float y);

		RigidBody* mBallRigidBody;

		RigidBody* mPunchRigidBody;

		MeshEntity* mBallMesh;

		MeshEntity* mPunchMesh;

		Gamepad* mGamepad;

		float mMovementForce;

		glm::vec3 mInitialPunchPos;

		bool mPunched;

		float mLength;

		float mCurrentLength;

		float mMass;

		float mCurrentMass;

		glm::vec3 mInitialPunchScale;

		static const std::string sRigidBodyKey;

		static const std::string sMeshKey;

		static const std::string sMoveSpeedKey;

		static const std::string sBallMeshKey;

		static const std::string sBallColliderKey;

		static const std::string sPunchMeshKey;

		static const std::string sPunchColliderKey;

		static const std::string sPunchEntityKey;

		static const std::string sLengthKey;

		static std::int32_t sPlayerId;
	};

	ConcreteEntityFactory(Player);
}