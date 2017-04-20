#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{

	TEST_CLASS(FactoryTest)
	{

	public:

#pragma region

		ConcreteFactory(RTTI, Foo);

		ConcreteFactory(RTTI, Scope);

		ConcreteFactory(AttributedFoo, AttributedFoo);

#pragma endregion Factory Definitions

	private:

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

		TEST_METHOD(FactoryConstructor)
		{
			FooFactory fooFactory;
			ScopeFactory scopeFactory;
			AttributedFooFactory attributedFooFactory;

			Assert::IsTrue(Factory<RTTI>::Find(fooFactory.GetClassName()) == &fooFactory);
			Assert::IsTrue(Factory<RTTI>::Find(scopeFactory.GetClassName()) == &scopeFactory);
			Assert::IsTrue(Factory<RTTI>::Find(attributedFooFactory.GetClassName()) == nullptr);

			Assert::IsTrue(Factory<AttributedFoo>::Find(fooFactory.GetClassName()) == nullptr);
			Assert::IsTrue(Factory<AttributedFoo>::Find(scopeFactory.GetClassName()) == nullptr);
			Assert::IsTrue(Factory<AttributedFoo>::Find(attributedFooFactory.GetClassName()) == &attributedFooFactory);

			FooFactory otherFooFactory;
			ScopeFactory otherScopeFactory;
			AttributedFooFactory otherAttributedFooFactory;

			Assert::IsTrue(Factory<RTTI>::Find(otherFooFactory.GetClassName()) == &fooFactory);
			Assert::IsTrue(Factory<RTTI>::Find(otherScopeFactory.GetClassName()) == &scopeFactory);
			Assert::IsTrue(Factory<AttributedFoo>::Find(otherAttributedFooFactory.GetClassName()) == &attributedFooFactory);
		}

		TEST_METHOD(FactoryDestructor)
		{
			FooFactory* fooFactory = new FooFactory();
			FooFactory* otherFooFactory = new FooFactory();
			ScopeFactory* scopeFactory = new ScopeFactory();
			ScopeFactory* otherScopeFactory = new ScopeFactory();
			AttributedFooFactory* attributedFooFactory = new AttributedFooFactory();
			AttributedFooFactory* otherAttributedFooFactory = new AttributedFooFactory();

			delete(fooFactory);
			delete(scopeFactory);
			delete(attributedFooFactory);

			Assert::IsTrue(Factory<RTTI>::Find(otherFooFactory->GetClassName()) == nullptr);
			Assert::IsTrue(Factory<RTTI>::Find(otherScopeFactory->GetClassName()) == nullptr);
			Assert::IsTrue(Factory<AttributedFoo>::Find(otherAttributedFooFactory->GetClassName()) == nullptr);

			delete(otherFooFactory);
			delete(otherScopeFactory);
			delete(otherAttributedFooFactory);
		}

		TEST_METHOD(FactoryGetClassName)
		{
			FooFactory fooFactory;
			ScopeFactory scopeFactory;
			AttributedFooFactory attributedFooFactory;

			Assert::IsTrue(fooFactory.GetClassName() == "Foo");
			Assert::IsTrue(scopeFactory.GetClassName() == "Scope");
			Assert::IsTrue(attributedFooFactory.GetClassName() == "AttributedFoo");
		}

		TEST_METHOD(FactoryCreate)
		{
			FooFactory fooFactory;
			ScopeFactory scopeFactory;
			AttributedFooFactory attributedFooFactory;

			RTTI* foo = Factory<RTTI>::Create("Foo");
			RTTI* scope = Factory<RTTI>::Create("Scope");
			AttributedFoo* attributed = Factory<AttributedFoo>::Create("AttributedFoo");

			Assert::IsTrue(foo->Is(Foo::TypeIdClass()));
			Assert::IsTrue(scope->Is(Scope::TypeIdClass()));
			Assert::IsTrue(attributed->Is(AttributedFoo::TypeIdClass()));

			delete(foo);
			delete(scope);
			delete(attributed);

			foo = fooFactory.Create();
			scope = scopeFactory.Create();
			attributed = attributedFooFactory.Create();

			Assert::IsTrue(foo->Is(Foo::TypeIdClass()));
			Assert::IsTrue(scope->Is(Scope::TypeIdClass()));
			Assert::IsTrue(attributed->Is(AttributedFoo::TypeIdClass()));

			delete(foo);
			delete(scope);
			delete(attributed);

			RTTI* thingWithNoFactory = Factory<RTTI>::Create("NoFactory");
			AttributedFoo* otherThingWithNoFactory = Factory<AttributedFoo>::Create("NoFactory");
			
			Assert::IsNull(thingWithNoFactory);
			Assert::IsNull(otherThingWithNoFactory);
		}

		TEST_METHOD(FactoryFind)
		{
			FooFactory fooFactory;
			ScopeFactory scopeFactory;
			AttributedFooFactory attributedFooFactory;

			Factory<RTTI>* fooPointer = Factory<RTTI>::Find(fooFactory.GetClassName());
			Factory<RTTI>* scopePointer = Factory<RTTI>::Find(scopeFactory.GetClassName());
			Factory<RTTI>* unknownRTTIFactory = Factory<RTTI>::Find("NoFactory");
			Factory<AttributedFoo>* attributedPointer = Factory<AttributedFoo>::Find(attributedFooFactory.GetClassName());
			Factory<AttributedFoo>* unknownAttributedPointer = Factory<AttributedFoo>::Find("NoFactory");

			Assert::IsTrue(fooPointer == &fooFactory);
			Assert::IsTrue(scopePointer == &scopeFactory);
			Assert::IsTrue(unknownRTTIFactory == nullptr);
			Assert::IsTrue(attributedPointer == &attributedFooFactory);
			Assert::IsTrue(unknownAttributedPointer == nullptr);
		}

		TEST_METHOD(Factorybeginend)
		{
			FooFactory fooFactory;
			ScopeFactory scopeFactory;
			AttributedFooFactory attributedFooFactory;

			Factory<RTTI>::Iterator it = Factory<RTTI>::begin();
			Factory<AttributedFoo>::Iterator attributedIt = Factory<AttributedFoo>::begin();

			Assert::IsTrue(it->second == &fooFactory);
			Assert::IsTrue(attributedIt->second == &attributedFooFactory);

			++it;
			++attributedIt;

			Assert::IsTrue(it->second == &scopeFactory);
			Assert::IsTrue(attributedIt == Factory<AttributedFoo>::end());

			++it;

			Assert::IsTrue(it == Factory<RTTI>::end());
		}

	private:

		static _CrtMemState sStartMemState;

	};

	_CrtMemState FactoryTest::sStartMemState;

}