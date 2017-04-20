#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(SectorTest)
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

		TEST_METHOD(SectorConstructor)
		{
			std::string name = "hello";
			Sector sector;

			Assert::IsNull(sector.GetWorld());
			Assert::IsTrue(sector.IsPrescribedAttribute(Sector::sSectorNameKey));
			Assert::IsTrue(sector.IsPrescribedAttribute(Sector::sSectorEntitiesKey));
			Assert::IsTrue(sector.IsPrescribedAttribute(Action::sActionsKey));

			Sector otherSector(name);

			Assert::IsNull(otherSector.GetWorld());
			Assert::IsTrue(otherSector.Name() == name);
		}

		TEST_METHOD(SectorDestructor)
		{
			std::string name = "hello";
			Sector* sector = new Sector();

			delete(sector);

			sector = new Sector(name);
			sector->AppendAuxiliaryAttribute(name) = name;

			Assert::IsTrue(sector->IsAuxiliaryAttribute(name));

			delete(sector);
		}

		TEST_METHOD(SectorCopyConstructor)
		{
			std::string name = "hello";
			World world;
			Sector sector(name);

			sector.SetWorld(world);

			Sector otherSector = sector;

			Assert::IsTrue(otherSector == sector);
			Assert::IsTrue(otherSector.Name() == sector.Name());
			Assert::IsNull(otherSector.GetWorld());
			Assert::AreEqual(1U, world.Sectors().Size());
		}

		TEST_METHOD(SectorAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			World world;
			Sector sector(name);

			sector.SetWorld(world);

			Sector otherSector(otherName);
			otherSector = sector;

			Assert::IsTrue(otherSector == sector);
			Assert::IsTrue(otherSector.Name() == sector.Name());
			Assert::IsNull(otherSector.GetWorld());
			Assert::AreEqual(1U, world.Sectors().Size());

			otherSector = otherSector;

			Assert::IsTrue(otherSector == sector);
			Assert::IsTrue(otherSector.Name() == sector.Name());
			Assert::IsNull(otherSector.GetWorld());
			Assert::AreEqual(1U, world.Sectors().Size());
		}

		TEST_METHOD(SectorMoveConstructor)
		{
			std::string name = "hello";
			World world;
			Sector sector(name);

			sector.SetWorld(world);

			Sector otherSector = std::move(sector);

			Assert::IsTrue(otherSector.Name() == name);
			Assert::IsTrue(otherSector.GetWorld() == &world);
			Assert::IsTrue(otherSector.IsPrescribedAttribute(Sector::sSectorNameKey));
			Assert::IsTrue(otherSector.IsPrescribedAttribute(Sector::sSectorEntitiesKey));
			Assert::IsTrue(otherSector.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsNull(sector.GetWorld());
			Assert::IsTrue(sector.Name() == std::string());
			Assert::AreEqual(1U, world.Sectors().Size());
		}

		TEST_METHOD(SectorMoveAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			World world;
			Sector sector(name);

			sector.SetWorld(world);

			Sector otherSector(otherName);

			otherSector = std::move(sector);

			Assert::IsTrue(otherSector.Name() == name);
			Assert::IsTrue(otherSector.GetWorld() == &world);
			Assert::IsTrue(otherSector.IsPrescribedAttribute(Sector::sSectorNameKey));
			Assert::IsTrue(otherSector.IsPrescribedAttribute(Sector::sSectorEntitiesKey));
			Assert::IsTrue(otherSector.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsNull(sector.GetWorld());
			Assert::IsTrue(sector.Name() == std::string());
			Assert::AreEqual(1U, world.Sectors().Size());

			otherSector = std::move(otherSector);

			Assert::IsTrue(otherSector.Name() == name);
			Assert::IsTrue(otherSector.GetWorld() == &world);
			Assert::IsTrue(otherSector.IsPrescribedAttribute(Sector::sSectorNameKey));
			Assert::IsTrue(otherSector.IsPrescribedAttribute(Sector::sSectorEntitiesKey));
			Assert::IsTrue(otherSector.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsNull(sector.GetWorld());
			Assert::IsTrue(sector.Name() == std::string());
			Assert::AreEqual(1U, world.Sectors().Size());
		}

		TEST_METHOD(SectorName)
		{
			std::string name = "hello", otherName = "howdy";
			Sector sector(name);

			Assert::IsTrue(sector.Name() == name);

			sector.SetName(otherName);

			Assert::IsTrue(sector.Name() == otherName);
		}

		TEST_METHOD(SectorSetName)
		{
			std::string name = "hello", otherName = "howdy";
			Sector sector;

			sector.SetName(name);

			Assert::IsTrue(sector.Name() == name);

			sector.SetName(otherName);

			Assert::IsTrue(sector.Name() == otherName);
		}

		TEST_METHOD(SectorGetWorld)
		{
			World world;
			Sector sector;

			Assert::IsNull(sector.GetWorld());

			sector.SetWorld(world);

			Assert::IsTrue(sector.GetWorld() == &world);
		}

		TEST_METHOD(SectorSetWorld)
		{
			World world, otherWorld;
			Sector sector;

			sector.SetWorld(world);

			Assert::IsTrue(sector.GetWorld() == &world);

			sector.SetWorld(otherWorld);

			Assert::IsTrue(sector.GetWorld() == &otherWorld);
		}

		TEST_METHOD(SectorEntities)
		{
			std::string name = "hello", otherName = "howdy";
			Sector sector;

			Assert::AreEqual(0U, sector.Entities().Size());

			sector.CreateEntity("Entity", name);
			sector.CreateEntity("Entity", otherName);

			Assert::AreEqual(2U, sector.Entities().Size());
			Assert::IsTrue(sector.Entities().Get<Scope&>()[Entity::sEntityNameKey] == name);
			Assert::IsTrue(sector.Entities().Get<Scope&>(1)[Entity::sEntityNameKey] == otherName);

			const Sector otherSector = sector;

			Assert::AreEqual(2U, otherSector.Entities().Size());
			Assert::IsTrue(otherSector.Entities().Get<const Scope&>()[Entity::sEntityNameKey] == name);
			Assert::IsTrue(otherSector.Entities().Get<const Scope&>(1)[Entity::sEntityNameKey] == otherName);
		}

		TEST_METHOD(SectorCreateEntity)
		{
			std::string name = "hello", otherName = "howdy";
			Sector sector;

			Entity* entity = sector.CreateEntity("Entity", name);
			Entity* otherEntity = sector.CreateEntity("Entity", otherName);

			Assert::IsTrue(entity->Name() == name);
			Assert::IsTrue(entity->GetSector() == &sector);
			Assert::IsTrue(otherEntity->Name() == otherName);
			Assert::IsTrue(otherEntity->GetSector() == &sector);
			Assert::IsTrue(sector.Entities().Get<Scope&>()[Entity::sEntityNameKey] == name);
			Assert::IsTrue(sector.Entities().Get<Scope&>(1)[Entity::sEntityNameKey] == otherName);
		}

		TEST_METHOD(SectorActions)
		{
			std::string name = "hello", otherName = "howdy";
			Sector sector;

			Assert::AreEqual(0U, sector.Actions().Size());

			sector.CreateAction("ActionList", name);
			sector.CreateAction("ActionList", otherName);

			Assert::AreEqual(2U, sector.Actions().Size());
			Assert::IsTrue(sector.Actions().Get<Scope&>()[Action::sActionNameKey] == name);
			Assert::IsTrue(sector.Actions().Get<Scope&>(1)[Action::sActionNameKey] == otherName);

			const Sector otherSector = sector;

			Assert::AreEqual(2U, otherSector.Actions().Size());
			Assert::IsTrue(otherSector.Actions().Get<const Scope&>()[Action::sActionNameKey] == name);
			Assert::IsTrue(otherSector.Actions().Get<const Scope&>(1)[Action::sActionNameKey] == otherName);
		}

		TEST_METHOD(SectorCreateAction)
		{
			std::string name = "hello", otherName = "howdy";
			Sector sector;

			Action* action = sector.CreateAction("ActionList", name);
			Action* otherAction = sector.CreateAction("ActionList", otherName);

			Assert::IsTrue(action->Name() == name);
			Assert::IsTrue(action->GetParent() == &sector);
			Assert::IsTrue(otherAction->Name() == otherName);
			Assert::IsTrue(otherAction->GetParent() == &sector);
			Assert::IsTrue(sector.Actions().Get<Scope&>()[Action::sActionNameKey] == name);
			Assert::IsTrue(sector.Actions().Get<Scope&>(1)[Action::sActionNameKey] == otherName);
		}

		TEST_METHOD(SectorReactions)
		{
			std::string name = "hello";
			Sector sector;

			Assert::AreEqual(0U, sector.Reactions().Size());

			ReactionAttributed reaction(name);

			sector.Adopt(reaction, Reaction::sReactionsKey);

			Assert::AreEqual(1U, sector.Reactions().Size());
			Assert::IsTrue(sector.Reactions().Get<Scope&>()[Action::sActionNameKey] == name);

			const Sector otherSector= sector;

			Assert::AreEqual(1U, otherSector.Reactions().Size());
			Assert::IsTrue(otherSector.Reactions().Get<const Scope&>()[Action::sActionNameKey] == name);
		}

		TEST_METHOD(SectorUpdate)
		{
			std::string name = "hello";
			WorldState worldState;
			Sector sector;

			sector.Update(worldState);

			Assert::IsTrue(worldState.mSector == &sector);
			Assert::IsNull(worldState.mEntity);

			Entity* entity = sector.CreateEntity("Entity", name);

			sector.Update(worldState);

			Assert::IsTrue(worldState.mSector == &sector);
			Assert::IsTrue(worldState.mEntity == entity);
		}

		TEST_METHOD(SectorCopy)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello", unknownStringValue = "Unknown";
			Sector sector, childOne, childTwo, childThree;

			sector.Adopt(childOne, stringValue);
			sector.Adopt(childTwo, nextStringValue);
			sector.Adopt(childThree, thirdStringValue);

			Scope* copy = sector.Copy();

			Assert::IsTrue(*copy == sector);
			Assert::IsTrue(copy->Find(stringValue)->Get<Scope&>().Is(Sector::TypeIdClass()));
			Assert::IsTrue(copy->Find(nextStringValue)->Get<Scope&>().Is(Sector::TypeIdClass()));
			Assert::IsTrue(copy->Find(thirdStringValue)->Get<Scope&>().Is(Sector::TypeIdClass()));

			delete(copy);
		}

		TEST_METHOD(SectorRTTIMethods)
		{
			Sector sector;

			Assert::IsTrue(sector.Is(Sector::TypeIdClass()));
			Assert::IsTrue(sector.Is(Sector::TypeName()));
			Assert::AreEqual(Sector::TypeIdClass(), sector.TypeIdInstance());
			Assert::AreEqual(Sector::TypeName(), sector.TypeName());

			RTTI* asRTTI = &sector;

			Assert::IsFalse(nullptr == asRTTI->QueryInterface(Sector::TypeIdClass()));
			Assert::IsTrue(sector == *(asRTTI->As<Sector>()));
			Assert::AreEqual(asRTTI->As<Sector>()->ToString(), sector.ToString());
		}

	private:

		static _CrtMemState sStartMemState;

		EntityFactory mEntityFactory;

		ActionListFactory mActionListFactory;
	};

	_CrtMemState SectorTest::sStartMemState;
}
