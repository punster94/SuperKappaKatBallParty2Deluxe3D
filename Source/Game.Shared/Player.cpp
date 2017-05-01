#include "pch.h"

using namespace FieaGameEngine;
using namespace std;

namespace KatBall
{
	RTTI_DEFINITIONS(Player)

	Player::Player(const std::string& name) :
		Entity(name), mBallRigidBody(nullptr), mBallMesh(nullptr), mGamepad(nullptr), mPunched(false),
		mLength(2.0f)
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

		mBallRigidBody = FindChildEntityByName(sBallColliderKey)->As<RigidBody>();
		mBallMesh = FindChildEntityByName(sBallMeshKey)->As<MeshEntity>();

		mPunchRigidBody = FindChildEntityByName(sPunchColliderKey)->As<RigidBody>();
		mPunchMesh = FindChildEntityByName(sPunchMeshKey)->As<MeshEntity>();

		mPunchRigidBody->mSimulatePhysics = false;
		mBallRigidBody->mBody->setFriction(30);

		mGamepad = new Gamepad(sPlayerId++);

		mInitialPunchScale = mPunchRigidBody->GetRelativeScale();
		mCurrentLength = mLength;

		mMass = mPunchRigidBody->mBody->getInvMass();
		mCurrentMass = mMass;
	}

	Scope* Player::Copy() const
	{
		return new Player(*this);
	}

	void Player::Update(FieaGameEngine::WorldState& worldState)
	{
		btTransform trans;
		btTransform trans1;
		btTransform trans2;

		if (mGamepad->Refresh())
		{
			mBallRigidBody->mBody->applyCentralImpulse(btVector3(mMovementForce * mGamepad->leftStickX, 0, mMovementForce * mGamepad->leftStickY));
		//	RotatePlayer(mGamepad->leftStickX, mGamepad->leftStickY);
		}

		if (!mPunchRigidBody->mSimulatePhysics && mGamepad->IsPressed(XINPUT_GAMEPAD_A))
		{
			mPunchRigidBody->mSimulatePhysics = true;
			mPunchRigidBody->mBody->applyCentralImpulse(btVector3(600, 0, 0));
			mInitialPunchPos = mPunchRigidBody->GetRelativePosition();
		}

		mBallRigidBody->mBody->getMotionState()->getWorldTransform(trans1);
		btVector3 pos = trans1.getOrigin();
		
		mPunchRigidBody->mBody->getMotionState()->getWorldTransform(trans2);
		btVector3 punchPos = trans2.getOrigin();

		if (pos.distance(punchPos) > mCurrentLength && !mGamepad->IsPressed(XINPUT_GAMEPAD_A))
		{
			mPunchRigidBody->mSimulatePhysics = false;
			mPunchRigidBody->mBody->setLinearVelocity(btVector3(0, 0, 0));
			mPunchRigidBody->mBody->clearForces();
			mPunchRigidBody->SetRelativePosition(mInitialPunchPos);	
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
	}

	void Player::ActivateBigBoi(float scaleFactor)
	{
		glm::vec3 scale = mPunchRigidBody->GetRelativeScale();
		scale *= scaleFactor;
		mPunchRigidBody->SetRelativeScale(scale);

		mCurrentMass *= scaleFactor;

		mPunchRigidBody->mBody->setMassProps(mCurrentMass, btVector3(0, 0, 0));
		
		mPunchRigidBody->ResizeCollider();
	}

	int32_t Player::sPlayerId = 0;

	const string Player::sMeshKey = "mesh";

	const string Player::sRigidBodyKey = "rigidbody";

	const string Player::sMoveSpeedKey = "movespeed";

	const string Player::sBallMeshKey = "ball mesh";

	const string Player::sBallColliderKey = "ball collider";

	const string Player::sPunchMeshKey = "punch mesh";

	const string Player::sPunchColliderKey = "punch collider";

	const string Player::sPunchEntityKey = "punch";

	const string Player::sLengthKey = "punch length";


}