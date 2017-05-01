#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "Reaction.h"
#include "WorldState.h"
#include "Renderer.h"
#include "Renderable.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Entity)

		Entity::Entity(const std::string& name) :
		Attributed(), mName(name), mSector(nullptr), mScaleVec4(1.0f, 1.0f, 1.0f, 0.0f)
	{
		InitializeSignatures();
	}

	Entity::Entity(const Entity& otherEntity) :
		Attributed(otherEntity), mName(otherEntity.mName), mSector(nullptr)
	{
		CopyPrivateDataMembers(otherEntity);
	}

	Entity& Entity::operator=(const Entity& otherEntity)
	{
		if (this != &otherEntity)
		{
			Attributed::operator=(otherEntity);

			CopyPrivateDataMembers(otherEntity);
		}

		return *this;
	}

	Entity::Entity(Entity&& previousEntity) :
		Attributed(std::move(previousEntity)), mName(previousEntity.mName), mSector(previousEntity.mSector)
	{
		CopyPrivateDataMembers(previousEntity);

		SetSector(*previousEntity.mSector);

		previousEntity.mSector = nullptr;
		previousEntity.mName = std::string();
	}

	Entity& Entity::operator=(Entity&& previousEntity)
	{
		if (this != &previousEntity)
		{
			Attributed::operator=(std::move(previousEntity));

			CopyPrivateDataMembers(previousEntity);

			SetSector(*previousEntity.mSector);

			previousEntity.mSector = nullptr;
			previousEntity.mName = std::string();
		}

		return *this;
	}

	const std::string& Entity::Name() const
	{
		return mName;
	}

	void Entity::SetName(const std::string& name)
	{
		mName = name;
	}

	Sector* Entity::GetSector() const
	{
		return mSector;
	}

	void Entity::SetSector(Sector& sector)
	{
		sector.Adopt(*this, Sector::sSectorEntitiesKey);
		mSector = &sector;
	}

	Datum& Entity::Actions()
	{
		return const_cast<Datum&>(const_cast<const Entity*>(this)->Actions());
	}

	const Datum& Entity::Actions() const
	{
		const Datum* datum = Find(Action::sActionsKey);
		assert(datum != nullptr);
		return *datum;
	}

	Datum& Entity::Reactions()
	{
		return const_cast<Datum&>(const_cast<const Entity*>(this)->Reactions());
	}

	const Datum& Entity::Reactions() const
	{
		const Datum* datum = Find(Reaction::sReactionsKey);
		assert(datum != nullptr);
		return *datum;
	}

	Action* Entity::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Action* action = Factory<Action>::Create(className);
		action->SetName(instanceName);
		Adopt(*action, Action::sActionsKey);

		return action;
	}

	Datum& Entity::Entities()
	{
		return const_cast<Datum&>(const_cast<const Entity*>(this)->Entities());
	}

	const Datum& Entity::Entities() const
	{
		const Datum* datum = Find(Sector::sSectorEntitiesKey);
		assert(datum != nullptr);
		return *datum;
	}

	void Entity::Update(WorldState& worldState)
	{
		worldState.mEntity = this;

		Datum& entities = Entities();

		for (std::uint32_t i = 0; i < entities.Size(); ++i)
		{
			static_cast<Entity*>(&entities.Get<Scope&>(i))->Update(worldState);
		}

		Datum& actions = Actions();

		for (std::uint32_t i = 0; i < actions.Size(); ++i)
		{
			static_cast<Action*>(&actions.Get<Scope&>(i))->Update(worldState);
		}
	}

	void Entity::SetRelativePosition(glm::vec3 position)
	{
		mPosition = position;
	}

	void Entity::SetRelativeRotation(glm::vec3 rotation)
	{
		mRotation = rotation;
	}

	void Entity::SetRelativeScale(glm::vec3 scale)
	{
		mScale = scale;
	}

	void Entity::SetWorldPosition(glm::vec3 position)
	{
		if (GetParent() == nullptr || !GetParent()->Is(Entity::TypeIdClass()))
		{
			mPosition = position;
		}
		else
		{
			mPosition = position - GetParent()->As<Entity>()->GetWorldPosition();
		}
	}

	void Entity::SetWorldRotation(glm::vec3 rotation)
	{
		if (GetParent() == nullptr || !GetParent()->Is(Entity::TypeIdClass()))
		{
			mRotation = rotation;
		}
		else
		{
			mRotation = rotation - GetParent()->As<Entity>()->GetWorldRotation();
		}
	}

	void Entity::SetWorldScale(glm::vec3 scale)
	{
		if (GetParent() == nullptr || !GetParent()->Is(Entity::TypeIdClass()))
		{
			mScale = scale;
		}
		else
		{
			mScale = scale / GetParent()->As<Entity>()->GetWorldScale();
		}
	}

	glm::vec3 Entity::GetWorldPosition() const
	{
		if (GetParent() == nullptr)
		{
			return mPosition;
		}

		Entity* parent = GetParent()->As<Entity>();

		if (parent != nullptr)
		{
			return parent->GetWorldPosition() + mPosition;
		}

		return mPosition;
	}

	glm::vec3 Entity::GetWorldRotation() const
	{
		if (GetParent() == nullptr)
		{
			return mRotation;
		}

		Entity* parent = GetParent()->As<Entity>();

		if (parent != nullptr)
		{
			return parent->GetWorldRotation() + mRotation;
		}

		return mRotation;
	}

	glm::vec3 Entity::GetWorldScale() const
	{
		if (GetParent() == nullptr)
		{
			return mScale;
		}

		Entity* parent = GetParent()->As<Entity>();

		if (parent != nullptr)
		{
			return parent->GetWorldScale() * mScale;
		}

		return mScale;
	}

	glm::vec3 Entity::GetRelativePosition() const
	{
		return mPosition;
	}

	glm::vec3 Entity::GetRelativeRotation() const
	{
		return mRotation;
	}

	glm::vec3 Entity::GetRelativeScale() const
	{
		return mScale;
	}

	void Entity::Translate(glm::vec3 delta)
	{
		mPosition += delta;
	}

	void  Entity::Rotate(glm::vec3 delta)
	{
		mRotation += delta;
	}

	void  Entity::Scale(glm::vec3 delta)
	{
		mScale += delta;
	}

	void Entity::Render(Renderer* renderer)
	{
		for (auto& renderable : mRenderables)
		{
			renderable->Render(renderer);
		}

		Datum& entities = Entities();

		for (std::uint32_t i = 0; i < entities.Size(); ++i)
		{
			static_cast<Entity*>(&entities.Get<Scope&>(i))->Render(renderer);
		}
	}

	Scope* Entity::Copy() const
	{
		return new Entity(*this);
	}

	void Entity::InitializeSignatures()
	{
		Attributed::InitializeSignatures();

		AddExternalAttribute(sEntityNameKey, &mName, 1);
		AddExternalAttribute(sTransformPositionKey, &mPositionVec4, 1);
		AddExternalAttribute(sTransformRotationKey, &mRotationVec4, 1);
		AddExternalAttribute(sTransformScaleKey, &mScaleVec4, 1);

		AddEmptyTable(Action::sActionsKey);
		AddEmptyTable(Reaction::sReactionsKey);
		AddEmptyTable(Sector::sSectorEntitiesKey);
		AddEmptyString(sMeshesKey);
	}

	void Entity::AddRenderable(Renderable& renderable)
	{
		mRenderables.PushBack(&renderable);
	}

	void Entity::RemoveRenderable(Renderable& renderable)
	{
		mRenderables.Remove(&renderable);
	}

	void Entity::RemoveAllRenderables()
	{
		mRenderables.Clear();
	}

	Entity* Entity::FindChildEntityByName(const std::string& name)
	{
		Datum& entities = Entities();

		if (entities.Size() == 0)
		{
			return nullptr;
		}

		for (std::uint32_t i = 0; i < entities.Size(); ++i)
		{
			Entity* entity = entities.Get<Scope&>(i).As<Entity>();

			if (entity->mName == name)
			{
				return entity;
			}

			Entity* childFind = entity->FindChildEntityByName(name);

			if (childFind != nullptr)
			{
				return childFind;
			}
		}

		return nullptr;
	}

	void Entity::CopyPrivateDataMembers(const Entity& otherEntity)
	{
		mName = otherEntity.mName;
		mSector = nullptr;

		mPosition = otherEntity.mPosition;
		mRotation = otherEntity.mRotation;
		mScale = otherEntity.mScale;

		mPositionVec4 = otherEntity.mPositionVec4;
		mRotationVec4 = otherEntity.mRotationVec4;
		mScaleVec4 = otherEntity.mScaleVec4;

		FixExternalAttributes();
	}

	void Entity::FixExternalAttributes()
	{
		Append(sEntityNameKey).SetStorage(&mName, 1);
		Append(sTransformPositionKey).SetStorage(&mPositionVec4, 1);
		Append(sTransformRotationKey).SetStorage(&mRotationVec4, 1);
		Append(sTransformScaleKey).SetStorage(&mScaleVec4, 1);
	}

	void Entity::Initialize(WorldState& worldState)
	{
		mPosition.x = mPositionVec4.x;
		mPosition.y = mPositionVec4.y;
		mPosition.z = mPositionVec4.z;

		mRotation.x = mRotationVec4.x;
		mRotation.y = mRotationVec4.y;
		mRotation.z = mRotationVec4.z;

		mScale.x = mScaleVec4.x;
		mScale.y = mScaleVec4.y;
		mScale.z = mScaleVec4.z;

		worldState.mEntity = this;

		Datum& entities = Entities();

		for (std::uint32_t i = 0; i < entities.Size(); ++i)
		{
			entities.Get<Scope&>(i).As<Entity>()->Initialize(worldState);
		}
	}

	const std::string Entity::sEntityNameKey = "name";

	const std::string Entity::sTransformPositionKey = "position";

	const std::string Entity::sTransformRotationKey = "rotation";

	const std::string Entity::sTransformScaleKey = "scale";

	const std::string Entity::sMeshesKey = "meshes";
}