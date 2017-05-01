#include "pch.h"

#include "Event.h"
#include "ActionUpdateScore.h"
#include "EventMessageAttributed.h"

using namespace FieaGameEngine;
using namespace std;

namespace KatBall
{
	RTTI_DEFINITIONS(Player)

	Player::Player(const std::string& name) :
		Entity(name), mBallRigidBody(nullptr), mBallMesh(nullptr), mKatMeshEntity(nullptr),
		mGamepad(nullptr), mPunchSound(nullptr), mHitSound(nullptr), mAnimTime(0.0f),
		mAnimState(AnimState::IDLE), mAnimFrame(0), mPunched(false), mLength(2.0f)
	{
		InitializeSignatures();
	}

	Player::Player(const Player& otherPlayer):
		Entity(otherPlayer)
	{
		CopyPrivateDataMembers(otherPlayer);
	}

	void Player::Initialize(WorldState& worldState)
	{
		Entity::Initialize(worldState);

		// Load all of the possible animations
		LoadRequiredMeshGeometries();

		mBallRigidBody = FindChildEntityByName(sBallColliderKey)->As<RigidBody>();
		mBallMesh = FindChildEntityByName(sBallMeshKey)->As<MeshEntity>();

		mPunchRigidBody = FindChildEntityByName(sPunchColliderKey)->As<RigidBody>();
		mPunchMesh = FindChildEntityByName(sPunchMeshKey)->As<MeshEntity>();

		mKatMeshEntity = FindChildEntityByName(sKatMeshKey)->As<MeshEntity>();

		mPunchSound = FindChildEntityByName(sPunchSoundKey)->As<KatSound>();
		mHitSound = FindChildEntityByName(sHitSoundKey)->As<KatSound>();

		mPunchRigidBody->mSimulatePhysics = false;
		mBallRigidBody->mBody->setFriction(30);

		mGamepad = new Gamepad();

		mInitialPunchScale = mPunchRigidBody->GetRelativeScale();
		mCurrentLength = mLength;

		mMass = mPunchRigidBody->mBody->getInvMass();
		mCurrentMass = mMass;

		mLastTouchingPlayerID = NUM_PLAYERS;
	}

	void Player::Reset(WorldState& worldState)
	{
		Entity::Reset(worldState);

		mInitialPunchScale = mPunchRigidBody->GetRelativeScale();
		mCurrentLength = mLength;
		mCurrentMass = mMass;

		mLastTouchingPlayerID = NUM_PLAYERS;
	}

	void Player::LoadRequiredMeshGeometries()
	{
		Datum& idleDatum = (*this)[sIdleAnimationsKey];
		Datum& runDatum = (*this)[sRunAnimationsKey];
		Datum& victoryDatum = (*this)[sVictoryAnimationsKey];

		// Idle Animations
		for (std::uint32_t i = 0; i < idleDatum.Size(); i++)
		{
			std::string& name = idleDatum.Get<string&>(i);
			std::string path = std::string(ASSET_DIRECTORY_MESHES) + name;
			Asset::Load(path, name, Asset::TYPE_MESH);
			mIdleMeshGeometries.PushBack(Asset::Get(name)->As<MeshGeometry>());
		}

		// run Animations
		for (std::uint32_t i = 0; i < runDatum.Size(); i++)
		{
			std::string& name = runDatum.Get<string&>(i);
			std::string path = std::string(ASSET_DIRECTORY_MESHES) + name;
			Asset::Load(path, name, Asset::TYPE_MESH);
			mRunMeshGeometries.PushBack(Asset::Get(name)->As<MeshGeometry>());
		}

		// victory Animations
		for (std::uint32_t i = 0; i < victoryDatum.Size(); i++)
		{
			std::string& name = victoryDatum.Get<string&>(i);
			std::string path = std::string(ASSET_DIRECTORY_MESHES) + name;
			Asset::Load(path, name, Asset::TYPE_MESH);
			mVictoryMeshGeometries.PushBack(Asset::Get(name)->As<MeshGeometry>());
		}
	}

	Scope* Player::Copy() const
	{
		return new Player(*this);
	}

	void Player::Update(FieaGameEngine::WorldState& worldState)
	{
		btTransform trans1;
		btTransform trans2;

		if (mGamepad->Refresh())
		{
			mBallRigidBody->mBody->applyCentralImpulse(btVector3(mMovementForce * mGamepad->leftStickX, 0, mMovementForce * mGamepad->leftStickY));
			// RotatePlayer(mGamepad->leftStickX, mGamepad->leftStickY);

			if(!mPunchRigidBody->mSimulatePhysics && mGamepad->IsPressed(XINPUT_GAMEPAD_A))
			{
				mPunchRigidBody->mSimulatePhysics = true;
				mPunchRigidBody->mBody->applyCentralImpulse(btVector3(600, 0, 0));
				mInitialPunchPos = mPunchRigidBody->GetRelativePosition();

				if(mPunchSound != nullptr && mPunchSound->GetStatus() != sf::Sound::Playing)
				{
					mPunchSound->Play();
				}
			}

			UpdateAnimation(worldState);

			mBallRigidBody->mBody->getMotionState()->getWorldTransform(trans1);
			btVector3 pos = trans1.getOrigin();

			mPunchRigidBody->mBody->getMotionState()->getWorldTransform(trans2);
			btVector3 punchPos = trans2.getOrigin();

			if(pos.distance(punchPos) > mCurrentLength && !mGamepad->IsPressed(XINPUT_GAMEPAD_A))
			{
				mPunchRigidBody->mSimulatePhysics = false;
				mPunchRigidBody->mBody->setLinearVelocity(btVector3(0, 0, 0));
				mPunchRigidBody->mBody->clearForces();
				mPunchRigidBody->SetRelativePosition(mInitialPunchPos);
			}

			// are we below the respawn threshold???
			if(pos.getY() < RESPAWN_THRESHOLD)
			{
				Respawn(worldState);
			}

			SetWorldPosition(mBallRigidBody->GetWorldPosition());
			mBallRigidBody->SetRelativePosition(glm::vec3(0.0f));

		}

		Entity::Update(worldState);
	}

	void Player::CopyPrivateDataMembers(const Player& otherPlayer)
	{
		mMovementForce = otherPlayer.mMovementForce;
		mPunched = otherPlayer.mPunched;
		mLength = otherPlayer.mLength;

		FixExternalAttributes();
	}

	void Player::FixExternalAttributes()
	{
		Append(sMoveSpeedKey).SetStorage(&mMovementForce, 1);
		Append(sLengthKey).SetStorage(&mLength, 1);
	}

	void Player::UpdateAnimation(FieaGameEngine::WorldState& worldState)
	{
		const float minAnimTime = 0.1f;
		const float maxAnimTime = 0.5f;
		const float maxSpeed = 1.0f;

		mAnimTime += worldState.DeltaTime();

		switch (mAnimState)
		{
			case AnimState::IDLE:
				UpdateIdleAnimation();
				break;
			case AnimState::RUN:
				UpdateRunAnimation();
				break;
			case AnimState::VICTORY:
				UpdateVictoryAnimation();
				break;
		}
	}

	void Player::UpdateIdleAnimation()
	{
		const float frameTime = 0.3f;
		const float runSpeedThreshold = 0.4f;

		float speed = mBallRigidBody->mBody->getLinearVelocity().length();

		if (speed > runSpeedThreshold)
		{
			mAnimState = AnimState::RUN;
			mAnimTime = 0.0f;
		}

		UpdateFrame(mIdleMeshGeometries, frameTime);
	}

	void Player::UpdateRunAnimation()
	{
		const float frameTime = 0.3f;
		const float runSpeedThreshold = 0.4f;

		float speed = mBallRigidBody->mBody->getLinearVelocity().length();

		if (speed < runSpeedThreshold)
		{
			SetAnimState(AnimState::IDLE);
		}

		UpdateFrame(mRunMeshGeometries, frameTime);
	}

	void Player::SetAnimState(AnimState state)
	{
		mAnimState = state;
		mAnimTime = 0.0f;
		mAnimFrame = 0;
	}

	void Player::UpdateVictoryAnimation()
	{
		const float frameTime = 0.3f;

		UpdateFrame(mVictoryMeshGeometries, frameTime);
	}

	void Player::UpdateFrame(Vector<MeshGeometry*>& meshes, float frameTime)
	{
		if (mAnimTime > frameTime)
		{
			mAnimTime = 0.0f;
			mAnimFrame++;
		}

		if (mAnimFrame >= meshes.Size())
		{
			mAnimFrame = 0U;
		}

		mKatMeshEntity->SetMeshGeometry(meshes[mAnimFrame]);
	}

	void Player::InitializeSignatures()
	{
		AddExternalAttribute(sMoveSpeedKey, &mMovementForce, 1);
		AddExternalAttribute(sLengthKey, &mLength, 1);
	}

	void Player::RotatePlayer(float x, float y)
	{
		glm::vec3 direction(x, y, 0);

		if (direction.length() > 0.5f)
		{
			SetWorldRotation(glm::vec3(0, glm::atan(x / y), 0));
		}
	}

	void Player::ActivateLongBoi(float length)
	{
		mCurrentLength += length;

		OutputDebugString(L"Hi");
	}

	void Player::ActivateBigBoi(float scaleFactor)
	{
		glm::vec3 scale = mPunchRigidBody->GetRelativeScale();
		scale *= scaleFactor;
		mPunchRigidBody->SetRelativeScale(scale);

		mCurrentMass *= scaleFactor;

		mPunchRigidBody->mBody->setMassProps(mCurrentMass, btVector3(0, 0, 0));

		mPunchRigidBody->ResizeCollider();

		OutputDebugString(L"Howdy");
	}

	bool Player::CollisionCallback(btManifoldPoint& collisionPoint, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2)
	{
		OutputDebugString(L"Blah");

		return false;
	}

	void Player::OnHit()
	{
		if (mHitSound != nullptr)
		{
			mHitSound->Play();
		}
	}

	void Player::Respawn(WorldState& worldState)
	{
		// post score event -- last cool dood to touch us gets the glory of our points
		EventMessageAttributed args(ActionUpdateScore::sScoreEventSubtype, &worldState);
		args.AppendAuxiliaryAttribute(ActionUpdateScore::sPlayerIDKey) = static_cast<int32_t>(mLastTouchingPlayerID);

		Event<EventMessageAttributed>* e = new Event<EventMessageAttributed>(args);
		worldState.mWorld->Enqueue(*e, worldState, 0);

		// reset position and stuff
		Reset(worldState);
	}

	const string Player::sMeshKey = "mesh";
	const string Player::sRigidBodyKey = "rigidbody";
	const string Player::sMoveSpeedKey = "movespeed";
	const string Player::sBallMeshKey = "ball mesh";
	const string Player::sKatMeshKey = "kat mesh";
	const string Player::sBallColliderKey = "ball collider";

	const string Player::sPunchSoundKey = "punch sound";

	const string Player::sHitSoundKey = "hit sound";

	const string Player::sIdleAnimationsKey = "idle meshes";

	const string Player::sRunAnimationsKey = "run meshes";

	const string Player::sVictoryAnimationsKey = "victory meshes";

	const string Player::sPunchMeshKey = "punch mesh";

	const string Player::sPunchColliderKey = "punch collider";

	const string Player::sPunchEntityKey = "punch";

	const string Player::sLengthKey = "punch length";
}
