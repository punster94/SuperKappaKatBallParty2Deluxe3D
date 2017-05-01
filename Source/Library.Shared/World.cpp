#include "pch.h"
#include "World.h"
#include "Reaction.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(World)

	World::World(const std::string& name) :
		Attributed(), mName(name)
	{
		InitializeSignatures();
	}

	World::World(const World& otherWorld) :
		Attributed(otherWorld), mName(otherWorld.mName)
	{
		CopyPrivateDataMembers(otherWorld);
	}

	World& World::operator=(const World& otherWorld)
	{
		if (this != &otherWorld)
		{
			Attributed::operator=(otherWorld);

			CopyPrivateDataMembers(otherWorld);
		}

		return *this;
	}

	World::World(World&& previousWorld) :
		Attributed(std::move(previousWorld)), mName(previousWorld.mName)
	{
		CopyPrivateDataMembers(previousWorld);

		previousWorld.mName = std::string();
	}

	World& World::operator=(World&& previousWorld)
	{
		if (this != &previousWorld)
		{
			Attributed::operator=(std::move(previousWorld));

			CopyPrivateDataMembers(previousWorld);

			previousWorld.mName = std::string();
		}

		return *this;
	}

	const std::string& World::Name() const
	{
		return mName;
	}

	void World::SetName(const std::string& name)
	{
		mName = name;
	}
	
	Datum& World::Sectors()
	{
		return const_cast<Datum&>(const_cast<const World*>(this)->Sectors());
	}

	const Datum& World::Sectors() const
	{
		const Datum* datum = Find(sWorldSectorsKey);
		assert(datum != nullptr);
		return *datum;
	}

	Sector* World::CreateSector(const std::string& instanceName)
	{
		Sector* sector = new Sector();
		sector->SetName(instanceName);
		sector->SetWorld(*this);

		return sector;
	}

	Datum& World::Actions()
	{
		return const_cast<Datum&>(const_cast<const World*>(this)->Actions());
	}

	const Datum& World::Actions() const
	{
		const Datum* datum = Find(Action::sActionsKey);
		assert(datum != nullptr);
		return *datum;
	}

	Action* World::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Action* action = Factory<Action>::Create(className);
		action->SetName(instanceName);
		Adopt(*action, Action::sActionsKey);

		return action;
	}

	Datum& World::Reactions()
	{
		return const_cast<Datum&>(const_cast<const World*>(this)->Reactions());
	}

	const Datum& World::Reactions() const
	{
		const Datum* datum = Find(Reaction::sReactionsKey);
		assert(datum != nullptr);
		return *datum;
	}

	void World::Update(WorldState& worldState)
	{
		worldState.mWorld = this;

		Datum& actions = Actions();

		for (std::uint32_t i = 0; i < actions.Size(); ++i)
		{
			static_cast<Action*>(&actions.Get<Scope&>(i))->Update(worldState);
		}

		Datum& sectors = Sectors();

		for (std::uint32_t i = 0; i < sectors.Size(); ++i)
		{
			static_cast<Sector*>(&sectors.Get<Scope&>(i))->Update(worldState);
		}

		mEventQueue.Update(worldState.mGameTime);
	}

	Scope* World::Copy() const
	{
		return new World(*this);
	}

	void World::Enqueue(EventPublisher& eventPublisher, WorldState& worldState, std::int32_t delay)
	{
		mEventQueue.Enqueue(eventPublisher, worldState.mGameTime, std::chrono::milliseconds(delay));
	}

	std::uint32_t World::QueueSize() const
	{
		return mEventQueue.Size();
	}

	void World::InitializeSignatures()
	{
		Attributed::InitializeSignatures();

		AddExternalAttribute(sWorldNameKey, &mName, 1);

		AddEmptyTable(sWorldSectorsKey);
		AddEmptyTable(Action::sActionsKey);
		AddEmptyTable(Reaction::sReactionsKey);
	}

	void World::CopyPrivateDataMembers(const World& otherWorld)
	{
		mName = otherWorld.mName;

		FixExternalAttributes();
	}

	void World::FixExternalAttributes()
	{
		Append(sWorldNameKey).SetStorage(&mName, 1);
	}

	void World::Initialize(WorldState& worldState)
	{
		worldState.mWorld = this;

		mEventQueue.Clear();

		Datum& sectors = Sectors();

		for (std::uint32_t i = 0; i < sectors.Size(); ++i)
		{
			static_cast<Sector*>(&sectors.Get<Scope&>(i))->Initialize(worldState);
		}
	}

	void World::Reset(WorldState& worldState)
	{
		worldState.mWorld = this;

		mEventQueue.Clear();

		Datum& sectors = Sectors();
		for(std::uint32_t i = 0; i < sectors.Size(); ++i)
		{
			static_cast<Sector*>(&sectors.Get<Scope&>(i))->Reset(worldState);
		}
	}

	const std::string World::sWorldNameKey = "name";

	const std::string World::sWorldSectorsKey = "sectors";
}