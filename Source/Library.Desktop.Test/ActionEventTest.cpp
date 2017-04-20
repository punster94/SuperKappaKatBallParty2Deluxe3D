#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(ActionEventTest)
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

		TEST_METHOD(ActionEventConstructor)
		{
			std::string name = "hello";
			ActionEvent action;

			Assert::IsTrue(action.IsPrescribedAttribute(ActionEvent::sActionEventSubtypeKey));
			Assert::IsTrue(action.IsPrescribedAttribute(ActionEvent::sActionEventDelayKey));
			Assert::AreEqual(std::string(), action.GetSubtype());
			Assert::AreEqual(std::int32_t(), action.GetDelay());
			Assert::AreEqual(std::string(), action.Name());

			ActionEvent otherAction(name);

			Assert::IsTrue(otherAction.IsPrescribedAttribute(ActionEvent::sActionEventSubtypeKey));
			Assert::IsTrue(otherAction.IsPrescribedAttribute(ActionEvent::sActionEventDelayKey));
			Assert::AreEqual(std::string(), otherAction.GetSubtype());
			Assert::AreEqual(std::int32_t(), otherAction.GetDelay());
			Assert::AreEqual(name, otherAction.Name());
		}

		TEST_METHOD(ActionEventDestructor)
		{
			std::string name = "hello";
			ActionEvent* action = new ActionEvent(name);

			delete(action);

			action = new ActionEvent(name);
			action->AppendAuxiliaryAttribute(name) = name;

			Assert::IsTrue(action->IsAuxiliaryAttribute(name));

			delete(action);
		}

		TEST_METHOD(ActionEventCopyConstructor)
		{
			std::string name = "hello";
			ActionEvent action(name);
			action.SetDelay(rand());

			ActionEvent otherAction = action;

			Assert::IsTrue(otherAction == action);
			Assert::IsTrue(otherAction.GetSubtype() == action.GetSubtype());
			Assert::IsTrue(otherAction.GetDelay() == action.GetDelay());
		}

		TEST_METHOD(ActionEventAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			ActionEvent action(name);
			action.SetDelay(rand());

			ActionEvent otherAction(otherName);
			otherAction = action;

			Assert::IsTrue(otherAction == action);
			Assert::IsTrue(otherAction.GetSubtype() == action.GetSubtype());
			Assert::IsTrue(otherAction.GetDelay() == action.GetDelay());

			otherAction = otherAction;

			Assert::IsTrue(otherAction == action);
			Assert::IsTrue(otherAction.GetSubtype() == action.GetSubtype());
			Assert::IsTrue(otherAction.GetDelay() == action.GetDelay());
		}

		TEST_METHOD(ActionEventMoveConstructor)
		{
			std::int32_t delay = rand();
			std::string name = "hello";
			ActionEvent action(name);
			action.SetDelay(delay);
			action.SetSubtype(name);

			ActionEvent otherAction = std::move(action);

			Assert::AreEqual(name, otherAction.GetSubtype());
			Assert::AreEqual(delay, otherAction.GetDelay());
			Assert::IsTrue(otherAction.IsPrescribedAttribute(ActionEvent::sActionEventSubtypeKey));
			Assert::IsTrue(otherAction.IsPrescribedAttribute(ActionEvent::sActionEventDelayKey));
			Assert::AreEqual(std::string(), action.GetSubtype());
			Assert::AreEqual(std::int32_t(), action.GetDelay());
		}

		TEST_METHOD(ActionEventMoveAssignmentOperator)
		{
			std::int32_t delay = rand();
			std::string name = "hello", otherName = "howdy";
			ActionEvent action(name);
			action.SetDelay(delay);
			action.SetSubtype(name);

			ActionEvent otherAction(otherName);

			otherAction = std::move(action);

			Assert::AreEqual(name, otherAction.GetSubtype());
			Assert::AreEqual(delay, otherAction.GetDelay());
			Assert::IsTrue(otherAction.IsPrescribedAttribute(ActionEvent::sActionEventSubtypeKey));
			Assert::IsTrue(otherAction.IsPrescribedAttribute(ActionEvent::sActionEventDelayKey));
			Assert::AreEqual(std::string(), action.GetSubtype());
			Assert::AreEqual(std::int32_t(), action.GetDelay());

			otherAction = std::move(otherAction);

			Assert::AreEqual(name, otherAction.GetSubtype());
			Assert::AreEqual(delay, otherAction.GetDelay());
			Assert::IsTrue(otherAction.IsPrescribedAttribute(ActionEvent::sActionEventSubtypeKey));
			Assert::IsTrue(otherAction.IsPrescribedAttribute(ActionEvent::sActionEventDelayKey));
			Assert::AreEqual(std::string(), action.GetSubtype());
			Assert::AreEqual(std::int32_t(), action.GetDelay());
		}

		TEST_METHOD(ActionEventSubtype)
		{
			std::string name = "hello", otherName = "howdy";
			ActionEvent action(name);

			Assert::AreEqual(std::string(), action.GetSubtype());

			action.SetSubtype(name);

			Assert::AreEqual(name, action.GetSubtype());

			action.SetSubtype(otherName);

			Assert::AreEqual(otherName, action.GetSubtype());
		}

		TEST_METHOD(ActionEventDelay)
		{
			std::int32_t delay = rand(), otherDelay = rand();
			ActionEvent action;

			Assert::AreEqual(std::int32_t(), action.GetDelay());

			action.SetDelay(delay);

			Assert::AreEqual(delay, action.GetDelay());

			action.SetDelay(otherDelay);

			Assert::AreEqual(otherDelay, action.GetDelay());
		}

		TEST_METHOD(ActionEventUpdate)
		{
			WorldState worldState;
			ActionEvent action;

			action.Update(worldState);

			Assert::IsTrue(&action == worldState.mAction);

			World world;
			worldState.mWorld = &world;

			action.Update(worldState);

			Assert::AreEqual(1U, world.QueueSize());

			worldState.SetGameTime(worldState.GetGameTime() + std::chrono::milliseconds(1));
			world.Update(worldState);

			Assert::AreEqual(0U, world.QueueSize());
		}

		TEST_METHOD(ActionEventCopy)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";
			ActionEvent action, childOne, childTwo, childThree;

			action.Adopt(childOne, stringValue);
			action.Adopt(childTwo, nextStringValue);
			action.Adopt(childThree, thirdStringValue);

			Scope* copy = action.Copy();

			Assert::IsTrue(*copy == action);
			Assert::IsTrue(copy->Is(ActionEvent::TypeIdClass()));
			Assert::IsTrue(copy->Find(stringValue)->Get<Scope&>().Is(ActionEvent::TypeIdClass()));
			Assert::IsTrue(copy->Find(nextStringValue)->Get<Scope&>().Is(ActionEvent::TypeIdClass()));
			Assert::IsTrue(copy->Find(thirdStringValue)->Get<Scope&>().Is(ActionEvent::TypeIdClass()));

			delete(copy);
		}

		TEST_METHOD(ActionEventRTTIMethods)
		{
			std::string name = "hello";
			ActionEvent action(name);

			Assert::IsTrue(action.Is(ActionEvent::TypeIdClass()));
			Assert::IsTrue(action.Is(ActionEvent::TypeName()));
			Assert::AreEqual(ActionEvent::TypeIdClass(), action.TypeIdInstance());
			Assert::AreEqual(ActionEvent::TypeName(), action.TypeName());

			RTTI* asRTTI = &action;

			Assert::IsFalse(nullptr == asRTTI->QueryInterface(ActionEvent::TypeIdClass()));
			Assert::IsTrue(action == *(asRTTI->As<ActionEvent>()));
			Assert::AreEqual(asRTTI->As<ActionEvent>()->ToString(), action.ToString());
		}

	private:

		static _CrtMemState sStartMemState;
	};

	_CrtMemState ActionEventTest::sStartMemState;
}
