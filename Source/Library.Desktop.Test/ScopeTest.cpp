#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(ScopeTest)
	{
	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);

			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}

		TEST_METHOD(ScopeConstructor)
		{
			Scope scope;

			Assert::IsTrue(nullptr == scope.GetParent());

			Scope otherScope(5);

			Assert::IsTrue(nullptr == otherScope.GetParent());
		}

		TEST_METHOD(ScopeDestructor)
		{
			Scope scope;
			std::int32_t intValue = rand();
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";

			scope.Append(stringValue) = intValue;
			scope.Append(nextStringValue) = intValue;
			scope.AppendScope(thirdStringValue);
		}

		TEST_METHOD(ScopeCopyConstructor)
		{
			Scope scope;
			std::int32_t intValue = rand();
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";

			scope.Append(stringValue) = intValue;
			scope.Append(nextStringValue) = intValue;
			scope.Append(thirdStringValue) = intValue;

			Scope otherScope(scope);

			Assert::IsTrue(scope == otherScope);
		}

		TEST_METHOD(ScopeAssignmentOperator)
		{
			Scope scope, otherScope;
			std::int32_t intValue = rand();
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";

			scope.Append(stringValue) = intValue;
			scope.Append(nextStringValue) = intValue;

			otherScope = scope;

			Assert::IsTrue(scope == otherScope);

			scope.Clear();
			scope = otherScope;

			Assert::IsTrue(scope == otherScope);

			otherScope.Clear();
			scope.AppendScope(thirdStringValue);
			otherScope = scope;

			Assert::IsTrue(scope == otherScope);
		}
		
		TEST_METHOD(ScopeMoveConstructor)
		{
			Scope parentScope, scope;
			std::int32_t intValue = rand();
			std::uint32_t index;
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";

			parentScope.Adopt(scope, stringValue);
			Scope& childScope = scope.AppendScope(stringValue);
			scope.Append(nextStringValue) = intValue;
			scope.Append(thirdStringValue) = intValue;

			Scope otherScope = std::move(scope);

			Assert::IsTrue(static_cast<Scope*>(nullptr) == scope.GetParent());
			Assert::IsTrue(parentScope.FindContainedScope(otherScope, index) != nullptr);
			Assert::IsTrue(*otherScope.Find(stringValue) == childScope);
			Assert::IsTrue(*otherScope.Find(nextStringValue) == intValue);
			Assert::IsTrue(*otherScope.Find(thirdStringValue) == intValue);
		}

		TEST_METHOD(ScopeMoveAssignmentOperator)
		{
			Scope scope;
			std::int32_t intValue = rand();
			std::uint32_t index;
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";

			Scope& childScope = scope.AppendScope(stringValue);
			scope.Append(nextStringValue) = intValue;
			scope.Append(thirdStringValue) = intValue;

			Scope parentScope, otherScope;
			parentScope.Adopt(scope, stringValue);

			otherScope = std::move(scope);

			Assert::IsTrue(static_cast<Scope*>(nullptr) == scope.GetParent());
			Assert::IsTrue(parentScope.FindContainedScope(otherScope, index) != nullptr);
			Assert::IsTrue(*otherScope.Find(stringValue) == childScope);
			Assert::AreEqual(1U, parentScope.Find(stringValue)->Size());
			Assert::IsTrue(parentScope[stringValue].Get<Scope&>().GetParent() == &parentScope);
			Assert::IsTrue(otherScope.GetParent() == &parentScope);
			Assert::IsTrue(parentScope[stringValue].Get<Scope&>() != scope);
			Assert::IsTrue(parentScope[stringValue].Get<Scope&>() == otherScope);
			Assert::IsTrue(*otherScope.Find(nextStringValue) == intValue);
			Assert::IsTrue(*otherScope.Find(thirdStringValue) == intValue);
		}

		TEST_METHOD(ScopeFindContainedScope)
		{
			Scope scope, otherScope;
			std::uint32_t index = 5U;
			std::int32_t intValue = rand();
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";

			scope.Append(nextStringValue) = intValue;
			scope.Append(thirdStringValue) = intValue;

			Scope& childScope = scope.AppendScope(stringValue);
			childScope.Append(nextStringValue) = intValue;
			childScope.Append(thirdStringValue) = intValue;

			Datum* datum = scope.FindContainedScope(otherScope, index);

			Assert::IsTrue(datum == nullptr);
			Assert::AreEqual(5U, index);

			datum = scope.FindContainedScope(childScope, index);

			Assert::IsTrue(datum != nullptr);
			Assert::AreEqual(0U, index);
		}

		TEST_METHOD(ScopeFind)
		{
			Scope scope;
			std::int32_t intValue = rand(), nextIntValue = rand(), thirdIntValue = rand();
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";

			Assert::IsTrue(nullptr == scope.Find(stringValue));

			scope.Append(stringValue) = intValue;
			scope.Append(nextStringValue) = nextIntValue;
			scope.Append(thirdStringValue) = thirdIntValue;

			Assert::IsTrue((*scope.Find(stringValue)) == intValue);
			Assert::IsTrue((*scope.Find(nextStringValue)) == nextIntValue);
			Assert::IsTrue((*scope.Find(thirdStringValue)) == thirdIntValue);

			const Scope constScope = scope;

			Assert::IsTrue((*constScope.Find(stringValue)) == intValue);
			Assert::IsTrue((*constScope.Find(nextStringValue)) == nextIntValue);
			Assert::IsTrue((*constScope.Find(thirdStringValue)) == thirdIntValue);
		}

		TEST_METHOD(ScopeSearch)
		{
			Scope scope;
			std::int32_t intValue = rand(), nextIntValue = rand(), thirdIntValue = rand();
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";

			Assert::IsTrue(nullptr == scope.Search(stringValue));

			scope.Append(stringValue) = intValue;
			scope.Append(nextStringValue) = nextIntValue;
			scope.Append(thirdStringValue) = thirdIntValue;

			Assert::IsTrue((*scope.Search(stringValue)) == intValue);
			Assert::IsTrue((*scope.Search(nextStringValue)) == nextIntValue);
			Assert::IsTrue((*scope.Search(thirdStringValue)) == thirdIntValue);

			const Scope constScope = scope;

			Assert::IsTrue((*constScope.Search(stringValue)) == intValue);
			Assert::IsTrue((*constScope.Search(nextStringValue)) == nextIntValue);
			Assert::IsTrue((*constScope.Search(thirdStringValue)) == thirdIntValue);

			Scope otherScope;
			Scope* containingScope = nullptr;
			std::string scopeKey = "Scope Datum", unknownKey = "Unknown";

			scope.Adopt(otherScope, scopeKey);

			Assert::IsTrue((*otherScope.Search(stringValue, &containingScope)) == intValue);
			Assert::IsTrue(containingScope == &scope);

			Assert::IsTrue(nullptr == otherScope.Search(unknownKey, &containingScope));
			Assert::IsTrue(containingScope == nullptr);
		}

		TEST_METHOD(ScopeAppend)
		{
			Scope scope;
			std::int32_t intValue = rand(), nextIntValue = rand(), thirdIntValue = rand();
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";

			scope.Append(stringValue) = intValue;
			scope.Append(nextStringValue) = nextIntValue;
			scope.Append(thirdStringValue) = thirdIntValue;

			Assert::IsTrue(scope[stringValue] == intValue);
			Assert::IsTrue(scope[nextStringValue] == nextIntValue);
			Assert::IsTrue(scope[thirdStringValue] == thirdIntValue);

			Assert::IsTrue(scope.Append(stringValue) == intValue);
			Assert::IsTrue(scope.Append(nextStringValue) == nextIntValue);
			Assert::IsTrue(scope.Append(thirdStringValue) == thirdIntValue);

			Assert::IsTrue(scope[0] == intValue);
			Assert::IsTrue(scope[1] == nextIntValue);
			Assert::IsTrue(scope[2] == thirdIntValue);

			scope.Append(stringValue);

			auto orderAccessorPointer = [&scope] { scope[3]; };

			Assert::ExpectException<std::exception>(orderAccessorPointer);
		}

		TEST_METHOD(ScopeAppendScope)
		{
			Scope scope;
			std::int32_t intValue = rand(), nextIntValue = rand(), thirdIntValue = rand();
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";

			scope.Append(stringValue) = intValue;
			scope.Append(nextStringValue) = nextIntValue;
			scope.Append(thirdStringValue) = thirdIntValue;

			auto appendScopePointer = [&scope, &stringValue] { scope.AppendScope(stringValue); };

			Assert::ExpectException<std::exception>(appendScopePointer);

			Scope otherScope;

			Scope& childScope = otherScope.AppendScope(stringValue);

			Assert::IsTrue(otherScope[stringValue] == childScope);
		}

		TEST_METHOD(ScopeAdopt)
		{
			Scope parentScope, otherParentScope;
			std::int32_t intValue = rand(), nextIntValue = rand(), thirdIntValue = rand();
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";

			otherParentScope.Append(stringValue) = intValue;
			otherParentScope.Append(nextStringValue) = nextIntValue;
			otherParentScope.Append(thirdStringValue) = thirdIntValue;

			auto adoptPointer = [&parentScope, &otherParentScope, &stringValue] { otherParentScope.Adopt(parentScope, stringValue); };

			Assert::ExpectException<std::exception>(adoptPointer);

			auto selfAdoptPointer = [&parentScope, &stringValue] { parentScope.Adopt(parentScope, stringValue); };

			Assert::ExpectException<std::exception>(selfAdoptPointer);

			Scope* scope = &parentScope.AppendScope(stringValue);

			Assert::IsTrue(scope->GetParent() == &parentScope);
			Assert::IsTrue(parentScope[stringValue] == *scope);

			scope->Orphan();
			parentScope.Adopt(*scope, stringValue);

			Assert::IsTrue(scope->GetParent() == &parentScope);
			Assert::IsTrue(parentScope[stringValue].Get<Scope&>() == *scope);

			otherParentScope.Clear();
			otherParentScope.Adopt(*scope, stringValue);

			Assert::IsTrue(scope->GetParent() == &otherParentScope);
			Assert::IsTrue(otherParentScope[stringValue] == *scope);
		}

		TEST_METHOD(ScopeGetParent)
		{
			Scope scope;
			std::string stringValue = "Howdy", nextStringValue = "Hi";

			Scope* childScope = &scope.AppendScope(stringValue);

			Assert::IsTrue(scope.GetParent() == nullptr);
			Assert::IsTrue(childScope->GetParent() == &scope);

			const Scope constScope;

			Assert::IsTrue(constScope.GetParent() == nullptr);
		}

		TEST_METHOD(ScopeHashAccessor)
		{
			Scope scope;
			std::int32_t intValue = rand(), nextIntValue = rand(), thirdIntValue = rand();
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello", unknownStringValue = "Unknown";

			scope.Append(stringValue) = intValue;
			scope.Append(nextStringValue) = nextIntValue;
			scope.Append(thirdStringValue) = thirdIntValue;

			Assert::IsTrue(scope[stringValue] == intValue);
			Assert::IsTrue(scope[nextStringValue] == nextIntValue);
			Assert::IsTrue(scope[thirdStringValue] == thirdIntValue);

			const Scope constScope = scope;
			auto hashAccessorPointer = [&constScope, &unknownStringValue] { constScope[unknownStringValue]; };

			Assert::ExpectException<std::exception>(hashAccessorPointer);
			Assert::IsTrue(constScope[stringValue] == intValue);
			Assert::IsTrue(constScope[nextStringValue] == nextIntValue);
			Assert::IsTrue(constScope[thirdStringValue] == thirdIntValue);
		}

		TEST_METHOD(ScopeOrderVectorAccessor)
		{
			Scope scope;
			std::int32_t intValue = rand(), nextIntValue = rand(), thirdIntValue = rand();
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello", unknownStringValue = "Unknown";

			scope.Append(stringValue) = intValue;
			scope.Append(nextStringValue) = nextIntValue;
			scope.Append(thirdStringValue) = thirdIntValue;

			Assert::IsTrue(scope[0] == intValue);
			Assert::IsTrue(scope[1] == nextIntValue);
			Assert::IsTrue(scope[2] == thirdIntValue);

			auto orderVectorAccessorPointer = [&scope] { scope[3]; };

			Assert::ExpectException<std::exception>(orderVectorAccessorPointer);

			const Scope constScope = scope;
			auto constOrderVectorAccessorPointer = [&constScope] { constScope[3]; };

			Assert::ExpectException<std::exception>(constOrderVectorAccessorPointer);
		}

		TEST_METHOD(ScopeEquality)
		{
			Scope scope, otherScope;

			Assert::IsTrue(scope == otherScope);

			std::int32_t intValue = rand();
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";

			scope.Append(stringValue) = intValue;
			scope.Append(nextStringValue) = intValue;
			scope.AppendScope(thirdStringValue);

			Assert::IsFalse(scope == otherScope);

			otherScope = scope;

			Assert::IsTrue(scope == otherScope);
		}

		TEST_METHOD(ScopeInequality)
		{
			Scope scope, otherScope;

			Assert::IsFalse(scope != otherScope);

			std::int32_t intValue = rand();
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";

			scope.Append(stringValue) = intValue;
			scope.Append(nextStringValue) = intValue;
			scope.AppendScope(thirdStringValue);

			Assert::IsTrue(scope != otherScope);

			otherScope = scope;

			Assert::IsFalse(scope != otherScope);
		}

		TEST_METHOD(ScopeEquals)
		{
			Scope scope, otherScope;

			Assert::IsTrue(scope.Equals(&otherScope));

			std::int32_t intValue = rand();
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";

			scope.Append(stringValue) = intValue;
			scope.Append(nextStringValue) = intValue;
			scope.AppendScope(thirdStringValue);

			Assert::IsFalse(scope.Equals(&otherScope));

			otherScope = scope;

			Assert::IsTrue(scope.Equals(&otherScope));

			Assert::IsFalse(scope.Equals(nullptr));

			Foo notAScope;

			Assert::IsFalse(scope.Equals(&notAScope));
		}

		TEST_METHOD(ScopeClear)
		{
			Scope scope;
			std::int32_t intValue = rand(), nextIntValue = rand(), thirdIntValue = rand();
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello", unknownStringValue = "Unknown";

			scope.Append(stringValue) = intValue;
			scope.Append(nextStringValue) = nextIntValue;
			scope.Append(thirdStringValue) = thirdIntValue;

			scope.Clear();

			Assert::IsTrue(scope[stringValue] != intValue);
			Assert::IsTrue(scope[nextStringValue] != nextIntValue);
			Assert::IsTrue(scope[thirdStringValue] != thirdIntValue);

			scope.Clear();

			scope.AppendScope(stringValue);
			scope.AppendScope(stringValue);
			scope.AppendScope(stringValue);
			scope.AppendScope(stringValue);
			scope.AppendScope(stringValue);
			scope.AppendScope(stringValue);
			scope.AppendScope(nextStringValue);
			scope.AppendScope(nextStringValue);
		}

		TEST_METHOD(ScopeCopy)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello", unknownStringValue = "Unknown";
			Scope scope;

			scope.AppendScope(stringValue);
			scope.AppendScope(nextStringValue);
			scope.AppendScope(thirdStringValue);

			Scope* copy = scope.Copy();

			Assert::IsTrue(scope == *copy);
			Assert::IsTrue(copy->Find(stringValue)->Get<Scope&>().Is(Scope::TypeIdClass()));
			Assert::IsTrue(copy->Find(nextStringValue)->Get<Scope&>().Is(Scope::TypeIdClass()));
			Assert::IsTrue(copy->Find(thirdStringValue)->Get<Scope&>().Is(Scope::TypeIdClass()));

			delete(copy);
		}

		TEST_METHOD(ScopeOrphan)
		{
			Scope scope, otherScope;
			std::string stringValue = "Howdy";

			Scope* child = &scope.AppendScope(stringValue);

			child->Orphan();

			Assert::IsTrue(*scope.Find(stringValue) != child);

			otherScope.Adopt(*child, stringValue);
		}

		TEST_METHOD(ScopeRTTIMethods)
		{
			Scope scope;

			Assert::IsTrue(scope.Is(Scope::TypeIdClass()));
			Assert::IsTrue(scope.Is(Scope::TypeName()));
			Assert::AreEqual(Scope::TypeIdClass(), scope.TypeIdInstance());
			Assert::AreEqual(Scope::TypeName(), scope.TypeName());

			RTTI* asRTTI = &scope;

			Assert::IsFalse(nullptr == asRTTI->QueryInterface(Scope::TypeIdClass()));
			Assert::IsTrue(scope == *(asRTTI->As<Scope>()));
			Assert::AreEqual(asRTTI->As<Scope>()->ToString(), scope.ToString());
		}

	private:

		static _CrtMemState sStartMemState;
	};

	_CrtMemState ScopeTest::sStartMemState;
}
