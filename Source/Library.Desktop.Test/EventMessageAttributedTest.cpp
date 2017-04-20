#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(EventMessageAttributedTest)
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

		TEST_METHOD(EventMessageAttributedConstructor)
		{
			std::string name = "hello";
			EventMessageAttributed message(name);

			Assert::IsTrue(message.IsPrescribedAttribute(EventMessageAttributed::sEventMessageAttributedSubtypeKey));
			Assert::IsTrue(message.GetSubtype() == name);
			Assert::IsNull(message.GetWorldState());

			WorldState worldState;
			EventMessageAttributed otherMessage(name, &worldState);

			Assert::IsTrue(otherMessage.IsPrescribedAttribute(EventMessageAttributed::sEventMessageAttributedSubtypeKey));
			Assert::IsTrue(otherMessage.GetSubtype() == name);
			Assert::IsTrue(otherMessage.GetWorldState() == &worldState);
		}

		TEST_METHOD(EventMessageAttributedDestructor)
		{
			std::string name = "hello";
			EventMessageAttributed* message = new EventMessageAttributed(name);

			delete(message);

			message = new EventMessageAttributed(name);
			message->AppendAuxiliaryAttribute(name) = name;

			Assert::IsTrue(message->IsAuxiliaryAttribute(name));

			delete(message);
		}

		TEST_METHOD(EventMessageAttributedCopyConstructor)
		{
			std::string name = "hello";
			EventMessageAttributed message(name);

			EventMessageAttributed otherMessage = message;

			Assert::IsTrue(otherMessage == message);
			Assert::IsTrue(otherMessage.GetSubtype() == message.GetSubtype());
		}

		TEST_METHOD(EventMessageAttributedAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			EventMessageAttributed message(name);

			EventMessageAttributed otherMessage(otherName);
			otherMessage = message;

			Assert::IsTrue(otherMessage == message);
			Assert::IsTrue(otherMessage.GetSubtype() == message.GetSubtype());

			otherMessage = otherMessage;

			Assert::IsTrue(otherMessage == message);
			Assert::IsTrue(otherMessage.GetSubtype() == message.GetSubtype());
		}

		TEST_METHOD(EventMessageAttributedMoveConstructor)
		{
			std::string name = "hello";
			EventMessageAttributed message(name);

			EventMessageAttributed otherMessage = std::move(message);

			Assert::IsTrue(otherMessage.GetSubtype() == name);
			Assert::IsTrue(otherMessage.IsPrescribedAttribute(EventMessageAttributed::sEventMessageAttributedSubtypeKey));
			Assert::IsTrue(message.GetSubtype() == std::string());
		}

		TEST_METHOD(EventMessageAttributedMoveAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			EventMessageAttributed message(name);

			EventMessageAttributed otherMessage(otherName);

			otherMessage = std::move(message);

			Assert::IsTrue(otherMessage.GetSubtype() == name);
			Assert::IsTrue(otherMessage.IsPrescribedAttribute(EventMessageAttributed::sEventMessageAttributedSubtypeKey));
			Assert::IsTrue(message.GetSubtype() == std::string());

			otherMessage = std::move(otherMessage);

			Assert::IsTrue(otherMessage.GetSubtype() == name);
			Assert::IsTrue(otherMessage.IsPrescribedAttribute(EventMessageAttributed::sEventMessageAttributedSubtypeKey));
			Assert::IsTrue(message.GetSubtype() == std::string());
		}

		TEST_METHOD(EventMessageAttributedSubtype)
		{
			std::string name = "hello", otherName = "howdy";
			EventMessageAttributed message(name);

			Assert::AreEqual(name, message.GetSubtype());

			message.SetSubtype(otherName);

			Assert::AreEqual(otherName, message.GetSubtype());
		}

		TEST_METHOD(EventMessageAttributedWorldState)
		{
			std::string name = "hello";
			EventMessageAttributed message(name);

			Assert::IsNull(message.GetWorldState());

			WorldState worldState;

			message.SetWorldState(worldState);

			Assert::IsTrue(&worldState == message.GetWorldState());
		}

		TEST_METHOD(EventMessageAttributedCopy)
		{
			std::string stringValue = "Howdy";
			EventMessageAttributed message(stringValue);

			Scope* copy = message.Copy();

			Assert::IsTrue(*copy == message);
			Assert::IsTrue(copy->Is(EventMessageAttributed::TypeIdClass()));

			delete(copy);
		}

		TEST_METHOD(EventMessageAttributedRTTIMethods)
		{
			std::string name = "hello";
			EventMessageAttributed message(name);

			Assert::IsTrue(message.Is(EventMessageAttributed::TypeIdClass()));
			Assert::IsTrue(message.Is(EventMessageAttributed::TypeName()));
			Assert::AreEqual(EventMessageAttributed::TypeIdClass(), message.TypeIdInstance());
			Assert::AreEqual(EventMessageAttributed::TypeName(), message.TypeName());

			RTTI* asRTTI = &message;

			Assert::IsFalse(nullptr == asRTTI->QueryInterface(EventMessageAttributed::TypeIdClass()));
			Assert::IsTrue(message == *(asRTTI->As<EventMessageAttributed>()));
			Assert::AreEqual(asRTTI->As<EventMessageAttributed>()->ToString(), message.ToString());
		}

	private:

		static _CrtMemState sStartMemState;
	};

	_CrtMemState EventMessageAttributedTest::sStartMemState;
}
