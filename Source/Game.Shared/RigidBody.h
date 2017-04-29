#pragma once

#include "Entity.h"
#include "MeshDirectX.h"
#define BT_NO_SIMD_OPERATOR_OVERLOADS
#include "btBulletDynamicsCommon.h"
#include "WorldState.h"
#include "Factory.h"

namespace KatBall
{
	class RigidBody : public FieaGameEngine::Entity
	{
		RTTI_DECLARATIONS(RigidBody, FieaGameEngine::Entity)

	public:

		explicit RigidBody(const std::string& name = "");

		virtual ~RigidBody();

		RigidBody(const RigidBody& otherRigidBody);

		virtual void Initialize(FieaGameEngine::WorldState& worldState) override;

		virtual Scope* Copy() const override;

	protected:

		void InitializeSignatures();

	private:

		glm::vec4 mColliderDimensions;

		glm::vec4 mTransformLocalIntertia;

		float  mMass;

		btCollisionShape* mCollider;

		btMotionState* mMotionState;

		btRigidBody* mBody;

		btTransform mTransform;

		btVector3 mLocalIntertia;

		btRigidBody::btRigidBodyConstructionInfo* mConstructionInfo;

		std::string mColliderType;

		typedef void(RigidBody::*CreateCollider)(btScalar x, btScalar y, btScalar z);

		static const FieaGameEngine::HashMap<std::string, CreateCollider> sCreateColliders;

		void CreateBoxCollider(btScalar x, btScalar y, btScalar z);

		void CreateSphereCollider(btScalar x, btScalar , btScalar );

		void CopyPrivateDataMembers(const RigidBody& otherRigidBody);

		void FixExternalAttributes();

		static const std::string sColliderDimensionsKey;

		static const std::string sLocalIntertiaKey;

		static const std::string sMassKey;

		static const std::string sColliderTypeKey;
	};

	ConcreteEntityFactory(RigidBody);
}