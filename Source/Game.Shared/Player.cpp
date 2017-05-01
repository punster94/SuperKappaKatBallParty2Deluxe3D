#include "pch.h"
#include "Event.h"

using namespace FieaGameEngine;
using namespace std;

namespace KatBall
{
	RTTI_DEFINITIONS(Player)

	Player::Player(const std::string& name) :
		Entity(name), mRigidBody(nullptr), mMeshEntity(nullptr), mGamepad(nullptr)
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

		mRigidBody = FindChildEntityByName(sBallColliderKey)->As<RigidBody>();
		mMeshEntity = FindChildEntityByName(sBallMeshKey)->As<MeshEntity>();

		mGamepad = new Gamepad();
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
			if (mGamepad->GetState()->wButtons != 0)
			{
				FieaGameEngine::Event<Gamepad>* event = new FieaGameEngine::Event<Gamepad>(*mGamepad, false);
				worldState.mWorld->Enqueue(*event, worldState, 0);
			}

			mRigidBody->mBody->applyCentralImpulse(btVector3(mMovementForce * mGamepad->leftStickX, 0, mMovementForce * mGamepad->leftStickY));
		}

		btTransform trans;
		mRigidBody->mBody->getMotionState()->getWorldTransform(trans);

		mPosition.x = trans.getOrigin().getX();
		mPosition.y = trans.getOrigin().getY();
		mPosition.z = trans.getOrigin().getZ();

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

	void Player::InitializeSignatures()
	{
		AddExternalAttribute(sMoveSpeedKey, &mMovementForce, 1);
	}

	int32_t Player::sPlayerId = 0;

	const string Player::sMeshKey = "mesh";

	const string Player::sRigidBodyKey = "rigidbody";

	const string Player::sMoveSpeedKey = "movespeed";

	const string Player::sBallMeshKey = "ball mesh";

	const string Player::sBallColliderKey = "ball collider";
}