#pragma once
#include "Entity.h"
#include "Gamepad.h"
#include "RigidBody.h"
#include "MeshEntity.h"
#include "KatSound.h"

namespace KatBall
{
	class Player : public FieaGameEngine::Entity
	{
		RTTI_DECLARATIONS(Player, FieaGameEngine::Entity)

	private:

		enum class AnimState
		{
			IDLE,
			RUN,
			VICTORY
		};

	public:

		explicit Player(const std::string& name = "");

		virtual ~Player() = default;

		Player(const Player& otherPlayer);

		virtual void Initialize(FieaGameEngine::WorldState& worldState) override;
		virtual void Reset(FieaGameEngine::WorldState& worldState) override;

		virtual Scope* Copy() const override;

		virtual void Update(FieaGameEngine::WorldState& worldState) override;

		void DeleteGamepad() { delete mGamepad; mGamepad = nullptr; }

		void CreateGamePad() { mGamepad = new Gamepad(); }

		void SetAnimState(AnimState state);

		void Player::ActivateLongBoi(float length);

		void Player::ActivateBigBoi(float scaleFactor);

		void Player::ActivateVortexBoi(float rotationSpeed);

		void OnHit();

		void Respawn(FieaGameEngine::WorldState& worldState);

		std::int32_t GetPlayerID();

		void SetLastPlayerTouching(std::int32_t id);

	protected:

		void InitializeSignatures();

	private:

		void CopyPrivateDataMembers(const Player& otherPlayer);

		void FixExternalAttributes();

		void UpdateAnimation(FieaGameEngine::WorldState& worldState);

		void UpdateFrame(FieaGameEngine::Vector<FieaGameEngine::MeshGeometry*>& meshes, float frameTime);

		void UpdateIdleAnimation();

		void UpdateRunAnimation();

		void UpdateVictoryAnimation();

		void LoadRequiredMeshGeometries();

		void RotatePlayer(float x, float y, FieaGameEngine::WorldState& worldState);

		bool CollisionCallback(btManifoldPoint& collisionPoint, const btCollisionObjectWrapper* obj1, int id1, int index1,
			const btCollisionObjectWrapper* obj2, int id2, int index2);

		RigidBody* mBallRigidBody;

		RigidBody* mPunchRigidBody;

		MeshEntity* mBallMesh;

		MeshEntity* mPunchMesh;

		MeshEntity* mKatMeshEntity;

		Gamepad* mGamepad;

		FieaGameEngine::KatSound* mPunchSound;

		FieaGameEngine::KatSound* mHitSound;

		float mMovementForce;

		float mAnimTime;

		AnimState mAnimState;

		std::uint32_t mAnimFrame;

		glm::vec3 mInitialPunchPos;

		bool mPunched;

		float mLength;

		float mCurrentLength;

		float mMass;

		float mCurrentMass;

		float mPunchImpulse;

		float mCurrentPunchImpulse;

		glm::vec3 mInitialPunchScale;

		std::uint32_t mLastTouchingPlayerID;

		sf::Sound mSoundPlayer;
		sf::SoundBuffer mSoundBuffer;

		FieaGameEngine::Vector<FieaGameEngine::MeshGeometry*> mIdleMeshGeometries;
		FieaGameEngine::Vector<FieaGameEngine::MeshGeometry*> mRunMeshGeometries;
		FieaGameEngine::Vector<FieaGameEngine::MeshGeometry*> mVictoryMeshGeometries;

		static const std::string sRigidBodyKey;
		static const std::string sMeshKey;
		static const std::string sMoveSpeedKey;
		static const std::string sBallMeshKey;
		static const std::string sKatMeshKey;
		static const std::string sBallColliderKey;
		static const std::string sPunchSoundKey;
		static const std::string sHitSoundKey;
		static const std::string sIdleAnimationsKey;
		static const std::string sRunAnimationsKey;
		static const std::string sVictoryAnimationsKey;

		static const std::string sPunchMeshKey;

		static const std::string sPunchColliderKey;

		static const std::string sPunchEntityKey;

		static const std::string sLengthKey;

		static const std::string sPunchImpulseKey;
	};

	ConcreteEntityFactory(Player);
}
