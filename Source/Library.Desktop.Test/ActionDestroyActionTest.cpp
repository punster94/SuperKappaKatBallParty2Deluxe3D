#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(ActionDestroyActionTest)
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

		TEST_METHOD(ActionDestroyActionConstructor)
		{
			std::string name = "hello";
			ActionDestroyAction actionDestroyAction(name);

			Assert::IsTrue(actionDestroyAction.Name() == name);
			Assert::IsTrue(actionDestroyAction[ActionDestroyAction::sActionDestroyActionInstanceNameKey] == "");
			Assert::IsTrue(actionDestroyAction.IsPrescribedAttribute(Attributed::sAttributedThisKey));
			Assert::IsTrue(actionDestroyAction.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(actionDestroyAction.IsPrescribedAttribute(ActionDestroyAction::sActionDestroyActionInstanceNameKey));

			ActionCreateAction otherActionDestroyAction(name);

			Assert::IsTrue(otherActionDestroyAction.Name() == name);
			Assert::IsTrue(otherActionDestroyAction[ActionDestroyAction::sActionDestroyActionInstanceNameKey] == "");
			Assert::IsTrue(otherActionDestroyAction.IsPrescribedAttribute(Attributed::sAttributedThisKey));
			Assert::IsTrue(otherActionDestroyAction.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(otherActionDestroyAction.IsPrescribedAttribute(ActionDestroyAction::sActionDestroyActionInstanceNameKey));
		}

		TEST_METHOD(ActionDestroyActionDestructor)
		{
			std::string name = "hello";
			ActionDestroyAction* actionDestroyAction = new ActionDestroyAction();

			delete(actionDestroyAction);

			actionDestroyAction = new ActionDestroyAction(name);
			actionDestroyAction->AppendAuxiliaryAttribute(name) = name;

			Assert::IsTrue(actionDestroyAction->IsAuxiliaryAttribute(name));

			delete(actionDestroyAction);
		}

		TEST_METHOD(ActionDestroyActionCopyConstructor)
		{
			std::string name = "hello";
			Entity entity;
			ActionDestroyAction actionDestroyAction(name);

			actionDestroyAction.SetInstanceName(name);
			entity.Adopt(actionDestroyAction, Action::sActionsKey);

			ActionDestroyAction otherActionDestroyAction = actionDestroyAction;

			Assert::IsTrue(otherActionDestroyAction == actionDestroyAction);
			Assert::IsTrue(otherActionDestroyAction.Name() == actionDestroyAction.Name());
			Assert::IsTrue(otherActionDestroyAction[ActionDestroyAction::sActionDestroyActionInstanceNameKey] == actionDestroyAction[ActionDestroyAction::sActionDestroyActionInstanceNameKey]);
			Assert::IsNull(otherActionDestroyAction.GetParent());
			Assert::IsTrue(actionDestroyAction.GetParent() == &entity);
			Assert::AreEqual(1U, entity.Actions().Size());
		}

		TEST_METHOD(ActionDestroyActionAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			Entity entity;
			ActionDestroyAction actionDestroyAction(name);

			actionDestroyAction.SetInstanceName(name);
			entity.Adopt(actionDestroyAction, Action::sActionsKey);

			ActionDestroyAction otherActionDestroyAction(otherName);
			otherActionDestroyAction = actionDestroyAction;

			Assert::IsTrue(otherActionDestroyAction == actionDestroyAction);
			Assert::IsTrue(otherActionDestroyAction.Name() == actionDestroyAction.Name());
			Assert::IsTrue(otherActionDestroyAction[ActionDestroyAction::sActionDestroyActionInstanceNameKey] == actionDestroyAction[ActionDestroyAction::sActionDestroyActionInstanceNameKey]);
			Assert::IsNull(otherActionDestroyAction.GetParent());
			Assert::IsTrue(actionDestroyAction.GetParent() == &entity);
			Assert::AreEqual(1U, entity.Actions().Size());

			otherActionDestroyAction = otherActionDestroyAction;

			Assert::IsTrue(otherActionDestroyAction == actionDestroyAction);
			Assert::IsTrue(otherActionDestroyAction.Name() == actionDestroyAction.Name());
			Assert::IsTrue(otherActionDestroyAction[ActionDestroyAction::sActionDestroyActionInstanceNameKey] == actionDestroyAction[ActionDestroyAction::sActionDestroyActionInstanceNameKey]);
			Assert::IsNull(otherActionDestroyAction.GetParent());
			Assert::IsTrue(actionDestroyAction.GetParent() == &entity);
			Assert::AreEqual(1U, entity.Actions().Size());
		}

		TEST_METHOD(ActionDestroyActionMoveConstructor)
		{
			std::string name = "hello";
			Entity entity;
			ActionDestroyAction actionDestroyAction(name);

			actionDestroyAction.SetInstanceName(name);
			entity.Adopt(actionDestroyAction, Action::sActionsKey);

			ActionDestroyAction otherActionDestroyAction = std::move(actionDestroyAction);

			Assert::IsTrue(otherActionDestroyAction.Name() == name);
			Assert::IsTrue(otherActionDestroyAction.GetParent() == &entity);
			Assert::IsTrue(otherActionDestroyAction.IsPrescribedAttribute(Attributed::sAttributedThisKey));
			Assert::IsTrue(otherActionDestroyAction.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(otherActionDestroyAction.IsPrescribedAttribute(ActionDestroyAction::sActionDestroyActionInstanceNameKey));
			Assert::IsTrue(otherActionDestroyAction[ActionDestroyAction::sActionDestroyActionInstanceNameKey] == name);
			Assert::IsNull(actionDestroyAction.GetParent());
			Assert::IsTrue(actionDestroyAction.Name() == std::string());
			Assert::AreEqual(1U, entity.Actions().Size());
		}

		TEST_METHOD(ActionDestroyActionMoveAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			Entity entity;
			ActionDestroyAction actionDestroyAction(name);

			actionDestroyAction.SetInstanceName(name);
			entity.Adopt(actionDestroyAction, Action::sActionsKey);

			ActionDestroyAction otherActionDestroyAction(otherName);

			otherActionDestroyAction = std::move(actionDestroyAction);

			Assert::IsTrue(otherActionDestroyAction.Name() == name);
			Assert::IsTrue(otherActionDestroyAction.GetParent() == &entity);
			Assert::IsTrue(otherActionDestroyAction.IsPrescribedAttribute(Attributed::sAttributedThisKey));
			Assert::IsTrue(otherActionDestroyAction.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(otherActionDestroyAction.IsPrescribedAttribute(ActionDestroyAction::sActionDestroyActionInstanceNameKey));
			Assert::IsTrue(otherActionDestroyAction[ActionDestroyAction::sActionDestroyActionInstanceNameKey] == name);
			Assert::IsNull(actionDestroyAction.GetParent());
			Assert::IsTrue(actionDestroyAction.Name() == std::string());
			Assert::AreEqual(1U, entity.Actions().Size());

			otherActionDestroyAction = std::move(otherActionDestroyAction);

			Assert::IsTrue(otherActionDestroyAction.Name() == name);
			Assert::IsTrue(otherActionDestroyAction.GetParent() == &entity);
			Assert::IsTrue(otherActionDestroyAction.IsPrescribedAttribute(Attributed::sAttributedThisKey));
			Assert::IsTrue(otherActionDestroyAction.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(otherActionDestroyAction.IsPrescribedAttribute(ActionDestroyAction::sActionDestroyActionInstanceNameKey));
			Assert::IsTrue(otherActionDestroyAction[ActionDestroyAction::sActionDestroyActionInstanceNameKey] == name);
			Assert::IsNull(actionDestroyAction.GetParent());
			Assert::IsTrue(actionDestroyAction.Name() == std::string());
			Assert::AreEqual(1U, entity.Actions().Size());
		}

		TEST_METHOD(ActionDestroyActionSetInstanceName)
		{
			std::string name = "hello";
			ActionDestroyAction actionDestroyAction;

			actionDestroyAction.SetInstanceName(name);

			Assert::IsTrue(actionDestroyAction[ActionDestroyAction::sActionDestroyActionInstanceNameKey] == name);
		}

		TEST_METHOD(ActionDestroyActionUpdate)
		{
			std::string name = "hello", otherName = "howdy";
			WorldState worldState;
			Entity entity;

			Action* action = entity.CreateAction("ActionDestroyAction", name);
			ActionDestroyAction* actionDestroyAction = action->As<ActionDestroyAction>();

			action = entity.CreateAction("ActionCreateAction", otherName);

			actionDestroyAction->Update(worldState);

			Assert::IsTrue(worldState.mAction == actionDestroyAction);
			Assert::AreEqual(2U, entity.Actions().Size());

			actionDestroyAction->SetInstanceName(otherName);

			actionDestroyAction->Update(worldState);

			Assert::IsTrue(worldState.mAction == actionDestroyAction);
			Assert::AreEqual(1U, entity.Actions().Size());

			actionDestroyAction->SetInstanceName(name);

			actionDestroyAction->Update(worldState);

			Assert::IsTrue(worldState.mAction == actionDestroyAction);
			Assert::AreEqual(0U, entity.Actions().Size());
		}

		TEST_METHOD(ActionDestroyActionCopy)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello", unknownStringValue = "Unknown";
			ActionDestroyAction actionDestroyAction, childOne, childTwo, childThree;

			actionDestroyAction.Adopt(childOne, stringValue);
			actionDestroyAction.Adopt(childTwo, nextStringValue);
			actionDestroyAction.Adopt(childThree, thirdStringValue);

			Scope* copy = actionDestroyAction.Copy();

			Assert::IsTrue(*copy == actionDestroyAction);
			Assert::IsTrue(copy->Find(stringValue)->Get<Scope&>().Is(ActionDestroyAction::TypeIdClass()));
			Assert::IsTrue(copy->Find(nextStringValue)->Get<Scope&>().Is(ActionDestroyAction::TypeIdClass()));
			Assert::IsTrue(copy->Find(thirdStringValue)->Get<Scope&>().Is(ActionDestroyAction::TypeIdClass()));

			delete(copy);
		}

		TEST_METHOD(ActionDestroyActionRTTIMethods)
		{
			ActionDestroyAction actionDestroyAction;

			Assert::IsTrue(actionDestroyAction.Is(ActionDestroyAction::TypeIdClass()));
			Assert::IsTrue(actionDestroyAction.Is(ActionDestroyAction::TypeName()));
			Assert::AreEqual(ActionDestroyAction::TypeIdClass(), actionDestroyAction.TypeIdInstance());
			Assert::AreEqual(ActionDestroyAction::TypeName(), actionDestroyAction.TypeName());

			RTTI* asRTTI = &actionDestroyAction;

			Assert::IsFalse(nullptr == asRTTI->QueryInterface(ActionDestroyAction::TypeIdClass()));
			Assert::IsTrue(actionDestroyAction == *(asRTTI->As<ActionDestroyAction>()));
			Assert::AreEqual(asRTTI->As<ActionDestroyAction>()->ToString(), actionDestroyAction.ToString());
		}

	private:

		static _CrtMemState sStartMemState;

		ActionListFactory mActionListFactory;

		ActionDestroyActionFactory mActionDestroyActionFactory;

		ActionCreateActionFactory mActionCreateActionFactory;
	};

	_CrtMemState ActionDestroyActionTest::sStartMemState;
}
