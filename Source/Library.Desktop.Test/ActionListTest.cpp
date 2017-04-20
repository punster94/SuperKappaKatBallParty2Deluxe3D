#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(ActionListTest)
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

		TEST_METHOD(ActionListConstructor)
		{
			std::string name = "hello";
			ActionList actionList;

			Assert::IsTrue(actionList.Name() == "");
			Assert::IsTrue(actionList.IsPrescribedAttribute(Attributed::sAttributedThisKey));
			Assert::IsTrue(actionList.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(actionList.IsPrescribedAttribute(Action::sActionsKey));

			ActionList otherActionList(name);

			Assert::IsTrue(otherActionList.Name() == name);
			Assert::IsTrue(otherActionList.IsPrescribedAttribute(Attributed::sAttributedThisKey));
			Assert::IsTrue(otherActionList.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(otherActionList.IsPrescribedAttribute(Action::sActionsKey));
		}

		TEST_METHOD(ActionListDestructor)
		{
			std::string name = "hello";
			ActionList* actionList = new ActionList();

			delete(actionList);

			actionList = new ActionList(name);
			actionList->AppendAuxiliaryAttribute(name) = name;

			Assert::IsTrue(actionList->IsAuxiliaryAttribute(name));

			delete(actionList);
		}

		TEST_METHOD(ActionListCopyConstructor)
		{
			std::string name = "hello";
			Entity entity;
			ActionList actionList(name);

			entity.Adopt(actionList, Action::sActionsKey);

			ActionList otherActionList = actionList;

			Assert::IsTrue(otherActionList == actionList);
			Assert::IsTrue(otherActionList.Name() == actionList.Name());
			Assert::IsNull(otherActionList.GetParent());
			Assert::IsTrue(actionList.GetParent() == &entity);
			Assert::AreEqual(1U, entity.Actions().Size());
		}

		TEST_METHOD(ActionListAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			Entity entity;
			ActionList actionList(name);

			entity.Adopt(actionList, Action::sActionsKey);

			ActionList otherActionList(otherName);
			otherActionList = actionList;

			Assert::IsTrue(otherActionList == actionList);
			Assert::IsTrue(otherActionList.Name() == actionList.Name());
			Assert::IsNull(otherActionList.GetParent());
			Assert::IsTrue(actionList.GetParent() == &entity);
			Assert::AreEqual(1U, entity.Actions().Size());

			otherActionList = otherActionList;

			Assert::IsTrue(otherActionList == actionList);
			Assert::IsTrue(otherActionList.Name() == actionList.Name());
			Assert::IsNull(otherActionList.GetParent());
			Assert::IsTrue(actionList.GetParent() == &entity);
			Assert::AreEqual(1U, entity.Actions().Size());
		}

		TEST_METHOD(ActionListMoveConstructor)
		{
			std::string name = "hello";
			Entity entity;
			ActionList actionList(name);

			entity.Adopt(actionList, Action::sActionsKey);

			ActionList otherActionList = std::move(actionList);

			Assert::IsTrue(otherActionList.Name() == name);
			Assert::IsTrue(otherActionList.GetParent() == &entity);
			Assert::IsTrue(otherActionList.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsTrue(otherActionList.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(otherActionList.IsPrescribedAttribute(Attributed::sAttributedThisKey));
			Assert::IsNull(actionList.GetParent());
			Assert::IsTrue(actionList.Name() == std::string());
			Assert::AreEqual(1U, entity.Actions().Size());
		}

		TEST_METHOD(ActionListMoveAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			Entity entity;
			ActionList actionList(name);

			entity.Adopt(actionList, Action::sActionsKey);

			ActionList otherActionList(otherName);

			otherActionList = std::move(actionList);

			Assert::IsTrue(otherActionList.Name() == name);
			Assert::IsTrue(otherActionList.GetParent() == &entity);
			Assert::IsTrue(otherActionList.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(otherActionList.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsNull(actionList.GetParent());
			Assert::IsTrue(actionList.Name() == std::string());
			Assert::AreEqual(1U, entity.Actions().Size());

			otherActionList = std::move(otherActionList);

			Assert::IsTrue(otherActionList.Name() == name);
			Assert::IsTrue(otherActionList.GetParent() == &entity);
			Assert::IsTrue(otherActionList.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(otherActionList.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsNull(actionList.GetParent());
			Assert::IsTrue(actionList.Name() == std::string());
			Assert::AreEqual(1U, entity.Actions().Size());
		}

		TEST_METHOD(ActionName)
		{
			std::string name = "hello", otherName = "howdy";
			ActionList actionList(name);

			Assert::IsTrue(actionList.Name() == name);

			actionList.SetName(otherName);

			Assert::IsTrue(actionList.Name() == otherName);
		}

		TEST_METHOD(ActionSetName)
		{
			std::string name = "hello", otherName = "howdy";
			ActionList actionList;

			actionList.SetName(name);

			Assert::IsTrue(actionList.Name() == name);

			actionList.SetName(otherName);

			Assert::IsTrue(actionList.Name() == otherName);
		}

		TEST_METHOD(ActionListActions)
		{
			std::string name = "hello", otherName = "howdy";
			ActionList actionList;

			Assert::AreEqual(0U, actionList.Actions().Size());

			actionList.CreateAction("ActionList", name);
			actionList.CreateAction("ActionList", otherName);

			Assert::AreEqual(2U, actionList.Actions().Size());
			Assert::IsTrue(actionList.Actions().Get<Scope&>()[Action::sActionNameKey] == name);
			Assert::IsTrue(actionList.Actions().Get<Scope&>(1)[Action::sActionNameKey] == otherName);

			const ActionList otherActionList = actionList;

			Assert::AreEqual(2U, otherActionList.Actions().Size());
			Assert::IsTrue(otherActionList.Actions().Get<const Scope&>()[Action::sActionNameKey] == name);
			Assert::IsTrue(otherActionList.Actions().Get<const Scope&>(1)[Action::sActionNameKey] == otherName);
		}

		TEST_METHOD(ActionListCreateAction)
		{
			std::string name = "hello", otherName = "howdy";
			ActionList actionList;

			Action* action = actionList.CreateAction("ActionList", name);
			Action* otherAction = actionList.CreateAction("ActionList", otherName);

			Assert::IsTrue(action->Name() == name);
			Assert::IsTrue(action->GetParent() == &actionList);
			Assert::IsTrue(otherAction->Name() == otherName);
			Assert::IsTrue(otherAction->GetParent() == &actionList);
			Assert::IsTrue(actionList.Actions().Get<Scope&>()[Action::sActionNameKey] == name);
			Assert::IsTrue(actionList.Actions().Get<Scope&>(1)[Action::sActionNameKey] == otherName);
		}

		TEST_METHOD(ActionReactions)
		{
			std::string name = "hello";
			ActionList action;

			Assert::AreEqual(0U, action.Reactions().Size());

			ReactionAttributed reaction(name);

			action.Adopt(reaction, Reaction::sReactionsKey);

			Assert::AreEqual(1U, action.Reactions().Size());
			Assert::IsTrue(action.Reactions().Get<Scope&>()[Action::sActionNameKey] == name);

			const ActionList otherAction = action;

			Assert::AreEqual(1U, otherAction.Reactions().Size());
			Assert::IsTrue(otherAction.Reactions().Get<const Scope&>()[Action::sActionNameKey] == name);
		}

		TEST_METHOD(ActionListFindAction)
		{
			std::string name = "hello", otherName = "howdy", thirdName = "Hi";
			ActionList actionList;

			Action* action = actionList.CreateAction("ActionList", name);
			Action* otherAction = actionList.CreateAction("ActionList", otherName);

			Assert::IsNull(actionList.FindAction(thirdName));
			Assert::IsTrue(actionList.FindAction(name) == action);
			Assert::IsTrue(actionList.FindAction(otherName) == otherAction);
		}

		TEST_METHOD(ActionListUpdate)
		{
			std::string name = "hello";
			WorldState worldState;
			ActionList actionList;

			actionList.Update(worldState);

			Assert::IsTrue(worldState.mAction == &actionList);

			Action* action = actionList.CreateAction("ActionList", name);

			actionList.Update(worldState);

			Assert::IsTrue(worldState.mAction == action);
		}

		TEST_METHOD(ActionListCopy)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello", unknownStringValue = "Unknown";
			ActionList actionList, childOne, childTwo, childThree;

			actionList.Adopt(childOne, stringValue);
			actionList.Adopt(childTwo, nextStringValue);
			actionList.Adopt(childThree, thirdStringValue);

			Scope* copy = actionList.Copy();

			Assert::IsTrue(*copy == actionList);
			Assert::IsTrue(copy->Find(stringValue)->Get<Scope&>().Is(ActionList::TypeIdClass()));
			Assert::IsTrue(copy->Find(nextStringValue)->Get<Scope&>().Is(ActionList::TypeIdClass()));
			Assert::IsTrue(copy->Find(thirdStringValue)->Get<Scope&>().Is(ActionList::TypeIdClass()));

			delete(copy);
		}

		TEST_METHOD(ActionListRTTIMethods)
		{
			ActionList actionList;

			Assert::IsTrue(actionList.Is(ActionList::TypeIdClass()));
			Assert::IsTrue(actionList.Is(Action::TypeIdClass()));
			Assert::IsTrue(actionList.Is(ActionList::TypeName()));
			Assert::IsTrue(actionList.Is(Action::TypeName()));
			Assert::AreEqual(ActionList::TypeIdClass(), actionList.TypeIdInstance());
			Assert::AreNotEqual(Action::TypeIdClass(), actionList.TypeIdInstance());
			Assert::AreEqual(ActionList::TypeName(), actionList.TypeName());
			Assert::AreNotEqual(Action::TypeName(), actionList.TypeName());

			RTTI* asRTTI = &actionList;

			Assert::IsFalse(nullptr == asRTTI->QueryInterface(ActionList::TypeIdClass()));
			Assert::IsTrue(actionList == *(asRTTI->As<ActionList>()));
			Assert::IsTrue(&actionList == asRTTI->As<Action>());
			Assert::AreEqual(asRTTI->As<ActionList>()->ToString(), actionList.ToString());
		}

	private:

		static _CrtMemState sStartMemState;

		ActionListFactory mActionListFactory;
	};

	_CrtMemState ActionListTest::sStartMemState;
}
