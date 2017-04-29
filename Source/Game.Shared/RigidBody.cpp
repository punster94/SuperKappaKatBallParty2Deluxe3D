#include "pch.h"

using namespace FieaGameEngine;

namespace KatBall
{
	RTTI_DEFINITIONS(RigidBody)
	RigidBody::RigidBody() :
		mMass(0.0f)
	{
		mMesh.SetOwner(*this);
		mMesh.SetMeshGeometry(Asset::Get(MESH_KAT)->As<MeshGeometry>());
		mMesh.SetTexture(Asset::Get(TEXTURE_KAT)->As<Texture>());
		mMesh.SetShaders(Asset::Get(SHADER_MESH_VERTEX)->As<VertexShader>(),
			Asset::Get(SHADER_MESH_PIXEL)->As<PixelShader>());
		AddRenderable(mMesh);
	}

	void RigidBody::InitializeSignatures()
	{
		Entity::InitializeSignatures();

		AddExternalAttribute(sColliderDimensionsKey, &mColliderDimensions, 1);
		AddExternalAttribute(sLocalIntertiaKey, &mLocalIntertia, 1);
		AddExternalAttribute(sMassKey, &mMass, 1);
		AddExternalAttribute(sColliderTypeKey, &mColliderType, 1);
	}

	void RigidBody::CreateBoxCollider(btScalar x, btScalar y, btScalar z)
	{
		mCollider = new btBoxShape(btVector3(x, y, z));
	}

	void RigidBody::CreateSphereCollider(btScalar x, btScalar , btScalar )
	{
		mCollider = new btSphereShape(x);
	}

	void Initialize()
	{

	}

	RigidBody::~RigidBody()
	{

	}

	const std::string RigidBody::sColliderDimensionsKey = "dimensions";

	const std::string RigidBody::sLocalIntertiaKey = "inertia";

	const std::string RigidBody::sMassKey = "mass";

	const std::string RigidBody::sColliderTypeKey = "type";

}