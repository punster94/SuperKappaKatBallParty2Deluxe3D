#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(ActionListIfTest)
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

		TEST_METHOD(ActionListIfConstructor)
		{
			std::string name = "hello";
			ActionListIf actionListIf(name);

			Assert::IsTrue(actionListIf.Name() == name);
			Assert::IsTrue(actionListIf.IsPrescribedAttribute(Attributed::sAttributedThisKey));
			Assert::IsTrue(actionListIf.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(actionListIf.IsPrescribedAttribute(Action::sActionsKey));

			ActionListIf otherActionListIf(name);

			Assert::IsTrue(otherActionListIf.Name() == name);
			Assert::IsTrue(otherActionListIf.IsPrescribedAttribute(Attributed::sAttributedThisKey));
			Assert::IsTrue(otherActionListIf.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(otherActionListIf.IsPrescribedAttribute(Action::sActionsKey));
		}

		TEST_METHOD(ActionListIfDestructor)
		{
			std::string name = "hello";
			ActionListIf* actionList = new ActionListIf();

			delete(actionList);

			actionList = new ActionListIf(name);
			actionList->AppendAuxiliaryAttribute(name) = name;

			Assert::IsTrue(actionList->IsAuxiliaryAttribute(name));

			delete(actionList);
		}

		TEST_METHOD(ActionListIfCopyConstructor)
		{
			std::string name = "hello";
			Entity entity;
			ActionListIf actionListIf(name);

			entity.Adopt(actionListIf, Action::sActionsKey);

			ActionListIf otherActionListIf = actionListIf;

			Assert::IsTrue(otherActionListIf == actionListIf);
			Assert::IsTrue(otherActionListIf.Name() == actionListIf.Name());
			Assert::IsNull(otherActionListIf.GetParent());
			Assert::IsTrue(actionListIf.GetParent() == &entity);
			Assert::AreEqual(1U, entity.Actions().Size());
		}

		TEST_METHOD(ActionListIfAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			Entity entity;
			ActionListIf actionListIf(name);

			entity.Adopt(actionListIf, Action::sActionsKey);

			ActionListIf otherActionListIf(otherName);
			otherActionListIf = actionListIf;

			Assert::IsTrue(otherActionListIf == actionListIf);
			Assert::IsTrue(otherActionListIf.Name() == actionListIf.Name());
			Assert::IsNull(otherActionListIf.GetParent());
			Assert::IsTrue(actionListIf.GetParent() == &entity);
			Assert::AreEqual(1U, entity.Actions().Size());

			otherActionListIf = otherActionListIf;

			Assert::IsTrue(otherActionListIf == actionListIf);
			Assert::IsTrue(otherActionListIf.Name() == actionListIf.Name());
			Assert::IsNull(otherActionListIf.GetParent());
			Assert::IsTrue(actionListIf.GetParent() == &entity);
			Assert::AreEqual(1U, entity.Actions().Size());
		}

		TEST_METHOD(ActionListIfMoveConstructor)
		{
			std::string name = "hello";
			Entity entity;
			ActionListIf actionListIf(name);

			entity.Adopt(actionListIf, Action::sActionsKey);

			ActionListIf otherActionListIf = std::move(actionListIf);

			Assert::IsTrue(otherActionListIf.Name() == name);
			Assert::IsTrue(otherActionListIf.GetParent() == &entity);
			Assert::IsTrue(otherActionListIf.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsTrue(otherActionListIf.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(otherActionListIf.IsPrescribedAttribute(Attributed::sAttributedThisKey));
			Assert::IsNull(actionListIf.GetParent());
			Assert::IsTrue(actionListIf.Name() == std::string());
			Assert::AreEqual(1U, entity.Actions().Size());
		}

		TEST_METHOD(ActionListIfMoveAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			Entity entity;
			ActionListIf actionListIf(name);

			entity.Adopt(actionListIf, Action::sActionsKey);

			ActionListIf otherActionListIf(otherName);

			otherActionListIf = std::move(actionListIf);

			Assert::IsTrue(otherActionListIf.Name() == name);
			Assert::IsTrue(otherActionListIf.GetParent() == &entity);
			Assert::IsTrue(otherActionListIf.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(otherActionListIf.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsNull(actionListIf.GetParent());
			Assert::IsTrue(actionListIf.Name() == std::string());
			Assert::AreEqual(1U, entity.Actions().Size());

			otherActionListIf = std::move(otherActionListIf);

			Assert::IsTrue(otherActionListIf.Name() == name);
			Assert::IsTrue(otherActionListIf.GetParent() == &entity);
			Assert::IsTrue(otherActionListIf.IsPrescribedAttribute(Action::sActionNameKey));
			Assert::IsTrue(otherActionListIf.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsNull(actionListIf.GetParent());
			Assert::IsTrue(actionListIf.Name() == std::string());
			Assert::AreEqual(1U, entity.Actions().Size());
		}


		TEST_METHOD(ActionListIfThenAction)
		{
			std::string name = "hello", otherName = "howdy";
			ActionListIf actionListIf;

			Assert::IsNull(actionListIf.ThenAction());

			actionListIf.CreateAction("ActionList", name);
			
			Assert::IsNull(actionListIf.ThenAction());

			Action* thenAction = actionListIf.CreateAction("ActionList", ActionListIf::sActionListIfThenKey);

			Assert::IsTrue(actionListIf.ThenAction() == thenAction);
		}

		TEST_METHOD(ActionListIfElseAction)
		{
			std::string name = "hello", otherName = "howdy";
			ActionListIf actionListIf;

			Assert::IsNull(actionListIf.ElseAction());

			actionListIf.CreateAction("ActionList", name);

			Assert::IsNull(actionListIf.ElseAction());

			Action* elseAction = actionListIf.CreateAction("ActionList", ActionListIf::sActionListIfElseKey);

			Assert::IsTrue(actionListIf.ElseAction() == elseAction);
		}

		TEST_METHOD(ActionListIfUpdate)
		{
			std::string name = "hello";
			WorldState worldState;
			ActionListIf actionListIf;

			Action* action = actionListIf.CreateAction("ActionExpression", ActionExpression::sExpressionKey);
			ActionExpression* expression = action->As<ActionExpression>();

			expression->SetExpression("0");

			actionListIf.Update(worldState);

			Assert::IsTrue(worldState.mAction == &actionListIf);

			Action* thenAction = actionListIf.CreateAction("ActionList", ActionListIf::sActionListIfThenKey);

			actionListIf.Update(worldState);

			Assert::IsTrue(worldState.mAction == &actionListIf);

			Action* elseAction = actionListIf.CreateAction("ActionList", ActionListIf::sActionListIfElseKey);

			actionListIf.Update(worldState);

			Assert::IsTrue(worldState.mAction == elseAction);
			
			expression->SetExpression("1");
			actionListIf.Update(worldState);

			Assert::IsTrue(worldState.mAction == thenAction);
		}

		TEST_METHOD(ActionListIfCopy)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello", unknownStringValue = "Unknown";
			ActionListIf actionListIf, childOne, childTwo, childThree;

			actionListIf.Adopt(childOne, stringValue);
			actionListIf.Adopt(childTwo, nextStringValue);
			actionListIf.Adopt(childThree, thirdStringValue);

			Scope* copy = actionListIf.Copy();

			Assert::IsTrue(*copy == actionListIf);
			Assert::IsTrue(copy->Find(stringValue)->Get<Scope&>().Is(ActionListIf::TypeIdClass()));
			Assert::IsTrue(copy->Find(nextStringValue)->Get<Scope&>().Is(ActionListIf::TypeIdClass()));
			Assert::IsTrue(copy->Find(thirdStringValue)->Get<Scope&>().Is(ActionListIf::TypeIdClass()));

			delete(copy);
		}

		TEST_METHOD(ActionListIfRTTIMethods)
		{
			ActionListIf actionListIf;

			Assert::IsTrue(actionListIf.Is(ActionListIf::TypeIdClass()));
			Assert::IsTrue(actionListIf.Is(ActionListIf::TypeName()));
			Assert::AreEqual(ActionListIf::TypeIdClass(), actionListIf.TypeIdInstance());
			Assert::AreEqual(ActionListIf::TypeName(), actionListIf.TypeName());

			RTTI* asRTTI = &actionListIf;

			Assert::IsFalse(nullptr == asRTTI->QueryInterface(ActionListIf::TypeIdClass()));
			Assert::IsTrue(actionListIf == *(asRTTI->As<ActionListIf>()));
			Assert::AreEqual(asRTTI->As<ActionListIf>()->ToString(), actionListIf.ToString());
		}

	private:

		static _CrtMemState sStartMemState;

		ActionListFactory mActionListFactory;

		ActionExpressionFactory mActionExpressionFactory;
	};

	_CrtMemState ActionListIfTest::sStartMemState;
}
