#include "pch.h"

using namespace FieaGameEngine;

namespace KatBall
{
	RTTI_DEFINITIONS(RigidBody)

	RigidBody::RigidBody(const std::string& name) :
		Entity(name), mMass(0.0f), mColliderDimensions(1.0f, 1.0f, 1.0f, 0.0f), mSimulatePhysics(true),
		mGravityEnable(1), mFriction(2)
	{
		InitializeSignatures();
	}

	RigidBody::RigidBody(const RigidBody& otherRigidBody) :
		Entity(otherRigidBody)
	{
		CopyPrivateDataMembers(otherRigidBody);
	}

	void RigidBody::InitializeSignatures()
	{
		Entity::InitializeSignatures();

		AddExternalAttribute(sColliderDimensionsKey, &mColliderDimensions, 1);
		AddExternalAttribute(sLocalIntertiaKey, &mTransformLocalIntertia, 1);
		AddExternalAttribute(sMassKey, &mMass, 1);
		AddExternalAttribute(sColliderTypeKey, &mColliderType, 1);
		AddExternalAttribute(sGravityEnableTypeKey, &mGravityEnable, 1);
		AddExternalAttribute(sFrictionKey, &mFriction, 1);
	}

	void RigidBody::CreateBoxCollider(btScalar x, btScalar y, btScalar z)
	{
		glm::vec3 worldScale = GetWorldScale() * glm::vec3(x, y, z);
		mCollider = new btBoxShape(btVector3(worldScale.x, worldScale.y, worldScale.z));
	}

	void RigidBody::CreateSphereCollider(btScalar x, btScalar , btScalar )
	{
		mCollider = new btSphereShape(GetWorldScale().x * x);
	}

	void RigidBody::ResizeCollider()
	{
		delete mCollider;
		(const_cast<RigidBody*>(this)->*sCreateColliders[mColliderType])(mColliderDimensions.x, mColliderDimensions.y, mColliderDimensions.z);

		mBody->setCollisionShape(mCollider);
	}

	void RigidBody::Initialize(WorldState& worldState)
	{
		Entity::Initialize(worldState);

		(const_cast<RigidBody*>(this)->*sCreateColliders[mColliderType])(mColliderDimensions.x, mColliderDimensions.y, mColliderDimensions.z);

		glm::vec3 pos = GetWorldPosition();
		mTransform.setIdentity();
		mTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));

		bool isDynamic = (mMass != 0.0f);

		mLocalIntertia = btVector3(mTransformLocalIntertia.x, mTransformLocalIntertia.y, mTransformLocalIntertia.z);

		if (isDynamic)
		{
			mCollider->calculateLocalInertia(mMass, mLocalIntertia);
		}

		mMotionState = new btDefaultMotionState(mTransform);
		mConstructionInfo = new btRigidBody::btRigidBodyConstructionInfo(mMass, mMotionState, mCollider, mLocalIntertia);
		mBody = new btRigidBody(*mConstructionInfo);

		mBody->activate(true);

		Game::GetInstance()->mDynamicsWorld->addRigidBody(mBody);

		mBody->setUserPointer(GetParent());
	}

	void RigidBody::Reset(WorldState& worldState)
	{
		// TODO -- leaks memory

		glm::vec3 pos = GetParent()->As<Entity>()->GetWorldPosition();
		btTransform trans;

		mBody->getMotionState()->getWorldTransform(trans);

		trans.setOrigin(btVector3(pos.x, pos.y, pos.z));

		mBody->getMotionState()->setWorldTransform(trans);
		mBody->setCenterOfMassTransform(trans);

		mBody->setLinearVelocity(btVector3(0, 0, 0));

		mBody->clearForces();

		mBody->applyCentralImpulse(btVector3(50, 0, 50));

		Entity::Reset(worldState);
	}

	glm::vec3 RigidBody::GetLinearVelocity() const
	{
		const btVector3& linVelocity = mBody->getLinearVelocity();
		return glm::vec3(linVelocity.getX(), linVelocity.getY(), linVelocity.getZ());
	}

	void RigidBody::Update(FieaGameEngine::WorldState& worldState)
	{
		btTransform trans;
		mBody->getMotionState()->getWorldTransform(trans);

		if (mSimulatePhysics)
		{			
			btVector3 pos = trans.getOrigin();

			glm::vec3 worldPos(pos.getX(), pos.getY(), pos.getZ());

			SetWorldPosition(worldPos);
		}
		else
		{
			glm::vec3 worldPostion = GetWorldPosition();
			btVector3 pos(worldPostion.x, worldPostion.y, worldPostion.z);
			trans.setOrigin(pos);
			mBody->getMotionState()->setWorldTransform(trans);
			mBody->setCenterOfMassTransform(trans);
		}

		Entity::Update(worldState);
	}

	Scope* RigidBody::Copy() const
	{
		return new RigidBody(*this);
	}

	RigidBody::~RigidBody()
	{
		if (Game::GetInstance()->mDynamicsWorld != nullptr)
		{
			Game::GetInstance()->mDynamicsWorld->removeRigidBody(mBody);
		}

		delete mCollider;

		delete mConstructionInfo;

		delete mBody;

		delete mMotionState;
	}

	void RigidBody::CopyPrivateDataMembers(const RigidBody& otherRigidBody)
	{
		mColliderDimensions = otherRigidBody.mColliderDimensions;
		mTransformLocalIntertia = otherRigidBody.mTransformLocalIntertia;
		mMass = otherRigidBody.mMass;
		mColliderType = otherRigidBody.mColliderType;
		mSimulatePhysics = otherRigidBody.mSimulatePhysics;
		mGravityEnable = otherRigidBody.mGravityEnable;
		mFriction = otherRigidBody.mFriction;

		FixExternalAttributes();
	}

	void RigidBody::FixExternalAttributes()
	{
		Append(sColliderDimensionsKey).SetStorage(&mColliderDimensions, 1);
		Append(sLocalIntertiaKey).SetStorage(&mTransformLocalIntertia, 1);
		Append(sMassKey).SetStorage(&mMass, 1);
		Append(sColliderTypeKey).SetStorage(&mColliderType, 1);
		Append(sGravityEnableTypeKey).SetStorage(&mGravityEnable, 1);
		Append(sFrictionKey).SetStorage(&mFriction, 1);
	}

	const std::string RigidBody::sColliderDimensionsKey = "dimensions";

	const std::string RigidBody::sLocalIntertiaKey = "inertia";

	const std::string RigidBody::sMassKey = "mass";

	const std::string RigidBody::sColliderTypeKey = "shape";

	const std::string RigidBody::sGravityEnableTypeKey = "gravity enable";

	const std::string RigidBody::sFrictionKey = "friction";

	const HashMap<std::string, RigidBody::CreateCollider> RigidBody::sCreateColliders =
	{
		{ "box", &RigidBody::CreateBoxCollider },
		{ "sphere", &RigidBody::CreateSphereCollider },
	};

}