#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(AttributedTest)
	{
	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			AttributedFoo::ClearPrescribedAttributes();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);

			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}

		TEST_METHOD(AttributedConstructor)
		{
			AttributedFoo attributed;

			Assert::IsTrue(attributed.IsAttribute(Attributed::sAttributedThisKey));
			Assert::IsTrue(attributed.IsPrescribedAttribute(Attributed::sAttributedThisKey));
			Assert::IsFalse(attributed.IsAuxiliaryAttribute(Attributed::sAttributedThisKey));
			Assert::IsFalse(attributed.IsAttribute("Hi"));
			Assert::IsTrue(attributed[Attributed::sAttributedThisKey] == &attributed);
			Assert::AreEqual(1U, attributed[Attributed::sAttributedThisKey].Size());
		}

		TEST_METHOD(AttributedDestructor)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";
			AttributedFoo attributed;

			attributed.AppendAuxiliaryAttribute(stringValue) = stringValue;
			attributed.AppendAuxiliaryAttribute(nextStringValue) = nextStringValue;
			attributed.AppendAuxiliaryAttribute(thirdStringValue) = thirdStringValue;
		}

		TEST_METHOD(AttributedCopyConstructor)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";
			AttributedFoo attributed;

			attributed.AppendAuxiliaryAttribute(stringValue) = stringValue;
			attributed.AppendAuxiliaryAttribute(nextStringValue) = nextStringValue;
			attributed.AppendAuxiliaryAttribute(thirdStringValue) = thirdStringValue;

			AttributedFoo otherAttributed(attributed);

			Assert::IsTrue(attributed == otherAttributed);
			Assert::IsTrue(otherAttributed.IsAuxiliaryAttribute(stringValue));
			Assert::IsTrue(otherAttributed.IsAuxiliaryAttribute(nextStringValue));
			Assert::IsTrue(otherAttributed.IsAuxiliaryAttribute(thirdStringValue));
			Assert::IsTrue(otherAttributed[Attributed::sAttributedThisKey] == &otherAttributed);
		}

		TEST_METHOD(AttributedAssignmentOperator)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";
			AttributedFoo attributed, otherAttributed;

			attributed.AppendAuxiliaryAttribute(stringValue) = stringValue;
			attributed.AppendAuxiliaryAttribute(nextStringValue) = nextStringValue;

			otherAttributed = attributed;

			Assert::IsTrue(attributed == otherAttributed);
			Assert::IsTrue(otherAttributed.IsAuxiliaryAttribute(stringValue));
			Assert::IsTrue(otherAttributed.IsAuxiliaryAttribute(nextStringValue));

			Assert::IsTrue(otherAttributed[Attributed::sAttributedThisKey] == &otherAttributed);

			attributed.AppendAuxiliaryAttribute(thirdStringValue) = thirdStringValue;

			otherAttributed = attributed;

			Assert::IsTrue(attributed == otherAttributed);
			Assert::IsTrue(otherAttributed.IsAuxiliaryAttribute(stringValue));
			Assert::IsTrue(otherAttributed.IsAuxiliaryAttribute(nextStringValue));
			Assert::IsTrue(otherAttributed.IsAuxiliaryAttribute(thirdStringValue));

			Assert::IsTrue(otherAttributed[Attributed::sAttributedThisKey] == &otherAttributed);
		}

		TEST_METHOD(AttributedMoveConstructor)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";
			AttributedFoo attributed;

			attributed.AppendAuxiliaryAttribute(stringValue) = stringValue;
			attributed.AppendAuxiliaryAttribute(nextStringValue) = nextStringValue;
			attributed.AppendAuxiliaryAttribute(thirdStringValue) = thirdStringValue;

			AttributedFoo otherAttributed = std::move(attributed);

			Assert::IsTrue(otherAttributed.IsAuxiliaryAttribute(stringValue));
			Assert::IsTrue(otherAttributed.IsAuxiliaryAttribute(nextStringValue));
			Assert::IsTrue(otherAttributed.IsAuxiliaryAttribute(thirdStringValue));

			Assert::IsTrue(otherAttributed[Attributed::sAttributedThisKey] == &otherAttributed);
		}

		TEST_METHOD(AttributedMoveAssignmentOperator)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";
			AttributedFoo attributed, otherAttributed;

			attributed.AppendAuxiliaryAttribute(stringValue) = stringValue;
			attributed.AppendAuxiliaryAttribute(nextStringValue) = nextStringValue;
			attributed.AppendAuxiliaryAttribute(thirdStringValue) = thirdStringValue;

			otherAttributed = std::move(attributed);

			Assert::IsTrue(otherAttributed.IsAuxiliaryAttribute(stringValue));
			Assert::IsTrue(otherAttributed.IsAuxiliaryAttribute(nextStringValue));
			Assert::IsTrue(otherAttributed.IsAuxiliaryAttribute(thirdStringValue));

			Assert::IsTrue(otherAttributed[Attributed::sAttributedThisKey] == &otherAttributed);
		}

		TEST_METHOD(AttributedCopy)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello", unknownStringValue = "Unknown";
			AttributedFoo attributed, childOne, childTwo, childThree;

			attributed.Adopt(childOne, stringValue);
			attributed.Adopt(childTwo, nextStringValue);
			attributed.Adopt(childThree, thirdStringValue);

			Scope* copy = attributed.Copy();

			Assert::IsTrue(*copy == attributed);
			Assert::IsTrue(copy->Find(stringValue)->Get<Scope&>().Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(copy->Find(nextStringValue)->Get<Scope&>().Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(copy->Find(thirdStringValue)->Get<Scope&>().Is(AttributedFoo::TypeIdClass()));

			delete(copy);
		}

		TEST_METHOD(AttributedEqualityOperator)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";
			AttributedFoo attributed, otherAttributed;

			attributed.AppendAuxiliaryAttribute(stringValue) = stringValue;
			attributed.AppendAuxiliaryAttribute(nextStringValue) = nextStringValue;

			otherAttributed = attributed;

			Assert::IsTrue(attributed == otherAttributed);
		}

		TEST_METHOD(AttributedNotEqualOperator)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";
			AttributedFoo attributed, otherAttributed;

			attributed.AppendAuxiliaryAttribute(stringValue) = stringValue;
			attributed.AppendAuxiliaryAttribute(nextStringValue) = nextStringValue;

			otherAttributed = attributed;

			Assert::IsFalse(attributed != otherAttributed);
		}

		TEST_METHOD(AttributedEquals)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";
			AttributedFoo attributed, otherAttributed;

			attributed.AppendAuxiliaryAttribute(stringValue) = stringValue;
			attributed.AppendAuxiliaryAttribute(nextStringValue) = nextStringValue;

			otherAttributed = attributed;

			Assert::IsTrue(attributed.Equals(&otherAttributed));

			Assert::IsFalse(attributed.Equals(nullptr));
		}

		TEST_METHOD(AttributedFooInitializeSignatures)
		{
			AttributedFoo attributed;

			Assert::IsTrue(attributed.IsPrescribedAttribute("Int"));
			Assert::IsTrue(attributed.IsPrescribedAttribute("Float"));
			Assert::IsTrue(attributed.IsPrescribedAttribute("Vector"));
			Assert::IsTrue(attributed.IsPrescribedAttribute("Matrix"));
			Assert::IsTrue(attributed.IsPrescribedAttribute("String"));

			Assert::IsTrue(attributed.IsPrescribedAttribute("ExternalInt"));
			Assert::IsTrue(attributed.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(attributed.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(attributed.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(attributed.IsPrescribedAttribute("ExternalString"));

			Assert::IsTrue(attributed.IsPrescribedAttribute("ExternalIntArray"));
			Assert::IsTrue(attributed.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(attributed.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(attributed.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(attributed.IsPrescribedAttribute("ExternalStringArray"));
		}

		TEST_METHOD(AttributedIsPrescribedAttribute)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";
			AttributedFoo attributed;

			attributed.AppendAuxiliaryAttribute(stringValue) = stringValue;
			attributed.AppendAuxiliaryAttribute(nextStringValue) = nextStringValue;
			attributed.AppendAuxiliaryAttribute(thirdStringValue) = thirdStringValue;

			Assert::IsTrue(attributed.IsPrescribedAttribute(Attributed::sAttributedThisKey));
			Assert::IsFalse(attributed.IsPrescribedAttribute(stringValue));
			Assert::IsFalse(attributed.IsPrescribedAttribute(nextStringValue));
			Assert::IsFalse(attributed.IsPrescribedAttribute(thirdStringValue));
		}

		TEST_METHOD(AttributedIsAuxiliaryAttribute)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";
			AttributedFoo attributed;

			attributed.AppendAuxiliaryAttribute(stringValue) = stringValue;
			attributed.AppendAuxiliaryAttribute(nextStringValue) = nextStringValue;
			attributed.AppendAuxiliaryAttribute(thirdStringValue) = thirdStringValue;

			Assert::IsFalse(attributed.IsAuxiliaryAttribute(Attributed::sAttributedThisKey));
			Assert::IsTrue(attributed.IsAuxiliaryAttribute(stringValue));
			Assert::IsTrue(attributed.IsAuxiliaryAttribute(nextStringValue));
			Assert::IsTrue(attributed.IsAuxiliaryAttribute(thirdStringValue));
		}

		TEST_METHOD(AttributedIsAttribute)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";
			AttributedFoo attributed;

			attributed.AppendAuxiliaryAttribute(stringValue) = stringValue;
			attributed.AppendAuxiliaryAttribute(nextStringValue) = nextStringValue;

			Assert::IsTrue(attributed.IsAttribute(Attributed::sAttributedThisKey));
			Assert::IsTrue(attributed.IsAttribute(stringValue));
			Assert::IsTrue(attributed.IsAttribute(nextStringValue));
			Assert::IsFalse(attributed.IsAttribute(thirdStringValue));
		}

		TEST_METHOD(AttributedAppendAuxiliaryAttribute)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";
			AttributedFoo attributed;

			attributed.AppendAuxiliaryAttribute(stringValue) = stringValue;
			attributed.AppendAuxiliaryAttribute(nextStringValue) = nextStringValue;
			attributed.AppendAuxiliaryAttribute(thirdStringValue) = thirdStringValue;

			auto AppendAuxiliaryAttributedPointer = [&attributed] { attributed.AppendAuxiliaryAttribute(Attributed::sAttributedThisKey); };
			Assert::ExpectException<std::exception>(AppendAuxiliaryAttributedPointer);

			Assert::IsTrue(attributed.IsAttribute(stringValue));
			Assert::IsTrue(attributed.IsAttribute(nextStringValue));
			Assert::IsTrue(attributed.IsAttribute(thirdStringValue));

			Assert::IsTrue(attributed.IsAuxiliaryAttribute(stringValue));
			Assert::IsTrue(attributed.IsAuxiliaryAttribute(nextStringValue));
			Assert::IsTrue(attributed.IsAuxiliaryAttribute(thirdStringValue));
		}

		TEST_METHOD(AttributedCopyAuxiliaryAttributesInto)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";
			AttributedFoo attributed, otherAttributed, thirdAttributed;

			attributed.AppendAuxiliaryAttribute(stringValue) = stringValue;
			attributed.AppendAuxiliaryAttribute(nextStringValue) = nextStringValue;

			attributed.CopyAuxiliaryAttributesInto(otherAttributed);

			Assert::IsTrue(otherAttributed.IsAuxiliaryAttribute(stringValue));
			Assert::IsTrue(otherAttributed.IsAuxiliaryAttribute(nextStringValue));
			Assert::IsFalse(otherAttributed.IsAuxiliaryAttribute(thirdStringValue));

			attributed.AppendAuxiliaryAttribute(thirdStringValue) = thirdStringValue;

			attributed.CopyAuxiliaryAttributesInto(thirdAttributed);

			Assert::IsTrue(thirdAttributed.IsAuxiliaryAttribute(stringValue));
			Assert::IsTrue(thirdAttributed.IsAuxiliaryAttribute(nextStringValue));
			Assert::IsTrue(thirdAttributed.IsAuxiliaryAttribute(thirdStringValue));
		}

		TEST_METHOD(AttributedRTTIMethods)
		{
			AttributedFoo attributed;

			Assert::IsTrue(attributed.Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(attributed.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(attributed.Is(AttributedFoo::TypeName()));
			Assert::IsTrue(attributed.Is(Attributed::TypeName()));
			Assert::AreEqual(AttributedFoo::TypeIdClass(), attributed.TypeIdInstance());
			Assert::AreNotEqual(Attributed::TypeIdClass(), attributed.TypeIdInstance());
			Assert::AreEqual(AttributedFoo::TypeName(), attributed.TypeName());
			Assert::AreNotEqual(Attributed::TypeName(), attributed.TypeName());

			RTTI* asRTTI = &attributed;

			Assert::IsFalse(nullptr == asRTTI->QueryInterface(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(attributed == *(asRTTI->As<AttributedFoo>()));
			Assert::AreEqual(asRTTI->As<AttributedFoo>()->ToString(), attributed.ToString());
		}

	private:

		static _CrtMemState sStartMemState;
	};

	_CrtMemState AttributedTest::sStartMemState;
}
