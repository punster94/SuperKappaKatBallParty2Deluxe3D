#include "pch.h"

using namespace FieaGameEngine;

namespace KatBall
{
	RTTI_DEFINITIONS(RigidBody)

	RigidBody::RigidBody(const std::string& name) :
		Entity(name), mMass(0.0f), mColliderDimensions(1.0f, 1.0f, 1.0f, 0.0f)
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

		worldState.mWorld->RegisterRigidBody(*mCollider, *mBody);
	}

	void RigidBody::Reset(WorldState& worldState)
	{
		Entity::Reset(worldState);

		// TODO -- leaks memory
		(const_cast<RigidBody*>(this)->*sCreateColliders[mColliderType])(mColliderDimensions.x, mColliderDimensions.y, mColliderDimensions.z);

		glm::vec3 pos = GetWorldPosition();
		mTransform.setIdentity();
		mTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));

		bool isDynamic = (mMass != 0.0f);

		mLocalIntertia = btVector3(mTransformLocalIntertia.x, mTransformLocalIntertia.y, mTransformLocalIntertia.z);

		if(isDynamic)
		{
			mCollider->calculateLocalInertia(mMass, mLocalIntertia);
		}

		mMotionState = new btDefaultMotionState(mTransform);
		mConstructionInfo = new btRigidBody::btRigidBodyConstructionInfo(mMass, mMotionState, mCollider, mLocalIntertia);
		mBody = new btRigidBody(*mConstructionInfo);

		mBody->activate(true);

		worldState.mWorld->RegisterRigidBody(*mCollider, *mBody);
		// TODO -- leaks memory
	}

	Scope* RigidBody::Copy() const
	{
		return new RigidBody(*this);
	}

	RigidBody::~RigidBody()
	{

	}

	void RigidBody::CopyPrivateDataMembers(const RigidBody& otherRigidBody)
	{
		mColliderDimensions = otherRigidBody.mColliderDimensions;
		mTransformLocalIntertia = otherRigidBody.mTransformLocalIntertia;
		mMass = otherRigidBody.mMass;
		mColliderType = otherRigidBody.mColliderType;

		FixExternalAttributes();
	}

	void RigidBody::FixExternalAttributes()
	{
		Append(sColliderDimensionsKey).SetStorage(&mColliderDimensions, 1);
		Append(sLocalIntertiaKey).SetStorage(&mTransformLocalIntertia, 1);
		Append(sMassKey).SetStorage(&mMass, 1);
		Append(sColliderTypeKey).SetStorage(&mColliderType, 1);
	}

	const std::string RigidBody::sColliderDimensionsKey = "dimensions";

	const std::string RigidBody::sLocalIntertiaKey = "inertia";

	const std::string RigidBody::sMassKey = "mass";

	const std::string RigidBody::sColliderTypeKey = "shape";

	const HashMap<std::string, RigidBody::CreateCollider> RigidBody::sCreateColliders =
	{
		{ "box", &RigidBody::CreateBoxCollider },
		{ "sphere", &RigidBody::CreateSphereCollider },
	};

}