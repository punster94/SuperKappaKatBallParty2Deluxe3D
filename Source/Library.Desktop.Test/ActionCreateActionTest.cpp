#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(ActionCreateActionTest)
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

		TEST_METHOD(ActionCreateActionConstructor)
		{
			std::string name = "hello";
			ActionCreateAction actionCreateAction(name);

			Assert::IsTrue(actionCreateAction.Name() == name);
			Assert::IsTrue(actionCreateAction[ActionCreateAction::sActionCreateActionInstanceNameKey] == "");
			Assert::IsTrue(actionCreateAction[ActionCreateAction::sActionCreateActionClassNameKey] == "");
			Assert::IsTrue(actionCreateAction.IsPrescribedAttribute(Attributed::sAttributedThisKey));
			Assert::IsTrue(actionCreateAction.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(actionCreateAction.IsPrescribedAttribute(ActionCreateAction::sActionCreateActionInstanceNameKey));
			Assert::IsTrue(actionCreateAction.IsPrescribedAttribute(ActionCreateAction::sActionCreateActionClassNameKey));

			ActionCreateAction otherActionCreateAction(name);

			Assert::IsTrue(otherActionCreateAction.Name() == name);
			Assert::IsTrue(otherActionCreateAction[ActionCreateAction::sActionCreateActionInstanceNameKey] == "");
			Assert::IsTrue(otherActionCreateAction[ActionCreateAction::sActionCreateActionClassNameKey] == "");
			Assert::IsTrue(otherActionCreateAction.IsPrescribedAttribute(Attributed::sAttributedThisKey));
			Assert::IsTrue(otherActionCreateAction.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(otherActionCreateAction.IsPrescribedAttribute(ActionCreateAction::sActionCreateActionInstanceNameKey));
			Assert::IsTrue(otherActionCreateAction.IsPrescribedAttribute(ActionCreateAction::sActionCreateActionClassNameKey));
		}

		TEST_METHOD(ActionCreateActionDestructor)
		{
			std::string name = "hello";
			ActionCreateAction* actionCreateAction = new ActionCreateAction();

			delete(actionCreateAction);

			actionCreateAction = new ActionCreateAction(name);
			actionCreateAction->AppendAuxiliaryAttribute(name) = name;

			Assert::IsTrue(actionCreateAction->IsAuxiliaryAttribute(name));

			delete(actionCreateAction);
		}

		TEST_METHOD(ActionCreateActionCopyConstructor)
		{
			std::string name = "hello";
			Entity entity;
			ActionCreateAction actionCreateAction(name);

			actionCreateAction.SetInstanceName(name);
			actionCreateAction.SetClassName(name);
			entity.Adopt(actionCreateAction, Action::sActionsKey);

			ActionCreateAction otherActionCreateAction = actionCreateAction;

			Assert::IsTrue(otherActionCreateAction == actionCreateAction);
			Assert::IsTrue(otherActionCreateAction.Name() == actionCreateAction.Name());
			Assert::IsTrue(otherActionCreateAction[ActionCreateAction::sActionCreateActionInstanceNameKey] == actionCreateAction[ActionCreateAction::sActionCreateActionInstanceNameKey]);
			Assert::IsTrue(otherActionCreateAction[ActionCreateAction::sActionCreateActionClassNameKey] == actionCreateAction[ActionCreateAction::sActionCreateActionClassNameKey]);
			Assert::IsNull(otherActionCreateAction.GetParent());
			Assert::IsTrue(actionCreateAction.GetParent() == &entity);
			Assert::AreEqual(1U, entity.Actions().Size());
		}

		TEST_METHOD(ActionCreateActionAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			Entity entity;
			ActionCreateAction actionCreateAction(name);

			actionCreateAction.SetInstanceName(name);
			actionCreateAction.SetClassName(name);
			entity.Adopt(actionCreateAction, Action::sActionsKey);

			ActionCreateAction otherActionCreateAction(otherName);
			otherActionCreateAction = actionCreateAction;

			Assert::IsTrue(otherActionCreateAction == actionCreateAction);
			Assert::IsTrue(otherActionCreateAction.Name() == actionCreateAction.Name());
			Assert::IsTrue(otherActionCreateAction[ActionCreateAction::sActionCreateActionInstanceNameKey] == actionCreateAction[ActionCreateAction::sActionCreateActionInstanceNameKey]);
			Assert::IsTrue(otherActionCreateAction[ActionCreateAction::sActionCreateActionClassNameKey] == actionCreateAction[ActionCreateAction::sActionCreateActionClassNameKey]);
			Assert::IsNull(otherActionCreateAction.GetParent());
			Assert::IsTrue(actionCreateAction.GetParent() == &entity);
			Assert::AreEqual(1U, entity.Actions().Size());

			otherActionCreateAction = otherActionCreateAction;

			Assert::IsTrue(otherActionCreateAction == actionCreateAction);
			Assert::IsTrue(otherActionCreateAction.Name() == actionCreateAction.Name());
			Assert::IsTrue(otherActionCreateAction[ActionCreateAction::sActionCreateActionInstanceNameKey] == actionCreateAction[ActionCreateAction::sActionCreateActionInstanceNameKey]);
			Assert::IsTrue(otherActionCreateAction[ActionCreateAction::sActionCreateActionClassNameKey] == actionCreateAction[ActionCreateAction::sActionCreateActionClassNameKey]);
			Assert::IsNull(otherActionCreateAction.GetParent());
			Assert::IsTrue(actionCreateAction.GetParent() == &entity);
			Assert::AreEqual(1U, entity.Actions().Size());
		}

		TEST_METHOD(ActionCreateActionMoveConstructor)
		{
			std::string name = "hello";
			Entity entity;
			ActionCreateAction actionCreateAction(name);

			actionCreateAction.SetInstanceName(name);
			actionCreateAction.SetClassName(name);
			entity.Adopt(actionCreateAction, Action::sActionsKey);

			ActionCreateAction otherActionCreateAction = std::move(actionCreateAction);

			Assert::IsTrue(otherActionCreateAction.Name() == name);
			Assert::IsTrue(otherActionCreateAction.GetParent() == &entity);
			Assert::IsTrue(otherActionCreateAction.IsPrescribedAttribute(Attributed::sAttributedThisKey));
			Assert::IsTrue(otherActionCreateAction.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(otherActionCreateAction.IsPrescribedAttribute(ActionCreateAction::sActionCreateActionInstanceNameKey));
			Assert::IsTrue(otherActionCreateAction.IsPrescribedAttribute(ActionCreateAction::sActionCreateActionClassNameKey));
			Assert::IsTrue(otherActionCreateAction[ActionCreateAction::sActionCreateActionInstanceNameKey] == name);
			Assert::IsTrue(otherActionCreateAction[ActionCreateAction::sActionCreateActionClassNameKey] == name);
			Assert::IsNull(actionCreateAction.GetParent());
			Assert::IsTrue(actionCreateAction.Name() == std::string());
			Assert::AreEqual(1U, entity.Actions().Size());
		}

		TEST_METHOD(ActionCreateActionMoveAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			Entity entity;
			ActionCreateAction actionCreateAction(name);

			actionCreateAction.SetInstanceName(name);
			actionCreateAction.SetClassName(name);
			entity.Adopt(actionCreateAction, Action::sActionsKey);

			ActionCreateAction otherActionCreateAction(otherName);

			otherActionCreateAction = std::move(actionCreateAction);

			Assert::IsTrue(otherActionCreateAction.Name() == name);
			Assert::IsTrue(otherActionCreateAction.GetParent() == &entity);
			Assert::IsTrue(otherActionCreateAction.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(otherActionCreateAction.IsPrescribedAttribute(ActionCreateAction::sActionCreateActionInstanceNameKey));
			Assert::IsTrue(otherActionCreateAction.IsPrescribedAttribute(ActionCreateAction::sActionCreateActionClassNameKey));
			Assert::IsTrue(otherActionCreateAction[ActionCreateAction::sActionCreateActionInstanceNameKey] == name);
			Assert::IsTrue(otherActionCreateAction[ActionCreateAction::sActionCreateActionClassNameKey] == name);
			Assert::IsNull(actionCreateAction.GetParent());
			Assert::IsTrue(actionCreateAction.Name() == std::string());
			Assert::AreEqual(1U, entity.Actions().Size());

			otherActionCreateAction = std::move(otherActionCreateAction);

			Assert::IsTrue(otherActionCreateAction.Name() == name);
			Assert::IsTrue(otherActionCreateAction.GetParent() == &entity);
			Assert::IsTrue(otherActionCreateAction.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(otherActionCreateAction.IsPrescribedAttribute(ActionCreateAction::sActionCreateActionInstanceNameKey));
			Assert::IsTrue(otherActionCreateAction.IsPrescribedAttribute(ActionCreateAction::sActionCreateActionClassNameKey));
			Assert::IsTrue(otherActionCreateAction[ActionCreateAction::sActionCreateActionInstanceNameKey] == name);
			Assert::IsTrue(otherActionCreateAction[ActionCreateAction::sActionCreateActionClassNameKey] == name);
			Assert::IsNull(actionCreateAction.GetParent());
			Assert::IsTrue(actionCreateAction.Name() == std::string());
			Assert::AreEqual(1U, entity.Actions().Size());
		}

		TEST_METHOD(ActionCreateActionSetInstanceName)
		{
			std::string name = "hello";
			ActionCreateAction actionCreateAction;

			actionCreateAction.SetInstanceName(name);

			Assert::IsTrue(actionCreateAction[ActionCreateAction::sActionCreateActionInstanceNameKey] == name);
		}

		TEST_METHOD(ActionCreateActionSetClassName)
		{
			std::string name = "hello";
			ActionCreateAction actionCreateAction;

			actionCreateAction.SetClassName(name);

			Assert::IsTrue(actionCreateAction[ActionCreateAction::sActionCreateActionClassNameKey] == name);
		}

		TEST_METHOD(ActionCreateActionUpdate)
		{
			std::string name = "hello", otherName = "howdy";
			WorldState worldState;
			Entity entity;

			Action* action = entity.CreateAction("ActionCreateAction", name);
			ActionCreateAction* actionCreateAction = action->As<ActionCreateAction>();

			actionCreateAction->Update(worldState);

			Assert::IsTrue(worldState.mAction == actionCreateAction);
			Assert::AreEqual(1U, entity.Actions().Size());

			actionCreateAction->SetClassName("ActionList");
			actionCreateAction->SetInstanceName(otherName);

			actionCreateAction->Update(worldState);

			Assert::IsTrue(worldState.mAction == actionCreateAction);
			Assert::AreEqual(2U, entity.Actions().Size());

			ActionList* createdAction = entity.Actions().Get<Scope&>(1).As<ActionList>();

			Assert::IsNotNull(createdAction);
			Assert::IsTrue(createdAction->Name() == otherName);

			action = createdAction->CreateAction("ActionCreateAction", name);
			actionCreateAction = action->As<ActionCreateAction>();

			actionCreateAction->Update(worldState);

			Assert::IsTrue(worldState.mAction == actionCreateAction);
			Assert::AreEqual(1U, createdAction->Actions().Size());

			actionCreateAction->SetClassName("ActionList");
			actionCreateAction->SetInstanceName(otherName);

			actionCreateAction->Update(worldState);

			Assert::IsTrue(worldState.mAction == actionCreateAction);
			Assert::AreEqual(2U, createdAction->Actions().Size());

			ActionList* nextCreatedAction = entity.Actions().Get<Scope&>(1).As<ActionList>();

			Assert::IsNotNull(nextCreatedAction);
			Assert::IsTrue(nextCreatedAction->Name() == otherName);
		}

		TEST_METHOD(ActionCreateActionCopy)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello", unknownStringValue = "Unknown";
			ActionCreateAction actionCreateAction, childOne, childTwo, childThree;

			actionCreateAction.Adopt(childOne, stringValue);
			actionCreateAction.Adopt(childTwo, nextStringValue);
			actionCreateAction.Adopt(childThree, thirdStringValue);

			Scope* copy = actionCreateAction.Copy();

			Assert::IsTrue(*copy == actionCreateAction);
			Assert::IsTrue(copy->Find(stringValue)->Get<Scope&>().Is(ActionCreateAction::TypeIdClass()));
			Assert::IsTrue(copy->Find(nextStringValue)->Get<Scope&>().Is(ActionCreateAction::TypeIdClass()));
			Assert::IsTrue(copy->Find(thirdStringValue)->Get<Scope&>().Is(ActionCreateAction::TypeIdClass()));

			delete(copy);
		}

		TEST_METHOD(ActionCreateActionRTTIMethods)
		{
			ActionCreateAction actionCreateAction;

			Assert::IsTrue(actionCreateAction.Is(ActionCreateAction::TypeIdClass()));
			Assert::IsTrue(actionCreateAction.Is(ActionCreateAction::TypeName()));
			Assert::AreEqual(ActionCreateAction::TypeIdClass(), actionCreateAction.TypeIdInstance());
			Assert::AreEqual(ActionCreateAction::TypeName(), actionCreateAction.TypeName());

			RTTI* asRTTI = &actionCreateAction;

			Assert::IsFalse(nullptr == asRTTI->QueryInterface(ActionCreateAction::TypeIdClass()));
			Assert::IsTrue(actionCreateAction == *(asRTTI->As<ActionCreateAction>()));
			Assert::AreEqual(asRTTI->As<ActionCreateAction>()->ToString(), actionCreateAction.ToString());
		}

	private:

		static _CrtMemState sStartMemState;

		ActionListFactory mActionListFactory;

		ActionCreateActionFactory mActionCreateActionFactory;
	};

	_CrtMemState ActionCreateActionTest::sStartMemState;
}
