#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(WorldTest)
	{
	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			Attributed::ClearPrescribedAttributes();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);

			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}

		TEST_METHOD(WorldConstructor)
		{
			std::string name = "hello";
			World world;

			Assert::IsTrue(world.IsPrescribedAttribute(World::sWorldNameKey));
			Assert::IsTrue(world.IsPrescribedAttribute(World::sWorldSectorsKey));
			Assert::IsTrue(world.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsTrue(world.Name() == std::string());

			World otherWorld(name);

			Assert::IsTrue(otherWorld.Name() == name);
		}

		TEST_METHOD(WorldDestructor)
		{
			std::string name = "hello";
			World* world = new World();

			delete(world);

			world = new World(name);
			world->AppendAuxiliaryAttribute(name) = name;

			Assert::IsTrue(world->IsAuxiliaryAttribute(name));

			delete(world);
		}

		TEST_METHOD(WorldCopyConstructor)
		{
			std::string name = "hello";
			World world(name);

			World otherWorld = world;

			Assert::IsTrue(otherWorld == world);
			Assert::IsTrue(otherWorld.Name() == world.Name());
		}

		TEST_METHOD(WorldAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			World world(name);

			World otherWorld(otherName);
			otherWorld = world;

			Assert::IsTrue(otherWorld == world);
			Assert::IsTrue(otherWorld.Name() == world.Name());

			otherWorld = otherWorld;

			Assert::IsTrue(otherWorld == world);
			Assert::IsTrue(otherWorld.Name() == world.Name());
		}

		TEST_METHOD(WorldMoveConstructor)
		{
			std::string name = "hello";
			World world(name);

			World otherWorld = std::move(world);

			Assert::IsTrue(otherWorld.Name() == name);
			Assert::IsTrue(otherWorld.IsPrescribedAttribute(World::sWorldNameKey));
			Assert::IsTrue(otherWorld.IsPrescribedAttribute(World::sWorldSectorsKey));
			Assert::IsTrue(otherWorld.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsTrue(world.Name() == std::string());
		}

		TEST_METHOD(EntityMoveAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			World world(name);

			World otherWorld(otherName);
			otherWorld = std::move(world);

			Assert::IsTrue(otherWorld.Name() == name);
			Assert::IsTrue(otherWorld.IsPrescribedAttribute(World::sWorldNameKey));
			Assert::IsTrue(otherWorld.IsPrescribedAttribute(World::sWorldSectorsKey));
			Assert::IsTrue(otherWorld.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsTrue(world.Name() == std::string());

			otherWorld = std::move(otherWorld);

			Assert::IsTrue(otherWorld.Name() == name);
			Assert::IsTrue(otherWorld.IsPrescribedAttribute(World::sWorldNameKey));
			Assert::IsTrue(otherWorld.IsPrescribedAttribute(World::sWorldSectorsKey));
			Assert::IsTrue(otherWorld.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsTrue(world.Name() == std::string());
		}

		TEST_METHOD(WorldName)
		{
			std::string name = "hello", otherName = "howdy";
			World world(name);

			Assert::IsTrue(world.Name() == name);

			world.SetName(otherName);

			Assert::IsTrue(world.Name() == otherName);
		}

		TEST_METHOD(WorldSetName)
		{
			std::string name = "hello", otherName = "howdy";
			World world;

			world.SetName(name);

			Assert::IsTrue(world.Name() == name);

			world.SetName(otherName);

			Assert::IsTrue(world.Name() == otherName);
		}

		TEST_METHOD(WorldSectors)
		{
			std::string name = "hello", otherName = "howdy";
			World world;

			Assert::AreEqual(0U, world.Sectors().Size());

			world.CreateSector(name);
			world.CreateSector(otherName);

			Assert::AreEqual(2U, world.Sectors().Size());
			Assert::IsTrue(world.Sectors().Get<Scope&>()[Sector::sSectorNameKey] == name);
			Assert::IsTrue(world.Sectors().Get<Scope&>(1)[Sector::sSectorNameKey] == otherName);

			const World otherWorld = world;

			Assert::AreEqual(2U, otherWorld.Sectors().Size());
			Assert::IsTrue(otherWorld.Sectors().Get<const Scope&>()[Sector::sSectorNameKey] == name);
			Assert::IsTrue(otherWorld.Sectors().Get<const Scope&>(1)[Sector::sSectorNameKey] == otherName);
		}

		TEST_METHOD(WorldCreateSector)
		{
			std::string name = "hello", otherName = "howdy";
			World world;

			Sector* sector = world.CreateSector(name);
			Sector* otherSector = world.CreateSector(otherName);

			Assert::IsTrue(sector->Name() == name);
			Assert::IsTrue(sector->GetWorld() == &world);
			Assert::IsTrue(otherSector->Name() == otherName);
			Assert::IsTrue(otherSector->GetWorld() == &world);
			Assert::IsTrue(world.Sectors().Get<Scope&>()[Sector::sSectorNameKey] == name);
			Assert::IsTrue(world.Sectors().Get<Scope&>(1)[Sector::sSectorNameKey] == otherName);
		}

		TEST_METHOD(WorldActions)
		{
			std::string name = "hello", otherName = "howdy";
			World world;

			Assert::AreEqual(0U, world.Actions().Size());

			world.CreateAction("ActionList", name);
			world.CreateAction("ActionList", otherName);

			Assert::AreEqual(2U, world.Actions().Size());
			Assert::IsTrue(world.Actions().Get<Scope&>()[Action::sActionNameKey] == name);
			Assert::IsTrue(world.Actions().Get<Scope&>(1)[Action::sActionNameKey] == otherName);

			const World otherWorld = world;

			Assert::AreEqual(2U, otherWorld.Actions().Size());
			Assert::IsTrue(otherWorld.Actions().Get<const Scope&>()[Action::sActionNameKey] == name);
			Assert::IsTrue(otherWorld.Actions().Get<const Scope&>(1)[Action::sActionNameKey] == otherName);
		}

		TEST_METHOD(WorldCreateAction)
		{
			std::string name = "hello", otherName = "howdy";
			World world;

			Action* action = world.CreateAction("ActionList", name);
			Action* otherAction = world.CreateAction("ActionList", otherName);

			Assert::IsTrue(action->Name() == name);
			Assert::IsTrue(action->GetParent() == &world);
			Assert::IsTrue(otherAction->Name() == otherName);
			Assert::IsTrue(otherAction->GetParent() == &world);
			Assert::IsTrue(world.Actions().Get<Scope&>()[Action::sActionNameKey] == name);
			Assert::IsTrue(world.Actions().Get<Scope&>(1)[Action::sActionNameKey] == otherName);
		}

		TEST_METHOD(WorldReactions)
		{
			std::string name = "hello";
			World world;

			Assert::AreEqual(0U, world.Reactions().Size());

			ReactionAttributed reaction(name);

			world.Adopt(reaction, Reaction::sReactionsKey);

			Assert::AreEqual(1U, world.Reactions().Size());
			Assert::IsTrue(world.Reactions().Get<Scope&>()[Action::sActionNameKey] == name);

			const World otherWorld = world;

			Assert::AreEqual(1U, otherWorld.Reactions().Size());
			Assert::IsTrue(otherWorld.Reactions().Get<const Scope&>()[Action::sActionNameKey] == name);
		}

		TEST_METHOD(WorldEnqueue)
		{
			std::string subtype = "name";
			std::int32_t delay = rand();
			WorldState worldState;
			World world;
			EventMessageAttributed message(subtype);
			Event<EventMessageAttributed>* e = new Event<EventMessageAttributed>(message);

			world.Enqueue(*e, worldState, delay);

			Assert::AreEqual(1U, world.QueueSize());
		}

		TEST_METHOD(WorldQueueSize)
		{
			std::string subtype = "name";
			std::int32_t delay = rand();
			WorldState worldState;
			World world;
			EventMessageAttributed message(subtype);
			Event<EventMessageAttributed>* e = new Event<EventMessageAttributed>(message);

			Assert::AreEqual(0U, world.QueueSize());

			world.Enqueue(*e, worldState, delay);

			Assert::AreEqual(1U, world.QueueSize());
		}

		TEST_METHOD(WorldUpdate)
		{
			std::string name = "hello";
			WorldState worldState;
			World world;

			world.Update(worldState);

			Assert::IsTrue(worldState.mWorld == &world);
			Assert::IsNull(worldState.mSector);
			Assert::IsNull(worldState.mEntity);

			Sector* sector = world.CreateSector(name);

			world.Update(worldState);

			Assert::IsTrue(worldState.mWorld == &world);
			Assert::IsTrue(worldState.mSector == sector);
			Assert::IsNull(worldState.mEntity);

			Entity* entity = sector->CreateEntity("Entity", name);

			world.Update(worldState);

			Assert::IsTrue(worldState.mWorld == &world);
			Assert::IsTrue(worldState.mSector == sector);
			Assert::IsTrue(worldState.mEntity == entity);
		}

		TEST_METHOD(WorldCopy)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello", unknownStringValue = "Unknown";
			World world, childOne, childTwo, childThree;

			world.Adopt(childOne, stringValue);
			world.Adopt(childTwo, nextStringValue);
			world.Adopt(childThree, thirdStringValue);

			Scope* copy = world.Copy();

			Assert::IsTrue(*copy == world);
			Assert::IsTrue(copy->Find(stringValue)->Get<Scope&>().Is(World::TypeIdClass()));
			Assert::IsTrue(copy->Find(nextStringValue)->Get<Scope&>().Is(World::TypeIdClass()));
			Assert::IsTrue(copy->Find(thirdStringValue)->Get<Scope&>().Is(World::TypeIdClass()));

			delete(copy);
		}

		TEST_METHOD(WorldRTTIMethods)
		{
			World world;

			Assert::IsTrue(world.Is(World::TypeIdClass()));
			Assert::IsTrue(world.Is(World::TypeName()));
			Assert::AreEqual(World::TypeIdClass(), world.TypeIdInstance());
			Assert::AreEqual(World::TypeName(), world.TypeName());

			RTTI* asRTTI = &world;

			Assert::IsFalse(nullptr == asRTTI->QueryInterface(World::TypeIdClass()));
			Assert::IsTrue(world == *(asRTTI->As<World>()));
			Assert::AreEqual(asRTTI->As<World>()->ToString(), world.ToString());
		}

	private:

		static _CrtMemState sStartMemState;

		EntityFactory mEntityFactory;

		ActionListFactory mActionListFactory;
	};

	_CrtMemState WorldTest::sStartMemState;
}
