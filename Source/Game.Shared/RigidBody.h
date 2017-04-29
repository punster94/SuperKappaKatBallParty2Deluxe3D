#pragma once

#include "Entity.h"
#include "MeshDirectX.h"
#define BT_NO_SIMD_OPERATOR_OVERLOADS
#include "btBulletDynamicsCommon.h"
#include "WorldState.h"

namespace KatBall
{
	class RigidBody : public FieaGameEngine::Entity
	{
		RTTI_DECLARATIONS(RigidBody, FieaGameEngine::Entity)

	public:

		RigidBody();

		virtual ~RigidBody();

		virtual void Initialize(WorldState& worldState) override;

		typedef void(RigidBody::*CreateCollider)(btScalar x, btScalar y, btScalar z);

		static const FieaGameEngine::HashMap<std::string, CreateCollider> sCreateColliders;

	protected:

		void InitializeSignatures();

	private:

		glm::vec4 mColliderDimensions;

		glm::vec4 mTransformPosition;

		glm::vec4 mTransformRotation;

		glm::vec4 mTransformLocalIntertia;

		float  mMass;

		FieaGameEngine::MeshDirectX mMesh;

		btCollisionShape* mCollider;

		btMotionState* mMotionState;

		btRigidBody* mBody;

		btTransform mTransform;

		btVector3 mLocalIntertia;

		std::string mColliderType;

		void CreateBoxCollider(btScalar x, btScalar y, btScalar z);

		void CreateSphereCollider(btScalar x, btScalar , btScalar );

		static const std::string sColliderDimensionsKey;

		static const std::string sLocalIntertiaKey;

		static const std::string sMassKey;

		static const std::string sColliderTypeKey;


	};
}