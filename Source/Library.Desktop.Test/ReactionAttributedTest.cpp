#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(ReactionAttributedTest)
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

		TEST_METHOD(ReactionAttributedConstructor)
		{
			std::string name = "hello";
			ReactionAttributed reaction;

			Assert::IsTrue(reaction.IsPrescribedAttribute(ReactionAttributed::sReactionAttributedSubtypesKey));
			Assert::IsTrue(reaction.IsPrescribedAttribute(Action::sActionsKey));

			ReactionAttributed otherReaction(name);

			Assert::IsTrue(otherReaction.Name() == name);
		}

		TEST_METHOD(ReactionAttributedDestructor)
		{
			std::string name = "hello";
			ReactionAttributed* reaction = new ReactionAttributed();

			delete(reaction);

			reaction = new ReactionAttributed(name);
			reaction->AppendAuxiliaryAttribute(name) = name;

			Assert::IsTrue(reaction->IsAuxiliaryAttribute(name));

			delete(reaction);
		}

		TEST_METHOD(ReactionAttributedCopyConstructor)
		{
			std::string name = "hello";
			ReactionAttributed reaction(name);

			ReactionAttributed otherReaction = reaction;

			Assert::IsTrue(otherReaction == reaction);
			Assert::IsTrue(otherReaction.Name() == reaction.Name());
		}

		TEST_METHOD(ReactionAttributedAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			ReactionAttributed reaction(name);

			ReactionAttributed otherReaction(otherName);
			otherReaction = reaction;

			Assert::IsTrue(otherReaction == reaction);
			Assert::IsTrue(otherReaction.Name() == reaction.Name());

			otherReaction = otherReaction;

			Assert::IsTrue(otherReaction == reaction);
			Assert::IsTrue(otherReaction.Name() == reaction.Name());
		}

		TEST_METHOD(ReactionAttributedMoveConstructor)
		{
			std::string name = "hello";
			ReactionAttributed reaction(name);

			ReactionAttributed otherReaction = std::move(reaction);

			Assert::IsTrue(otherReaction.Name() == name);
			Assert::IsTrue(otherReaction.IsPrescribedAttribute(ReactionAttributed::sReactionAttributedSubtypesKey));
			Assert::IsTrue(otherReaction.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsTrue(reaction.Name() == std::string());
		}

		TEST_METHOD(ReactionAttributedMoveAssignmentOperator)
		{
			std::string name = "hello", otherName = "howdy";
			ReactionAttributed reaction(name);

			ReactionAttributed otherReaction(otherName);

			otherReaction = std::move(reaction);

			Assert::IsTrue(otherReaction.Name() == name);
			Assert::IsTrue(otherReaction.IsPrescribedAttribute(ReactionAttributed::sReactionAttributedSubtypesKey));
			Assert::IsTrue(otherReaction.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsTrue(reaction.Name() == std::string());

			otherReaction = std::move(otherReaction);

			Assert::IsTrue(otherReaction.Name() == name);
			Assert::IsTrue(otherReaction.IsPrescribedAttribute(ReactionAttributed::sReactionAttributedSubtypesKey));
			Assert::IsTrue(otherReaction.IsPrescribedAttribute(Action::sActionsKey));
			Assert::IsTrue(reaction.Name() == std::string());
		}

		TEST_METHOD(ReactionAttributedNotify)
		{
			std::string subtype = "hello";
			WorldState worldState;
			EventMessageAttributed message(subtype, &worldState);
			Event<EventMessageAttributed>* e = new Event<EventMessageAttributed>(message);

			ReactionAttributed reaction;

			Action* action = reaction.CreateAction("ActionList", "list");

			reaction.Notify(*e);

			Assert::IsNull(worldState.mAction);

			reaction.Append(ReactionAttributed::sReactionAttributedSubtypesKey).PushBack(subtype);

			reaction.Notify(*e);

			Assert::IsTrue(action == worldState.mAction);

			delete(e);
		}

		TEST_METHOD(ReactionAttributedCopy)
		{
			std::string stringValue = "Howdy";
			ReactionAttributed reaction;

			Scope* copy = reaction.Copy();

			Assert::IsTrue(*copy == reaction);
			Assert::IsTrue(copy->Is(ReactionAttributed::TypeIdClass()));

			delete(copy);
		}

		TEST_METHOD(ReactionAttributedRTTIMethods)
		{
			ReactionAttributed reaction;

			Assert::IsTrue(reaction.Is(ReactionAttributed::TypeIdClass()));
			Assert::IsTrue(reaction.Is(ReactionAttributed::TypeName()));
			Assert::IsTrue(reaction.Is(Reaction::TypeIdClass()));
			Assert::IsTrue(reaction.Is(Reaction::TypeName()));
			Assert::AreEqual(ReactionAttributed::TypeIdClass(), reaction.TypeIdInstance());
			Assert::AreEqual(ReactionAttributed::TypeName(), reaction.TypeName());

			RTTI* asRTTI = &reaction;
			Reaction* asReaction = &reaction;

			Assert::IsFalse(nullptr == asRTTI->QueryInterface(ReactionAttributed::TypeIdClass()));
			Assert::IsTrue(reaction == *(asRTTI->As<ReactionAttributed>()));
			Assert::AreEqual(asRTTI->As<ReactionAttributed>()->ToString(), reaction.ToString());
			Assert::IsTrue(asReaction == asRTTI->As<Reaction>());
		}

	private:

		static _CrtMemState sStartMemState;

		ActionListFactory mActionListFactory;
	};

	_CrtMemState ReactionAttributedTest::sStartMemState;
}
