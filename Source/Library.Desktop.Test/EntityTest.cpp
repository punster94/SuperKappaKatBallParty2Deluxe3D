#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(EntityTest)
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

		TEST_METHOD(EntityConstructor)
		{
			std::string name = "hello";
			Entity entity;

			Assert::IsNull(entity.GetSector());
			Assert::IsTrue(entity.IsPrescribedAttribute(Entity::sEntityNameKey));
			Assert::IsTrue(entity.IsPrescribedAttribute(Action::sActionsKey));

			Entity otherEntity(name);

			Assert::IsNull(otherEntity.GetSector());
			Assert::IsTrue(otherEntity.Name() == name);
		}

		TEST_METHOD(EntityDestructor)
		{
			std::string name = "hello";
			Entity* entity = new Entity();

			delete(entity);

			entity = new Entity(name);
			entity->AppendAuxiliaryAttribute(name) = name;

			Assert::IsTrue(entity->IsAuxiliaryAttribute(name));

			delete(entity);
		}

		TEST_METHOD(EntityCopyConstructor)
		{
			std::string name = "hello";
			Sector sector;
			Entity entity(name);

			entity.SetSector(sector);

			Entity otherEntity = entity;

			Assert::IsTrue(otherEntity == entity);
			Assert::IsTrue(otherEntity.Name() == entity.Name());
			Assert::IsNull(otherEntity.GetSector());
			Assert::IsTrue(entity.GetSector() == &sector);
			Assert::AreEqual(1U, sector.Entities().Size());
		}

		TEST_METHOD(EntityAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			Sector sector;
			Entity entity(name);

			entity.SetSector(sector);

			Entity otherEntity(otherName);
			otherEntity = entity;

			Assert::IsTrue(otherEntity == entity);
			Assert::IsTrue(otherEntity.Name() == entity.Name());
			Assert::IsNull(otherEntity.GetSector());
			Assert::IsTrue(entity.GetSector() == &sector);
			Assert::AreEqual(1U, sector.Entities().Size());

			otherEntity = otherEntity;

			Assert::IsTrue(otherEntity == entity);
			Assert::IsTrue(otherEntity.Name() == entity.Name());
			Assert::IsNull(otherEntity.GetSector());
			Assert::IsTrue(entity.GetSector() == &sector);
			Assert::AreEqual(1U, sector.Entities().Size());
		}

		TEST_METHOD(EntityMoveConstructor)
		{
			std::string name = "hello";
			Sector sector;
			Entity entity(name);

			entity.SetSector(sector);

			Entity otherEntity = std::move(entity);

			Assert::IsTrue(otherEntity.Name() == name);
			Assert::IsTrue(otherEntity.GetSector() == &sector);
			Assert::IsTrue(otherEntity.IsPrescribedAttribute(Entity::sEntityNameKey));
			Assert::IsTrue(otherEntity.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsNull(entity.GetSector());
			Assert::IsTrue(entity.Name() == std::string());
			Assert::AreEqual(1U, sector.Entities().Size());
		}

		TEST_METHOD(EntityMoveAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			Sector sector;
			Entity entity(name);

			entity.SetSector(sector);

			Entity otherEntity(otherName);

			otherEntity = std::move(entity);

			Assert::IsTrue(otherEntity.Name() == name);
			Assert::IsTrue(otherEntity.GetSector() == &sector);
			Assert::IsTrue(otherEntity.IsPrescribedAttribute(Entity::sEntityNameKey));
			Assert::IsTrue(otherEntity.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsNull(entity.GetSector());
			Assert::IsTrue(entity.Name() == std::string());
			Assert::AreEqual(1U, sector.Entities().Size());

			otherEntity = std::move(otherEntity);

			Assert::IsTrue(otherEntity.Name() == name);
			Assert::IsTrue(otherEntity.GetSector() == &sector);
			Assert::IsTrue(otherEntity.IsPrescribedAttribute(Entity::sEntityNameKey));
			Assert::IsTrue(otherEntity.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsNull(entity.GetSector());
			Assert::IsTrue(entity.Name() == std::string());
			Assert::AreEqual(1U, sector.Entities().Size());
		}

		TEST_METHOD(EntityName)
		{
			std::string name = "hello", otherName = "howdy";
			Entity entity(name);

			Assert::IsTrue(entity.Name() == name);

			entity.SetName(otherName);

			Assert::IsTrue(entity.Name() == otherName);
		}

		TEST_METHOD(EntitySetName)
		{
			std::string name = "hello", otherName = "howdy";
			Entity entity;

			entity.SetName(name);

			Assert::IsTrue(entity.Name() == name);

			entity.SetName(otherName);

			Assert::IsTrue(entity.Name() == otherName);
		}

		TEST_METHOD(EntityGetSector)
		{
			Sector sector;
			Entity entity;

			Assert::IsNull(entity.GetSector());

			entity.SetSector(sector);

			Assert::IsTrue(entity.GetSector() == &sector);
		}

		TEST_METHOD(EntitySetSector)
		{
			Sector sector, otherSector;
			Entity entity;

			entity.SetSector(sector);

			Assert::IsTrue(entity.GetSector() == &sector);

			entity.SetSector(otherSector);

			Assert::IsTrue(entity.GetSector() == &otherSector);
		}

		TEST_METHOD(EntityActions)
		{
			std::string name = "hello", otherName = "howdy";
			Entity entity;

			Assert::AreEqual(0U, entity.Actions().Size());

			entity.CreateAction("ActionList", name);
			entity.CreateAction("ActionList", otherName);

			Assert::AreEqual(2U, entity.Actions().Size());
			Assert::IsTrue(entity.Actions().Get<Scope&>()[Action::sActionNameKey] == name);
			Assert::IsTrue(entity.Actions().Get<Scope&>(1)[Action::sActionNameKey] == otherName);

			const Entity otherEntity = entity;

			Assert::AreEqual(2U, otherEntity.Actions().Size());
			Assert::IsTrue(otherEntity.Actions().Get<const Scope&>()[Action::sActionNameKey] == name);
			Assert::IsTrue(otherEntity.Actions().Get<const Scope&>(1)[Action::sActionNameKey] == otherName);
		}

		TEST_METHOD(EntityCreateAction)
		{
			std::string name = "hello", otherName = "howdy";
			Entity entity;

			Action* action = entity.CreateAction("ActionList", name);
			Action* otherAction = entity.CreateAction("ActionList", otherName);

			Assert::IsTrue(action->Name() == name);
			Assert::IsTrue(action->GetParent() == &entity);
			Assert::IsTrue(otherAction->Name() == otherName);
			Assert::IsTrue(otherAction->GetParent() == &entity);
			Assert::IsTrue(entity.Actions().Get<Scope&>()[Action::sActionNameKey] == name);
			Assert::IsTrue(entity.Actions().Get<Scope&>(1)[Action::sActionNameKey] == otherName);
		}

		TEST_METHOD(EntityReactions)
		{
			std::string name = "hello";
			Entity entity;

			Assert::AreEqual(0U, entity.Reactions().Size());

			ReactionAttributed reaction(name);

			entity.Adopt(reaction, Reaction::sReactionsKey);

			Assert::AreEqual(1U, entity.Reactions().Size());
			Assert::IsTrue(entity.Reactions().Get<Scope&>()[Action::sActionNameKey] == name);

			const Entity otherEntity = entity;

			Assert::AreEqual(1U, otherEntity.Reactions().Size());
			Assert::IsTrue(otherEntity.Reactions().Get<const Scope&>()[Action::sActionNameKey] == name);
		}

		TEST_METHOD(EntityUpdate)
		{
			std::string name = "hello";
			WorldState worldState;
			Entity entity;

			entity.Update(worldState);

			Assert::IsTrue(worldState.mEntity == &entity);
			Assert::IsNull(worldState.mAction);

			Action* action = entity.CreateAction("ActionList", name);

			entity.Update(worldState);

			Assert::IsTrue(worldState.mEntity == &entity);
			Assert::IsTrue(worldState.mAction == action);
		}

		TEST_METHOD(EntityCopy)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello", unknownStringValue = "Unknown";
			Entity entity, childOne, childTwo, childThree;

			entity.Adopt(childOne, stringValue);
			entity.Adopt(childTwo, nextStringValue);
			entity.Adopt(childThree, thirdStringValue);

			Scope* copy = entity.Copy();

			Assert::IsTrue(*copy == entity);
			Assert::IsTrue(copy->Find(stringValue)->Get<Scope&>().Is(Entity::TypeIdClass()));
			Assert::IsTrue(copy->Find(nextStringValue)->Get<Scope&>().Is(Entity::TypeIdClass()));
			Assert::IsTrue(copy->Find(thirdStringValue)->Get<Scope&>().Is(Entity::TypeIdClass()));

			delete(copy);
		}

		TEST_METHOD(EntityRTTIMethods)
		{
			Entity entity;

			Assert::IsTrue(entity.Is(Entity::TypeIdClass()));
			Assert::IsTrue(entity.Is(Entity::TypeName()));
			Assert::AreEqual(Entity::TypeIdClass(), entity.TypeIdInstance());
			Assert::AreEqual(Entity::TypeName(), entity.TypeName());

			RTTI* asRTTI = &entity;

			Assert::IsFalse(nullptr == asRTTI->QueryInterface(Entity::TypeIdClass()));
			Assert::IsTrue(entity == *(asRTTI->As<Entity>()));
			Assert::AreEqual(asRTTI->As<Entity>()->ToString(), entity.ToString());
		}

	private:

		static _CrtMemState sStartMemState;

		ActionListFactory mActionListFactory;
	};

	_CrtMemState EntityTest::sStartMemState;
}
