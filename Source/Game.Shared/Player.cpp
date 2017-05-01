#include "pch.h"
#include "Event.h"

using namespace FieaGameEngine;
using namespace std;

namespace KatBall
{
	RTTI_DEFINITIONS(Player)

	Player::Player(const std::string& name) :
		Entity(name), mRigidBody(nullptr), mMeshEntity(nullptr), mKatMeshEntity(nullptr), mGamepad(nullptr), mPunchSound(nullptr), mHitSound(nullptr), mAnimTime(0.0f), mAnimState(AnimState::IDLE), mAnimFrame(0)
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

		Entity* entity;
		if ((entity = FindChildEntityByName(sBallColliderKey)))
		{
			mRigidBody = entity->As<RigidBody>();
		}
		if ((entity = FindChildEntityByName(sBallMeshKey)))
		{
			mMeshEntity = entity->As<MeshEntity>();
		}
		if ((entity = FindChildEntityByName(sKatMeshKey)))
		{
			mKatMeshEntity = entity->As<MeshEntity>();
		}
		if ((entity = FindChildEntityByName(sPunchSoundKey)))
		{
			mPunchSound = entity->As<KatSound>();
		}
		if ((entity = FindChildEntityByName(sHitSoundKey)))
		{
			mHitSound = entity->As<KatSound>();
		}

		mGamepad = new Gamepad(sPlayerId++);

		// Load all of the possible animations
		LoadRequiredMeshGeometries();
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
		worldState.mEntity = this;

		if (mGamepad->Refresh())
		{
			mRigidBody->mBody->applyCentralImpulse(btVector3(mMovementForce * mGamepad->leftStickX, 0, mMovementForce * mGamepad->leftStickY));
		
			//if ((mGamepad->GetState()->wButtons & XINPUT_GAMEPAD_A) != 0)
			//{
			//	if (mPunchSound->GetStatus() != sf::Sound::Playing)
			//	{
			//		mPunchSound->Play();
			//	}
			//}
			//if ((mGamepad->GetState()->wButtons & XINPUT_GAMEPAD_B) != 0)
			//{
			//	if (mHitSound->GetStatus() != sf::Sound::Playing)
			//	{
			//		mHitSound->Play();
			//	}
			//}
		}

		btTransform trans;
		mRigidBody->mBody->getMotionState()->getWorldTransform(trans);

		mPosition.x = trans.getOrigin().getX();
		mPosition.y = trans.getOrigin().getY();
		mPosition.z = trans.getOrigin().getZ();
		
		UpdateAnimation(worldState);

		SetRelativePosition(mPosition);
	}

	void Player::CopyPrivateDataMembers(const Player& otherPlayer)
	{
		mMovementForce = otherPlayer.mMovementForce;

		FixExternalAttributes();
	}

	void Player::FixExternalAttributes()
	{
		Append(sMoveSpeedKey).SetStorage(&mMovementForce, 1);
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

		float speed = mRigidBody->mBody->getLinearVelocity().length();

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

		float speed = mRigidBody->mBody->getLinearVelocity().length();

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
	}

	int32_t Player::sPlayerId = 0;

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
}