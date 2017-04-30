#include "pch.h"

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

		Datum& entities = Entities();

		for (uint32_t i = 0; i < entities.Size(); ++i)
		{
			if (entities.Get<Scope*>(i)->Is(RigidBody::TypeIdClass()))
			{
				mRigidBody = entities.Get<Scope*>(i)->As<RigidBody>();
			}

			if (entities.Get<Scope*>(i)->Is(MeshEntity::TypeIdClass()))
			{
				mMeshEntity = entities.Get<Scope*>(i)->As<MeshEntity>();
			}
		}

		mGamepad = new Gamepad(sPlayerId++);
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
			mRigidBody->mBody->applyCentralForce(btVector3(mMovementForce * mGamepad->leftStickX, mMovementForce * mGamepad->leftStickY, 0));
		}

		btTransform trans;
		mRigidBody->mBody->getMotionState()->getWorldTransform(trans);

		mPosition.x = trans.getOrigin().getX();
		mPosition.y = trans.getOrigin().getY();
		mPosition.z = trans.getOrigin().getZ();

		mMeshEntity->SetPosition(mPosition);
	}

	void Player::CopyPrivateDataMembers(const Player& otherPlayer)
	{

	}

	void Player::FixExternalAttributes()
	{

	}

	void Player::InitializeSignatures()
	{
		AddExternalAttribute(sMoveSpeedKey, &mMovementForce, 1);
	}

	int32_t Player::sPlayerId = 0;

	const string Player::sMeshKey = "mesh";

	const string Player::sRigidBodyKey = "rigidbody";

	const string Player::sMoveSpeedKey = "movespeed";


}