#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{		
	TEST_CLASS(SListTest)
	{
	public:

		// This gets run for every test in the TEST_CLASS
		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			mIntList.Clear();
			mStringList.Clear();
			mIntPointerList.Clear();
			mFooList.Clear();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);

			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
		
		/*
			Tests SList's Default Constructor by ensuring that a default constructed list is empty.
			Does this for lists of types std::int32_t, std::string, std::int32_t*, and Foo.
		*/
		TEST_METHOD(SListDefaultConstructor)
		{
			Assert::IsTrue(mIntList.IsEmpty());
			Assert::IsTrue(mStringList.IsEmpty());
			Assert::IsTrue(mIntPointerList.IsEmpty());
			Assert::IsTrue(mFooList.IsEmpty());
		}

		/*
			Tests SList's Copy Constructor by ensuring a copied list has the same length, whether populated or not.
			Additionally ensures that the data in the source list is equal to the data in the destination list.
			Ensures that the destination list has a different address than the source list;
			Does this for lists of types std::int32_t, std::string, std::int32_t*, and Foo.
		*/
		TEST_METHOD(SListCopyConstructor)
		{
			SList<std::int32_t>* othermIntList = new SList<std::int32_t>(mIntList);
			SList<std::string>* otherStringList = new SList<std::string>(mStringList);
			SList<std::int32_t*>* otherIntPointerList = new SList<std::int32_t*>(mIntPointerList);
			SList<Foo>* otherFooList = new SList<Foo>(mFooList);

			Assert::AreEqual(mIntList.Size(), othermIntList->Size());
			Assert::AreEqual(mStringList.Size(), otherStringList->Size());
			Assert::AreEqual(mIntPointerList.Size(), otherIntPointerList->Size());
			Assert::AreEqual(mFooList.Size(), otherFooList->Size());

			Assert::AreNotEqual((void*)&mIntList, (void*)othermIntList);
			Assert::AreNotEqual((void*)&mStringList, (void*)otherStringList);
			Assert::AreNotEqual((void*)&mIntPointerList, (void*)otherIntPointerList);
			Assert::AreNotEqual((void*)&mFooList, (void*)otherFooList);

			for (std::uint32_t i = 0; i < numberOfTestItems; ++i)
			{
				std::int32_t randomIntData = rand();
				std::string randomString = std::string("howdy");
				Foo randomFoo = Foo(randomIntData);

				mIntList.PushFront(randomIntData);
				mStringList.PushFront(randomString);
				mIntPointerList.PushFront(&randomIntData);
				mFooList.PushFront(randomFoo);
			}

			delete(othermIntList);
			delete(otherStringList);
			delete(otherIntPointerList);
			delete(otherFooList);

			othermIntList = new SList<std::int32_t>(mIntList);
			otherStringList = new SList<std::string>(mStringList);
			otherIntPointerList = new SList<std::int32_t*>(mIntPointerList);
			otherFooList = new SList<Foo>(mFooList);

			Assert::AreEqual(mIntList.Size(), othermIntList->Size());
			Assert::AreEqual(mStringList.Size(), otherStringList->Size());
			Assert::AreEqual(mIntPointerList.Size(), otherIntPointerList->Size());
			Assert::AreEqual(mFooList.Size(), otherFooList->Size());

			Assert::AreNotEqual((void*)&mIntList, (void*)othermIntList);
			Assert::AreNotEqual((void*)&mStringList, (void*)otherStringList);
			Assert::AreNotEqual((void*)&mIntPointerList, (void*)otherIntPointerList);
			Assert::AreNotEqual((void*)&mFooList, (void*)otherFooList);

			for (std::uint32_t i = 0; i < numberOfTestItems; ++i)
			{
				Assert::AreEqual(mIntList.Front(), othermIntList->Front());
				Assert::AreEqual(mStringList.Front(), otherStringList->Front());
				Assert::AreEqual(mIntPointerList.Front(), otherIntPointerList->Front());
				Assert::IsTrue(mFooList.Front() == otherFooList->Front());

				Assert::AreEqual(mIntList.Back(), othermIntList->Back());
				Assert::AreEqual(mStringList.Back(), otherStringList->Back());
				Assert::AreEqual(mIntPointerList.Back(), otherIntPointerList->Back());
				Assert::IsTrue(mFooList.Back() == otherFooList->Back());

				mIntList.PopFront();
				othermIntList->PopFront();
				mStringList.PopFront();
				otherStringList->PopFront();

				mIntPointerList.PopFront();
				otherIntPointerList->PopFront();
				mFooList.PopFront();
				otherFooList->PopFront();
			}

			delete(othermIntList);
			delete(otherStringList);
			delete(otherIntPointerList);
			delete(otherFooList);
		}

		/*
			Tests SList Assignment Operator by ensuring an assigned list's address is not set during assignment.
			Additionally ensures that an assigned list has the same size as the source list.
			Ensures that when the list is assigned to itself, its size remains the same.
			Ensures that an assigned list contains the same data as the source list.
			Does this for lists of types std::int32_t, std::string, std::int32_t*, and Foo.
		*/
		TEST_METHOD(SListAssignmentOperator)
		{
			SList<std::int32_t> othermIntList;
			SList<std::string> otherStringList;
			SList<std::int32_t*> otherIntPointerList;
			SList<Foo> otherFooList;

			Assert::AreNotEqual((void*)&mIntList, (void*)&othermIntList);
			Assert::AreNotEqual((void*)&mStringList, (void*)&otherStringList);
			Assert::AreNotEqual((void*)&mIntPointerList, (void*)&otherIntPointerList);
			Assert::AreNotEqual((void*)&mFooList, (void*)&otherFooList);

			for (std::uint32_t i = 0; i < numberOfTestItems; ++i)
			{
				std::int32_t randomIntData = rand();
				std::string randomString = std::string("howdy");
				Foo randomFoo = Foo(randomIntData);

				mIntList.PushFront(randomIntData);
				mStringList.PushFront(randomString);
				mIntPointerList.PushFront(&randomIntData);
				mFooList.PushFront(randomFoo);
			}

			Assert::AreNotEqual(mIntList.Size(), othermIntList.Size());
			Assert::AreNotEqual(mStringList.Size(), otherStringList.Size());
			Assert::AreNotEqual(mIntPointerList.Size(), otherIntPointerList.Size());
			Assert::AreNotEqual(mFooList.Size(), otherFooList.Size());

			othermIntList = mIntList;
			otherStringList = mStringList;
			otherIntPointerList = mIntPointerList;
			otherFooList = mFooList;

			Assert::AreEqual(mIntList.Size(), othermIntList.Size());
			Assert::AreEqual(mStringList.Size(), otherStringList.Size());
			Assert::AreEqual(mIntPointerList.Size(), otherIntPointerList.Size());
			Assert::AreEqual(mFooList.Size(), otherFooList.Size());

			Assert::AreNotEqual((void*)&mIntList, (void*)&othermIntList);
			Assert::AreNotEqual((void*)&mStringList, (void*)&otherStringList);
			Assert::AreNotEqual((void*)&mIntPointerList, (void*)&otherIntPointerList);
			Assert::AreNotEqual((void*)&mFooList, (void*)&otherFooList);

			std::uint32_t intSize = othermIntList.Size();
			std::uint32_t stringSize = otherStringList.Size();
			std::uint32_t intPointerSize = otherIntPointerList.Size();
			std::uint32_t fooSize = otherFooList.Size();

			othermIntList = othermIntList;
			otherStringList = otherStringList;
			otherIntPointerList = otherIntPointerList;
			otherFooList = otherFooList;

			Assert::AreEqual(intSize, othermIntList.Size());
			Assert::AreEqual(stringSize, otherStringList.Size());
			Assert::AreEqual(intPointerSize, otherIntPointerList.Size());
			Assert::AreEqual(fooSize, otherFooList.Size());

			for (std::uint32_t i = 0; i < numberOfTestItems; ++i)
			{
				Assert::AreEqual(mIntList.Front(), othermIntList. Front());
				Assert::AreEqual(mStringList.Front(), otherStringList.Front());
				Assert::AreEqual(mIntPointerList.Front(), otherIntPointerList.Front());
				Assert::IsTrue(mFooList.Front() == otherFooList.Front());

				Assert::AreEqual(mIntList.Back(), othermIntList.Back());
				Assert::AreEqual(mStringList.Back(), otherStringList.Back());
				Assert::AreEqual(mIntPointerList.Back(), otherIntPointerList.Back());
				Assert::IsTrue(mFooList.Back() == otherFooList.Back());

				mIntList.PopFront();
				othermIntList.PopFront();
				mStringList.PopFront();
				otherStringList.PopFront();
				mIntPointerList.PopFront();
				otherIntPointerList.PopFront();
				mFooList.PopFront();
				otherFooList.PopFront();
			}
		}

		/*
			Exercises SList Destructor by deleting a new list as well as a populated list.
			Ensures that no memory leaks are detected.
			Does this for lists of types std::int32_t, std::string, std::int32_t*, and Foo.
		*/
		TEST_METHOD(SListDestructor)
		{
			SList<std::int32_t>* mIntListPointer = new SList<std::int32_t>();
			SList<std::string>* mStringListPointer = new SList<std::string>();
			SList<std::int32_t*>* mIntPointerListPointer = new SList<std::int32_t*>();
			SList<Foo>* mFooListPointer = new SList<Foo>();

			delete(mIntListPointer);
			delete(mStringListPointer);
			delete(mIntPointerListPointer);
			delete(mFooListPointer);

			mIntListPointer = new SList<std::int32_t>();
			mStringListPointer = new SList<std::string>();
			mIntPointerListPointer = new SList<std::int32_t*>();
			mFooListPointer = new SList<Foo>();

			for (std::uint32_t i = 0; i < numberOfTestItems; ++i)
			{
				std::int32_t randomIntData = rand();
				std::string randomString = std::string("howdy");
				Foo randomFoo = Foo(randomIntData);

				mIntListPointer->PushFront(randomIntData);
				mStringListPointer->PushFront(randomString);
				mIntPointerListPointer->PushFront(&randomIntData);
				mFooListPointer->PushFront(randomFoo);
			}

			delete(mIntListPointer);
			delete(mStringListPointer);
			delete(mIntPointerListPointer);
			delete(mFooListPointer);
		}

		/*
			Tests PushFront() on SList by ensuring the size is upkept and the Front value is correct.
			Does this for lists of types std::int32_t, std::string, std::int32_t*, and Foo.
		*/
		TEST_METHOD(SListPushFront)
		{
			for (std::uint32_t i = 0; i < numberOfTestItems; ++i)
			{
				std::int32_t randomIntData = rand();
				std::string randomString = std::string("howdy");
				Foo randomFoo = Foo(randomIntData);

				SList<std::int32_t>::Iterator intIt = mIntList.PushFront(randomIntData);
				SList<std::string>::Iterator stringIt = mStringList.PushFront(randomString);
				SList<std::int32_t*>::Iterator intPointerIt = mIntPointerList.PushFront(&randomIntData);
				SList<Foo>::Iterator fooIt = mFooList.PushFront(randomFoo);

				Assert::AreEqual(randomIntData, *intIt);
				Assert::AreEqual(randomString, *stringIt);
				Assert::AreEqual(&randomIntData, *intPointerIt);
				Assert::IsTrue(randomFoo == *fooIt);

				Assert::AreEqual(randomIntData, mIntList.Front());
				Assert::AreEqual(randomString, mStringList.Front());
				Assert::AreEqual(&randomIntData, mIntPointerList.Front());
				Assert::IsTrue(randomFoo == mFooList.Front());

				Assert::AreEqual(i + 1, mIntList.Size());
				Assert::AreEqual(i + 1, mStringList.Size());
				Assert::AreEqual(i + 1, mIntPointerList.Size());
				Assert::AreEqual(i + 1, mFooList.Size());

				if (mIntList.Size() == 1)
				{
					Assert::AreEqual(randomIntData, mIntList.Back());
				}

				if (mStringList.Size() == 1)
				{
					Assert::AreEqual(randomString, mStringList.Back());
				}

				if (mIntPointerList.Size() == 1)
				{
					Assert::AreEqual(&randomIntData, mIntPointerList.Back());
				}

				if (mFooList.Size() == 1)
				{
					Assert::IsTrue(randomFoo == mFooList.Back());
				}
			}
		}

		/*
			Tests PopFront() by ensuring Front() returns the value sent through PushFront() before the last one.
			Additionally ensures that the size of the list was reduced by one after PopFront was called.
			Does this for lists of types std::int32_t, std::string, std::int32_t*, and Foo.
		*/
		TEST_METHOD(SListPopFront)
		{
			for (std::uint32_t i = 0; i < numberOfTestItems; ++i)
			{
				std::int32_t randomIntData = rand(), nextIntData = rand();
				std::string randomString = std::string("howdy"), nextRandomString = std::string("hi");
				Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextIntData);

				mIntList.PushFront(randomIntData);
				mIntList.PushFront(nextIntData);
				mStringList.PushFront(randomString);
				mStringList.PushFront(nextRandomString);
				mIntPointerList.PushFront(&randomIntData);
				mIntPointerList.PushFront(&nextIntData);
				mFooList.PushFront(randomFoo);
				mFooList.PushFront(nextRandomFoo);

				std::uint32_t intSize = mIntList.Size(), stringSize = mStringList.Size(), intPointerSize = mIntPointerList.Size(), fooSize = mFooList.Size();

				mIntList.PopFront();
				mStringList.PopFront();
				mIntPointerList.PopFront();
				mFooList.PopFront();

				Assert::AreEqual(randomIntData, mIntList.Front());
				Assert::AreEqual(randomString, mStringList.Front());
				Assert::AreEqual(&randomIntData, mIntPointerList.Front());
				Assert::IsTrue(randomFoo == mFooList.Front());

				Assert::AreEqual(intSize, mIntList.Size() + 1);
				Assert::AreEqual(stringSize, mStringList.Size() + 1);
				Assert::AreEqual(intPointerSize, mIntPointerList.Size() + 1);
				Assert::AreEqual(fooSize, mFooList.Size() + 1);
			}
		}

		/*
			Tests PushBack() by ensuring the value returned by Back() is equal to the value passed into PushBack().
			Additionally ensures that the size of the list is increased by 1 when PushBack() is called.
			Ensures that Front() also returns the value passed into PushBack() if the list contains one item.
			Does this for lists of types std::int32_t, std::string, std::int32_t*, and Foo.
		*/
		TEST_METHOD(SListPushBack)
		{
			for (std::uint32_t i = 0; i < numberOfTestItems; ++i)
			{
				std::int32_t randomIntData = rand();
				std::string randomString = std::string("howdy");
				Foo randomFoo = Foo(randomIntData);

				SList<std::int32_t>::Iterator intIt = mIntList.PushBack(randomIntData);
				SList<std::string>::Iterator stringIt = mStringList.PushBack(randomString);
				SList<std::int32_t*>::Iterator intPointerIt = mIntPointerList.PushBack(&randomIntData);
				SList<Foo>::Iterator fooIt = mFooList.PushBack(randomFoo);

				Assert::AreEqual(randomIntData, *intIt);
				Assert::AreEqual(randomString, *stringIt);
				Assert::AreEqual(&randomIntData, *intPointerIt);
				Assert::IsTrue(randomFoo == *fooIt);

				Assert::AreEqual(randomIntData, mIntList.Back());
				Assert::AreEqual(randomString, mStringList.Back());
				Assert::AreEqual(&randomIntData, mIntPointerList.Back());
				Assert::IsTrue(randomFoo == mFooList.Back());

				Assert::AreEqual(i + 1, mIntList.Size());
				Assert::AreEqual(i + 1, mStringList.Size());
				Assert::AreEqual(i + 1, mIntPointerList.Size());
				Assert::AreEqual(i + 1, mFooList.Size());

				if (mIntList.Size() == 1)
				{
					Assert::AreEqual(randomIntData, mIntList.Front());
				}

				if (mStringList.Size() == 1)
				{
					Assert::AreEqual(randomString, mStringList.Front());
				}

				if (mIntPointerList.Size() == 1)
				{
					Assert::AreEqual(&randomIntData, mIntPointerList.Front());
				}

				if (mFooList.Size() == 1)
				{
					Assert::IsTrue(randomFoo == mFooList.Front());
				}
			}
		}

		/*
			Tests Clear() to ensure that when called, the list is considered empty.
			Does this for lists of types std::int32_t, std::string, std::int32_t*, and Foo.
		*/
		TEST_METHOD(SListClear)
		{
			for (std::uint32_t i = 0; i < numberOfTestItems; ++i)
			{
				std::int32_t randomIntData = rand();
				std::string randomString = std::string("howdy");
				Foo randomFoo = Foo(randomIntData);

				mIntList.PushFront(randomIntData);
				mStringList.PushFront(randomString);
				mIntPointerList.PushFront(&randomIntData);
				mFooList.PushFront(randomFoo);
			}

			mIntList.Clear();
			mStringList.Clear();
			mIntPointerList.Clear();
			mFooList.Clear();

			Assert::IsTrue(mIntList.IsEmpty());
			Assert::IsTrue(mStringList.IsEmpty());
			Assert::IsTrue(mIntPointerList.IsEmpty());
			Assert::IsTrue(mFooList.IsEmpty());
		}

		/*
			Tests IsEmpty() by ensuring that an initially constructed list with 0 items is considered empty.
			Additionally ensures that as items are added to a list that it is no longer considered empty.
			Ensures that a list is considered empty when all items are removed.
			Does this for lists of types std::int32_t, std::string, std::int32_t*, and Foo.
		*/
		TEST_METHOD(SListIsEmpty)
		{
			Assert::IsTrue(mIntList.IsEmpty());
			Assert::IsTrue(mStringList.IsEmpty());
			Assert::IsTrue(mIntPointerList.IsEmpty());
			Assert::IsTrue(mFooList.IsEmpty());

			for (std::uint32_t i = 0; i < numberOfTestItems; ++i)
			{
				std::int32_t randomIntData = rand();
				std::string randomString = std::string("howdy");
				Foo randomFoo = Foo(randomIntData);

				mIntList.PushFront(randomIntData);
				mStringList.PushFront(randomString);
				mIntPointerList.PushFront(&randomIntData);
				mFooList.PushFront(randomFoo);

				Assert::IsFalse(mIntList.IsEmpty());
				Assert::IsFalse(mStringList.IsEmpty());
				Assert::IsFalse(mIntPointerList.IsEmpty());
				Assert::IsFalse(mFooList.IsEmpty());
			}

			mIntList.Clear();
			mStringList.Clear();
			mIntPointerList.Clear();
			mFooList.Clear();

			Assert::IsTrue(mIntList.IsEmpty());
			Assert::IsTrue(mStringList.IsEmpty());
			Assert::IsTrue(mIntPointerList.IsEmpty());
			Assert::IsTrue(mFooList.IsEmpty());
		}

		/*
			Tests Size() by ensuring that the size of a list is  0 when cleared.
			Additionally ensures that a list's size grows by 1 when an item is added.
			Ensures that a list's size reduces by 1 when an item is popped from the list.
			Does this for lists of types std::int32_t, std::string, std::int32_t*, and Foo.
		*/
		TEST_METHOD(SListSize)
		{
			Assert::AreEqual((std::uint32_t)0, mIntList.Size());
			Assert::AreEqual((std::uint32_t)0, mStringList.Size());
			Assert::AreEqual((std::uint32_t)0, mIntPointerList.Size());
			Assert::AreEqual((std::uint32_t)0, mFooList.Size());

			for (std::uint32_t i = 0; i < numberOfTestItems; ++i)
			{
				std::int32_t randomIntData = rand();
				std::string randomString = std::string("howdy");
				Foo randomFoo = Foo(randomIntData);

				mIntList.PushFront(randomIntData);
				mStringList.PushFront(randomString);
				mIntPointerList.PushFront(&randomIntData);
				mFooList.PushFront(randomFoo);

				Assert::AreEqual((std::uint32_t)(i + 1), mIntList.Size());
				Assert::AreEqual((std::uint32_t)(i + 1), mStringList.Size());
				Assert::AreEqual((std::uint32_t)(i + 1), mIntPointerList.Size());
				Assert::AreEqual((std::uint32_t)(i + 1), mFooList.Size());
			}

			for (std::uint32_t i = numberOfTestItems; i > 0; --i)
			{
				mIntList.PopFront();
				mStringList.PopFront();
				mIntPointerList.PopFront();
				mFooList.PopFront();

				Assert::AreEqual((std::uint32_t)(i - 1), mIntList.Size());
				Assert::AreEqual((std::uint32_t)(i - 1), mStringList.Size());
				Assert::AreEqual((std::uint32_t)(i - 1), mIntPointerList.Size());
				Assert::AreEqual((std::uint32_t)(i - 1), mFooList.Size());
			}
		}

		/*
			Tests Front() by ensuring a std::exception is thrown when a list is empty.
			Additionally ensures that Front() returns the value passed to PushFront().
			Ensures that a value passed to PushBack() does not change the value returned by Front().
			Does this for lists of types std::int32_t, std::string, std::int32_t*, and Foo.
		*/
		TEST_METHOD(SListFront)
		{
			SList<std::int32_t>* mIntListPointer = &mIntList;
			SList<std::string>* mStringListPointer = &mStringList;
			SList<std::int32_t*>* mIntPointerListPointer = &mIntPointerList;
			SList<Foo>* mFooListPointer = &mFooList;

			auto intFrontPointer = [mIntListPointer] { mIntListPointer->Front(); };
			auto stringFrontPointer = [mStringListPointer] { mStringListPointer->Front(); };
			auto intPointerFrontPointer = [mIntPointerListPointer] { mIntPointerListPointer->Front(); };
			auto fooFrontPointer = [mFooListPointer] { mFooListPointer->Front(); };

			Assert::ExpectException<std::exception>(intFrontPointer);
			Assert::ExpectException<std::exception>(stringFrontPointer);
			Assert::ExpectException<std::exception>(intPointerFrontPointer);
			Assert::ExpectException<std::exception>(fooFrontPointer);

			for (std::uint32_t i = 0; i < numberOfTestItems; ++i)
			{
				std::int32_t randomIntData = rand(), nextRandomInt = rand();
				std::string randomString = std::string("howdy"), nextRandomString = std::string("hi");
				Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt);

				mIntList.PushFront(randomIntData);
				mStringList.PushFront(randomString);
				mIntPointerList.PushFront(&randomIntData);
				mFooList.PushFront(randomFoo);

				Assert::AreEqual(randomIntData, mIntList.Front());
				Assert::AreEqual(randomString, mStringList.Front());
				Assert::AreEqual(&randomIntData, mIntPointerList.Front());
				Assert::IsTrue(randomFoo == mFooList.Front());

				mIntList.PushBack(nextRandomInt);
				mStringList.PushBack(nextRandomString);
				mIntPointerList.PushBack(&nextRandomInt);
				mFooList.PushBack(nextRandomFoo);

				Assert::AreEqual(randomIntData, mIntList.Front());
				Assert::AreEqual(randomString, mStringList.Front());
				Assert::AreEqual(&randomIntData, mIntPointerList.Front());
				Assert::IsTrue(randomFoo == mFooList.Front());
			}

			SList<std::int32_t> constIntList;
			SList<std::string> constStringList;
			SList<std::int32_t*> constIntPointerList;
			SList<Foo> constFooList;

			auto constIntFrontPointer = [&constIntList] { constIntList.Front(); };
			auto constStringFrontPointer = [&constStringList] { constStringList.Front(); };
			auto constIntPointerFrontPointer = [&constIntPointerList] { constIntPointerList.Front(); };
			auto constFooFrontPointer = [&constFooList] { constFooList.Front(); };

			Assert::ExpectException<std::exception>(constIntFrontPointer);
			Assert::ExpectException<std::exception>(constStringFrontPointer);
			Assert::ExpectException<std::exception>(constIntPointerFrontPointer);
			Assert::ExpectException<std::exception>(constFooFrontPointer);
		}

		/*
			Tests Back() by ensuring a std::exception is thrown when a list is empty.
			Additionally ensures that Back() returns the value passed to PushBack().
			Ensures that a value passed to PushFront() does not change the value returned by Back().
			Does this for lists of types std::int32_t, std::string, std::int32_t*, and Foo.
		*/
		TEST_METHOD(SListBack)
		{
			SList<std::int32_t>* mIntListPointer = &mIntList;
			SList<std::string>* mStringListPointer = &mStringList;
			SList<std::int32_t*>* mIntPointerListPointer = &mIntPointerList;
			SList<Foo>* mFooListPointer = &mFooList;

			auto intBackPointer = [mIntListPointer] { mIntListPointer->Back(); };
			auto stringBackPointer = [mStringListPointer] { mStringListPointer->Back(); };
			auto intPointerBackPointer = [mIntPointerListPointer] { mIntPointerListPointer->Back(); };
			auto fooBackPointer = [mFooListPointer] { mFooListPointer->Back(); };

			Assert::ExpectException<std::exception>(intBackPointer);
			Assert::ExpectException<std::exception>(stringBackPointer);
			Assert::ExpectException<std::exception>(intPointerBackPointer);
			Assert::ExpectException<std::exception>(fooBackPointer);

			for (std::uint32_t i = 0; i < numberOfTestItems; ++i)
			{
				std::int32_t randomIntData = rand(), nextRandomInt = rand();
				std::string randomString = std::string("howdy"), nextRandomString = std::string("hi");
				Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt);

				mIntList.PushBack(randomIntData);
				mStringList.PushBack(randomString);
				mIntPointerList.PushBack(&randomIntData);
				mFooList.PushBack(randomFoo);

				Assert::AreEqual(randomIntData, mIntList.Back());
				Assert::AreEqual(randomString, mStringList.Back());
				Assert::AreEqual(&randomIntData, mIntPointerList.Back());
				Assert::IsTrue(randomFoo == mFooList.Back());

				mIntList.PushFront(nextRandomInt);
				mStringList.PushFront(nextRandomString);
				mIntPointerList.PushFront(&nextRandomInt);
				mFooList.PushFront(nextRandomFoo);

				Assert::AreEqual(randomIntData, mIntList.Back());
				Assert::AreEqual(randomString, mStringList.Back());
				Assert::AreEqual(&randomIntData, mIntPointerList.Back());
				Assert::IsTrue(randomFoo == mFooList.Back());
			}

			SList<std::int32_t> constIntList;
			SList<std::string> constStringList;
			SList<std::int32_t*> constIntPointerList;
			SList<Foo> constFooList;

			auto constIntFrontPointer = [&constIntList] { constIntList.Back(); };
			auto constStringFrontPointer = [&constStringList] { constStringList.Back(); };
			auto constIntPointerFrontPointer = [&constIntPointerList] { constIntPointerList.Back(); };
			auto constFooFrontPointer = [&constFooList] { constFooList.Back(); };

			Assert::ExpectException<std::exception>(constIntFrontPointer);
			Assert::ExpectException<std::exception>(constStringFrontPointer);
			Assert::ExpectException<std::exception>(constIntPointerFrontPointer);
			Assert::ExpectException<std::exception>(constFooFrontPointer);
		}

		TEST_METHOD(SListbegin)
		{
			SList<std::int32_t>::Iterator intIt = mIntList.begin();
			SList<std::string>::Iterator stringIt = mStringList.begin();
			SList<std::int32_t*>::Iterator intPointerIt = mIntPointerList.begin();
			SList<Foo>::Iterator fooIt = mFooList.begin();

			Assert::IsTrue(mIntList.begin() == mIntList.end());
			Assert::IsTrue(mStringList.begin() == mStringList.end());
			Assert::IsTrue(mIntPointerList.begin() == mIntPointerList.end());
			Assert::IsTrue(mFooList.begin() == mFooList.end());

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			mIntList.PushFront(randomIntData);
			mStringList.PushFront(randomString);
			mIntPointerList.PushFront(&randomIntData);
			mFooList.PushFront(randomFoo);

			Assert::IsTrue(intIt != mIntList.begin());
			Assert::IsTrue(randomIntData == *mIntList.begin());

			Assert::IsTrue(stringIt != mStringList.begin());
			Assert::IsTrue(randomString == *mStringList.begin());

			Assert::IsTrue(intPointerIt != mIntPointerList.begin());
			Assert::IsTrue(&randomIntData == *mIntPointerList.begin());

			Assert::IsTrue(fooIt != mFooList.begin());
			Assert::IsTrue(randomFoo == *mFooList.begin());
		}

		TEST_METHOD(SListend)
		{
			SList<std::int32_t>::Iterator intIt = mIntList.end();
			SList<std::string>::Iterator stringIt = mStringList.end();
			SList<std::int32_t*>::Iterator intPointerIt = mIntPointerList.end();
			SList<Foo>::Iterator fooIt = mFooList.end();

			Assert::IsTrue(intIt == SList<std::int32_t>::Iterator(mIntList, nullptr));
			Assert::IsTrue(stringIt == SList<std::string>::Iterator(mStringList, nullptr));
			Assert::IsTrue(intPointerIt == SList<std::int32_t*>::Iterator(mIntPointerList, nullptr));
			Assert::IsTrue(fooIt == SList<Foo>::Iterator(mFooList, nullptr));

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			mIntList.PushFront(randomIntData);
			mStringList.PushFront(randomString);
			mIntPointerList.PushFront(&randomIntData);
			mFooList.PushFront(randomFoo);

			Assert::IsTrue(intIt == mIntList.end());
			Assert::IsTrue(stringIt == mStringList.end());
			Assert::IsTrue(intPointerIt == mIntPointerList.end());
			Assert::IsTrue(fooIt == mFooList.end());
		}

		TEST_METHOD(SListMoveConstructor)
		{
			std::int32_t randomIntData = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi"), thirdRandomString = std::string("hello");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			mIntList.PushBack(randomIntData);
			mStringList.PushBack(randomString);
			mIntPointerList.PushBack(&randomIntData);
			mFooList.PushBack(randomFoo);

			mIntList.PushBack(nextRandomInt);
			mStringList.PushBack(nextRandomString);
			mIntPointerList.PushBack(&nextRandomInt);
			mFooList.PushBack(nextRandomFoo);

			mIntList.PushBack(thirdRandomInt);
			mStringList.PushBack(thirdRandomString);
			mIntPointerList.PushBack(&thirdRandomInt);
			mFooList.PushBack(thirdRandomFoo);

			SList<std::int32_t> otherIntList = std::move(mIntList);
			SList<std::string> otherStringList = std::move(mStringList);
			SList<std::int32_t*> otherIntPointerList = std::move(mIntPointerList);
			SList<Foo> otherFooList = std::move(mFooList);

			Assert::IsTrue(mIntList.IsEmpty());
			Assert::IsTrue(mStringList.IsEmpty());
			Assert::IsTrue(mIntPointerList.IsEmpty());
			Assert::IsTrue(mFooList.IsEmpty());

			Assert::IsFalse(otherIntList.IsEmpty());
			Assert::IsFalse(otherStringList.IsEmpty());
			Assert::IsFalse(otherIntPointerList.IsEmpty());
			Assert::IsFalse(otherFooList.IsEmpty());

			Assert::IsTrue(otherIntList.Front() == randomIntData);
			Assert::IsTrue(otherStringList.Front() == randomString);
			Assert::IsTrue(otherIntPointerList.Front() == &randomIntData);
			Assert::IsTrue(otherFooList.Front() == randomFoo);

			Assert::IsTrue(otherIntList.Back() == thirdRandomInt);
			Assert::IsTrue(otherStringList.Back() == thirdRandomString);
			Assert::IsTrue(otherIntPointerList.Back() == &thirdRandomInt);
			Assert::IsTrue(otherFooList.Back() == thirdRandomFoo);
		}

		TEST_METHOD(SListMoveAssignmentOperator)
		{
			std::int32_t randomIntData = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi"), thirdRandomString = std::string("hello");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			mIntList.PushFront(randomIntData);
			mStringList.PushFront(randomString);
			mIntPointerList.PushFront(&randomIntData);
			mFooList.PushFront(randomFoo);

			mIntList.PushFront(nextRandomInt);
			mStringList.PushFront(nextRandomString);
			mIntPointerList.PushFront(&nextRandomInt);
			mFooList.PushFront(nextRandomFoo);

			mIntList.PushFront(thirdRandomInt);
			mStringList.PushFront(thirdRandomString);
			mIntPointerList.PushFront(&thirdRandomInt);
			mFooList.PushFront(thirdRandomFoo);

			SList<std::int32_t> otherIntList;
			SList<std::string> otherStringList;
			SList<std::int32_t*> otherIntPointerList;
			SList<Foo> otherFooList;

			otherIntList = std::move(mIntList);
			otherStringList = std::move(mStringList);
			otherIntPointerList = std::move(mIntPointerList);
			otherFooList = std::move(mFooList);

			Assert::IsTrue(mIntList.IsEmpty());
			Assert::IsTrue(mStringList.IsEmpty());
			Assert::IsTrue(mIntPointerList.IsEmpty());
			Assert::IsTrue(mFooList.IsEmpty());

			Assert::IsFalse(otherIntList.IsEmpty());
			Assert::IsFalse(otherStringList.IsEmpty());
			Assert::IsFalse(otherIntPointerList.IsEmpty());
			Assert::IsFalse(otherFooList.IsEmpty());
		}

		TEST_METHOD(SListInsertAfter)
		{
			std::int32_t randomIntData = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi"), thirdRandomString = std::string("hello");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			std::uint32_t listSize = mIntList.Size();
			
			SList<std::int32_t>::Iterator intIt = mIntList.InsertAfter(randomIntData, mIntList.begin());
			SList<std::string>::Iterator stringIt = mStringList.InsertAfter(randomString, mStringList.begin());
			SList<std::int32_t*>::Iterator intPointerIt = mIntPointerList.InsertAfter(&randomIntData, mIntPointerList.begin());
			SList<Foo>::Iterator fooIt = mFooList.InsertAfter(randomFoo, mFooList.begin());

			Assert::AreNotEqual(listSize, mIntList.Size());
			Assert::AreEqual(randomIntData, mIntList.Front());
			Assert::AreEqual(randomIntData, mIntList.Back());
			Assert::AreEqual(randomIntData, *intIt);

			Assert::AreNotEqual(listSize, mStringList.Size());
			Assert::AreEqual(randomString, mStringList.Front());
			Assert::AreEqual(randomString, mStringList.Back());
			Assert::AreEqual(randomString, *stringIt);

			Assert::AreNotEqual(listSize, mIntPointerList.Size());
			Assert::AreEqual(&randomIntData, mIntPointerList.Front());
			Assert::AreEqual(&randomIntData, mIntPointerList.Back());
			Assert::AreEqual(&randomIntData, *intPointerIt);

			Assert::AreNotEqual(listSize, mFooList.Size());
			Assert::IsTrue(randomFoo == mFooList.Front());
			Assert::IsTrue(randomFoo == mFooList.Back());
			Assert::IsTrue(randomFoo == *fooIt);

			mIntList.PopFront();
			mStringList.PopFront();
			mIntPointerList.PopFront();
			mFooList.PopFront();

			mIntList.PushFront(randomIntData);
			mStringList.PushFront(randomString);
			mIntPointerList.PushFront(&randomIntData);
			mFooList.PushFront(randomFoo);

			listSize = mIntList.Size();

			mIntList.InsertAfter(nextRandomInt, mIntList.Find(randomIntData));
			mStringList.InsertAfter(nextRandomString, mStringList.Find(randomString));
			mIntPointerList.InsertAfter(&nextRandomInt, mIntPointerList.Find(&randomIntData));
			mFooList.InsertAfter(nextRandomFoo, mFooList.Find(randomFoo));

			Assert::AreNotEqual(listSize, mIntList.Size());
			Assert::AreEqual(randomIntData, mIntList.Front());
			Assert::AreEqual(nextRandomInt, mIntList.Back());

			Assert::AreNotEqual(listSize, mStringList.Size());
			Assert::AreEqual(randomString, mStringList.Front());
			Assert::AreEqual(nextRandomString, mStringList.Back());

			Assert::AreNotEqual(listSize, mIntPointerList.Size());
			Assert::AreEqual(&randomIntData, mIntPointerList.Front());
			Assert::AreEqual(&nextRandomInt, mIntPointerList.Back());

			Assert::AreNotEqual(listSize, mFooList.Size());
			Assert::IsTrue(randomFoo == mFooList.Front());
			Assert::IsTrue(nextRandomFoo == mFooList.Back());

			listSize = mIntList.Size();

			mIntList.InsertAfter(thirdRandomInt, mIntList.Find(randomIntData));
			mStringList.InsertAfter(thirdRandomString, mStringList.Find(randomString));
			mIntPointerList.InsertAfter(&thirdRandomInt, mIntPointerList.Find(&randomIntData));
			mFooList.InsertAfter(thirdRandomFoo, mFooList.Find(randomFoo));

			Assert::AreNotEqual(listSize, mIntList.Size());
			Assert::AreEqual(randomIntData, mIntList.Front());
			Assert::AreEqual(nextRandomInt, mIntList.Back());

			Assert::AreNotEqual(listSize, mStringList.Size());
			Assert::AreEqual(randomString, mStringList.Front());
			Assert::AreEqual(nextRandomString, mStringList.Back());

			Assert::AreNotEqual(listSize, mIntPointerList.Size());
			Assert::AreEqual(&randomIntData, mIntPointerList.Front());
			Assert::AreEqual(&nextRandomInt, mIntPointerList.Back());

			Assert::AreNotEqual(listSize, mFooList.Size());
			Assert::IsTrue(randomFoo == mFooList.Front());
			Assert::IsTrue(nextRandomFoo == mFooList.Back());

			SList<std::int32_t> onemIntList, othermIntList;
			SList<std::string> oneStringList, otherStringList;
			SList<std::int32_t*> oneIntPointerList, otherIntPointerList;
			SList<Foo> oneFooList, otherFooList;

			auto intInsertAfterPointer = [&onemIntList, &othermIntList, randomIntData] { onemIntList.InsertAfter(randomIntData, othermIntList.begin()); };
			auto stringInsertAfterPointer = [&oneStringList, &otherStringList, randomString] { oneStringList.InsertAfter(randomString, otherStringList.begin()); };
			auto intPointerInsertAfterPointer = [&oneIntPointerList, &otherIntPointerList, &randomIntData] { oneIntPointerList.InsertAfter(&randomIntData, otherIntPointerList.begin()); };
			auto fooInsertAfterPointer = [&oneFooList, &otherFooList, randomFoo] { oneFooList.InsertAfter(randomFoo, otherFooList.begin()); };

			Assert::ExpectException<std::exception>(intInsertAfterPointer);
			Assert::ExpectException<std::exception>(stringInsertAfterPointer);
			Assert::ExpectException<std::exception>(intPointerInsertAfterPointer);
			Assert::ExpectException<std::exception>(fooInsertAfterPointer);
		}

		TEST_METHOD(SListFind)
		{
			std::int32_t randomIntData = rand(), nextRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt);

			Assert::IsTrue(mIntList.Find(randomIntData) == mIntList.end());
			Assert::IsTrue(mStringList.Find(randomString) == mStringList.end());
			Assert::IsTrue(mIntPointerList.Find(&randomIntData) == mIntPointerList.end());
			Assert::IsTrue(mFooList.Find(randomFoo) == mFooList.end());

			mIntList.PushBack(randomIntData);
			mStringList.PushBack(randomString);
			mIntPointerList.PushBack(&randomIntData);
			mFooList.PushBack(randomFoo);

			Assert::IsTrue(mIntList.Find(nextRandomInt) == mIntList.end());
			Assert::IsTrue(mIntList.Find(randomIntData) == mIntList.begin());

			Assert::IsTrue(mStringList.Find(nextRandomString) == mStringList.end());
			Assert::IsTrue(mStringList.Find(randomString) == mStringList.begin());

			Assert::IsTrue(mIntPointerList.Find(&nextRandomInt) == mIntPointerList.end());
			Assert::IsTrue(mIntPointerList.Find(&randomIntData) == mIntPointerList.begin());

			Assert::IsTrue(mFooList.Find(nextRandomFoo) == mFooList.end());
			Assert::IsTrue(mFooList.Find(randomFoo) == mFooList.begin());
		}

		TEST_METHOD(SListRemove)
		{
			for (uint32_t i = 0; i < numberOfTestItems; ++i)
			{
				std::int32_t randomIntData = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
				std::string randomString = std::string("howdy"), nextRandomString = std::string("hi"), thirdRandomString = std::string("hello");
				Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

				mIntList.PushFront(randomIntData);
				mIntList.PushFront(nextRandomInt);
				mStringList.PushFront(randomString);
				mStringList.PushFront(nextRandomString);
				mIntPointerList.PushFront(&randomIntData);
				mIntPointerList.PushFront(&nextRandomInt);
				mFooList.PushFront(randomFoo);
				mFooList.PushFront(nextRandomFoo);

				std::uint32_t listSize = mIntList.Size();

				mIntList.Remove(thirdRandomInt);
				mStringList.Remove(thirdRandomString);
				mIntPointerList.Remove(&thirdRandomInt);
				mFooList.Remove(thirdRandomFoo);

				Assert::AreEqual(listSize, mIntList.Size());
				Assert::AreEqual(randomIntData, mIntList.Back());
				Assert::AreEqual(nextRandomInt, mIntList.Front());

				Assert::AreEqual(listSize, mStringList.Size());
				Assert::AreEqual(randomString, mStringList.Back());
				Assert::AreEqual(nextRandomString, mStringList.Front());

				Assert::AreEqual(listSize, mIntPointerList.Size());
				Assert::AreEqual(&randomIntData, mIntPointerList.Back());
				Assert::AreEqual(&nextRandomInt, mIntPointerList.Front());

				Assert::AreEqual(listSize, mFooList.Size());
				Assert::IsTrue(randomFoo == mFooList.Back());
				Assert::IsTrue(nextRandomFoo == mFooList.Front());

				mIntList.PushFront(thirdRandomInt);
				mStringList.PushFront(thirdRandomString);
				mIntPointerList.PushFront(&thirdRandomInt);
				mFooList.PushFront(thirdRandomFoo);

				listSize = mIntList.Size();

				mIntList.Remove(nextRandomInt);
				mStringList.Remove(nextRandomString);
				mIntPointerList.Remove(&nextRandomInt);
				mFooList.Remove(nextRandomFoo);

				Assert::AreEqual(listSize - 1, mIntList.Size());
				Assert::AreEqual(randomIntData, mIntList.Back());
				Assert::AreEqual(thirdRandomInt, mIntList.Front());

				Assert::AreEqual(listSize - 1, mStringList.Size());
				Assert::AreEqual(randomString, mStringList.Back());
				Assert::AreEqual(thirdRandomString, mStringList.Front());

				Assert::AreEqual(listSize - 1, mIntPointerList.Size());
				Assert::AreEqual(&randomIntData, mIntPointerList.Back());
				Assert::AreEqual(&thirdRandomInt, mIntPointerList.Front());

				Assert::AreEqual(listSize - 1, mFooList.Size());
				Assert::IsTrue(randomFoo == mFooList.Back());
				Assert::IsTrue(thirdRandomFoo == mFooList.Front());

				mIntList.PushFront(nextRandomInt);
				mStringList.PushFront(nextRandomString);
				mIntPointerList.PushFront(&nextRandomInt);
				mFooList.PushFront(nextRandomFoo);

				listSize = mIntList.Size();

				mIntList.Remove(nextRandomInt);
				mStringList.Remove(nextRandomString);
				mIntPointerList.Remove(&nextRandomInt);
				mFooList.Remove(nextRandomFoo);

				Assert::AreEqual(listSize - 1, mIntList.Size());
				Assert::AreEqual(randomIntData, mIntList.Back());
				Assert::AreEqual(thirdRandomInt, mIntList.Front());

				Assert::AreEqual(listSize - 1, mStringList.Size());
				Assert::AreEqual(randomString, mStringList.Back());
				Assert::AreEqual(thirdRandomString, mStringList.Front());

				Assert::AreEqual(listSize - 1, mIntPointerList.Size());
				Assert::AreEqual(&randomIntData, mIntPointerList.Back());
				Assert::AreEqual(&thirdRandomInt, mIntPointerList.Front());

				Assert::AreEqual(listSize - 1, mFooList.Size());
				Assert::IsTrue(randomFoo == mFooList.Back());
				Assert::IsTrue(thirdRandomFoo == mFooList.Front());

				mIntList.PushFront(nextRandomInt);
				mStringList.PushFront(nextRandomString);
				mIntPointerList.PushFront(&nextRandomInt);
				mFooList.PushFront(nextRandomFoo);

				listSize = mIntList.Size();

				mIntList.Remove(randomIntData);
				mStringList.Remove(randomString);
				mIntPointerList.Remove(&randomIntData);
				mFooList.Remove(randomFoo);

				Assert::AreEqual(listSize - 1, mIntList.Size());
				Assert::AreEqual(thirdRandomInt, mIntList.Back());
				Assert::AreEqual(nextRandomInt, mIntList.Front());

				Assert::AreEqual(listSize - 1, mStringList.Size());
				Assert::AreEqual(thirdRandomString, mStringList.Back());
				Assert::AreEqual(nextRandomString, mStringList.Front());

				Assert::AreEqual(listSize - 1, mIntPointerList.Size());
				Assert::AreEqual(&thirdRandomInt, mIntPointerList.Back());
				Assert::AreEqual(&nextRandomInt, mIntPointerList.Front());

				Assert::AreEqual(listSize - 1, mFooList.Size());
				Assert::IsTrue(thirdRandomFoo == mFooList.Back());
				Assert::IsTrue(nextRandomFoo == mFooList.Front());

				mIntList.Remove(randomIntData);
				mIntList.Remove(nextRandomInt);
				mIntList.Remove(thirdRandomInt);
				mStringList.Remove(randomString);
				mStringList.Remove(nextRandomString);
				mStringList.Remove(thirdRandomString);
				mIntPointerList.Remove(&randomIntData);
				mIntPointerList.Remove(&nextRandomInt);
				mIntPointerList.Remove(&thirdRandomInt);
				mFooList.Remove(randomFoo);
				mFooList.Remove(nextRandomFoo);
				mFooList.Remove(thirdRandomFoo);
			}
		}

		TEST_METHOD(SListIteratorDefaultConstructor)
		{
			SList<std::int32_t>::Iterator intIt;
			SList<std::string>::Iterator stringIt;
			SList<std::int32_t*>::Iterator intPointerIt;
			SList<Foo>::Iterator fooIt;

			SList<std::int32_t>::Iterator otherIntIt;
			SList<std::string>::Iterator otherStringIt;
			SList<std::int32_t*>::Iterator otherIntPointerIt;
			SList<Foo>::Iterator otherFooIt;

			Assert::IsTrue(intIt == otherIntIt);
			Assert::IsTrue(stringIt == otherStringIt);
			Assert::IsTrue(intPointerIt == otherIntPointerIt);
			Assert::IsTrue(fooIt == otherFooIt);
		}

		TEST_METHOD(SListIteratorDestructor)
		{
			SList<std::int32_t>::Iterator* intIt = new SList<std::int32_t>::Iterator();
			SList<std::string>::Iterator* stringIt = new SList<std::string>::Iterator();
			SList<std::int32_t*>::Iterator* intPointerIt = new SList<std::int32_t*>::Iterator();
			SList<Foo>::Iterator* fooIt = new SList<Foo>::Iterator();

			delete(intIt);
			delete(stringIt);
			delete(intPointerIt);
			delete(fooIt);
		}

		TEST_METHOD(SListIteratorPostIncrement)
		{
			std::int32_t randomIntData = rand(), nextRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt);

			mIntList.PushFront(randomIntData);
			mStringList.PushFront(randomString);
			mIntPointerList.PushFront(&randomIntData);
			mFooList.PushFront(randomFoo);

			mIntList.PushFront(nextRandomInt);
			mStringList.PushFront(nextRandomString);
			mIntPointerList.PushFront(&nextRandomInt);
			mFooList.PushFront(nextRandomFoo);

			SList<std::int32_t>::Iterator intIt = mIntList.begin();
			SList<std::string>::Iterator stringIt = mStringList.begin();
			SList<std::int32_t*>::Iterator intPointerIt = mIntPointerList.begin();
			SList<Foo>::Iterator fooIt = mFooList.begin();

			intIt++;
			stringIt++;
			intPointerIt++;
			fooIt++;

			Assert::IsTrue(intIt != mIntList.end());
			Assert::IsTrue(stringIt != mStringList.end());
			Assert::IsTrue(intPointerIt != mIntPointerList.end());
			Assert::IsTrue(fooIt != mFooList.end());

			SList<std::int32_t>::Iterator otherIntIt = intIt++;
			SList<std::string>::Iterator otherStringIt = stringIt++;
			SList<std::int32_t*>::Iterator otherIntPointerIt = intPointerIt++;
			SList<Foo>::Iterator otherFooIt = fooIt++;

			Assert::IsTrue(intIt != otherIntIt);
			Assert::IsTrue(stringIt != otherStringIt);
			Assert::IsTrue(intPointerIt != otherIntPointerIt);
			Assert::IsTrue(fooIt != otherFooIt);

			intIt = SList<std::int32_t>::Iterator();
			stringIt = SList<std::string>::Iterator();
			intPointerIt = SList<std::int32_t*>::Iterator();
			fooIt = SList<Foo>::Iterator();

			auto intItIncrement = [&intIt] { intIt++; };
			auto stringItIncrement = [&stringIt] { stringIt++; };
			auto intPointerItIncrement = [&intPointerIt] { intPointerIt++; };
			auto fooItIncrement = [&fooIt] { fooIt++; };

			Assert::ExpectException<std::exception>(intItIncrement);
			Assert::ExpectException<std::exception>(stringItIncrement);
			Assert::ExpectException<std::exception>(intPointerItIncrement);
			Assert::ExpectException<std::exception>(fooItIncrement);
		}

		TEST_METHOD(SListIteratorPreIncrement)
		{
			std::int32_t randomIntData = rand(), nextRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt);

			mIntList.PushFront(randomIntData);
			mStringList.PushFront(randomString);
			mIntPointerList.PushFront(&randomIntData);
			mFooList.PushFront(randomFoo);

			mIntList.PushFront(nextRandomInt);
			mStringList.PushFront(nextRandomString);
			mIntPointerList.PushFront(&nextRandomInt);
			mFooList.PushFront(nextRandomFoo);

			SList<std::int32_t>::Iterator intIt = mIntList.begin();
			SList<std::string>::Iterator stringIt = mStringList.begin();
			SList<std::int32_t*>::Iterator intPointerIt = mIntPointerList.begin();
			SList<Foo>::Iterator fooIt = mFooList.begin();

			++intIt;
			++stringIt;
			++intPointerIt;
			++fooIt;

			Assert::IsTrue(intIt != mIntList.end());
			Assert::IsTrue(stringIt != mStringList.end());
			Assert::IsTrue(intPointerIt != mIntPointerList.end());
			Assert::IsTrue(fooIt != mFooList.end());

			SList<std::int32_t>::Iterator otherIntIt = ++intIt;
			SList<std::string>::Iterator otherStringIt = ++stringIt;
			SList<std::int32_t*>::Iterator otherIntPointerIt = ++intPointerIt;
			SList<Foo>::Iterator otherFooIt = ++fooIt;

			Assert::IsTrue(intIt == mIntList.end());
			Assert::IsTrue(stringIt == mStringList.end());
			Assert::IsTrue(intPointerIt == mIntPointerList.end());
			Assert::IsTrue(fooIt == mFooList.end());

			Assert::IsTrue(intIt == otherIntIt);
			Assert::IsTrue(stringIt == otherStringIt);
			Assert::IsTrue(intPointerIt == otherIntPointerIt);
			Assert::IsTrue(fooIt == otherFooIt);

			intIt = SList<std::int32_t>::Iterator();
			stringIt = SList<std::string>::Iterator();
			intPointerIt = SList<std::int32_t*>::Iterator();
			fooIt = SList<Foo>::Iterator();

			auto intItIncrement = [&intIt] { ++intIt; };
			auto stringItIncrement = [&stringIt] { ++stringIt; };
			auto intPointerItIncrement = [&intPointerIt] { ++intPointerIt; };
			auto fooItIncrement = [&fooIt] { ++fooIt; };

			Assert::ExpectException<std::exception>(intItIncrement);
			Assert::ExpectException<std::exception>(stringItIncrement);
			Assert::ExpectException<std::exception>(intPointerItIncrement);
			Assert::ExpectException<std::exception>(fooItIncrement);
		}

		TEST_METHOD(SListIteratorDereference)
		{
			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			mIntList.PushFront(randomIntData);
			mStringList.PushFront(randomString);
			mIntPointerList.PushFront(&randomIntData);
			mFooList.PushFront(randomFoo);

			Assert::AreEqual(*mIntList.begin(), randomIntData);
			Assert::AreEqual(*mStringList.begin(), randomString);
			Assert::AreEqual(*mIntPointerList.begin(), &randomIntData);
			Assert::IsTrue(*mFooList.begin() == randomFoo);

			SList<std::int32_t>* mIntListPointer = &mIntList;
			SList<std::string>* mStringListPointer = &mStringList;
			SList<std::int32_t*>* mIntPointerListPointer = &mIntPointerList;
			SList<Foo>* mFooListPointer = &mFooList;

			auto intDereference = [mIntListPointer] { *(mIntListPointer->end()); };
			auto stringDereference = [mStringListPointer] { *(mStringListPointer->end()); };
			auto intPointerDereference = [mIntPointerListPointer] { *(mIntPointerListPointer->end()); };
			auto fooDereference = [mFooListPointer] { *(mFooListPointer->end()); };

			Assert::ExpectException<std::exception>(intDereference);
			Assert::ExpectException<std::exception>(stringDereference);
			Assert::ExpectException<std::exception>(intPointerDereference);
			Assert::ExpectException<std::exception>(fooDereference);

			SList<std::int32_t>::Iterator intIt;
			SList<std::string>::Iterator stringIt;
			SList<std::int32_t*>::Iterator intPointerIt;
			SList<Foo>::Iterator fooIt;

			auto intItDereference = [&intIt] { *(intIt); };
			auto stringItDereference = [&stringIt] { *(stringIt); };
			auto intPointerItDereference = [&intPointerIt] { *(intPointerIt); };
			auto fooItDereference = [&fooIt] { *(fooIt); };

			Assert::ExpectException<std::exception>(intItDereference);
			Assert::ExpectException<std::exception>(stringItDereference);
			Assert::ExpectException<std::exception>(intPointerItDereference);
			Assert::ExpectException<std::exception>(fooItDereference);

			const SList<std::int32_t>::Iterator constIntIt;
			const SList<std::string>::Iterator constStringIt;
			const SList<std::int32_t*>::Iterator constIntPointerIt;
			const SList<Foo>::Iterator constFooIt;

			auto constIntItDereference = [&constIntIt] { *(constIntIt); };
			auto constStringItDereference = [&constStringIt] { *(constStringIt); };
			auto constIntPointerItDereference = [&constIntPointerIt] { *(constIntPointerIt); };
			auto constFooItDereference = [&constFooIt] { *(constFooIt); };

			Assert::ExpectException<std::exception>(constIntItDereference);
			Assert::ExpectException<std::exception>(constStringItDereference);
			Assert::ExpectException<std::exception>(constIntPointerItDereference);
			Assert::ExpectException<std::exception>(constFooItDereference);
		}

		TEST_METHOD(SListIteratorIsEqual)
		{
			SList<std::int32_t>::Iterator* intIt = new SList<std::int32_t>::Iterator();
			SList<std::string>::Iterator* stringIt = new SList<std::string>::Iterator();
			SList<std::int32_t*>::Iterator* intPointerIt = new SList<std::int32_t*>::Iterator();
			SList<Foo>::Iterator* fooIt = new SList<Foo>::Iterator();

			SList<std::int32_t>::Iterator* otherIntIt = new SList<std::int32_t>::Iterator();
			SList<std::string>::Iterator* otherStringIt = new SList<std::string>::Iterator();
			SList<std::int32_t*>::Iterator* otherIntPointerIt = new SList<std::int32_t*>::Iterator();
			SList<Foo>::Iterator* otherFooIt = new SList<Foo>::Iterator();

			Assert::IsTrue(*intIt == *otherIntIt);
			Assert::IsTrue(*stringIt == *otherStringIt);
			Assert::IsTrue(*intPointerIt == *otherIntPointerIt);
			Assert::IsTrue(*fooIt == *otherFooIt);

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			mIntList.PushFront(randomIntData);
			mStringList.PushFront(randomString);
			mIntPointerList.PushFront(&randomIntData);
			mFooList.PushFront(randomFoo);

			Assert::IsFalse(*intIt == mIntList.begin());
			Assert::IsTrue(mIntList.begin() == mIntList.begin());

			Assert::IsFalse(*stringIt == mStringList.begin());
			Assert::IsTrue(mStringList.begin() == mStringList.begin());

			Assert::IsFalse(*intPointerIt == mIntPointerList.begin());
			Assert::IsTrue(mIntPointerList.begin() == mIntPointerList.begin());

			Assert::IsFalse(*fooIt == mFooList.begin());
			Assert::IsTrue(mFooList.begin() == mFooList.begin());

			delete(intIt);
			delete(stringIt);
			delete(intPointerIt);
			delete(fooIt);
			delete(otherIntIt);
			delete(otherStringIt);
			delete(otherIntPointerIt);
			delete(otherFooIt);
		}

		TEST_METHOD(SListIteratorNotEqual)
		{
			SList<std::int32_t>::Iterator intIt;
			SList<std::string>::Iterator stringIt;
			SList<std::int32_t*>::Iterator intPointerIt;
			SList<Foo>::Iterator fooIt;

			Assert::IsTrue(intIt != mIntList.begin());
			Assert::IsTrue(stringIt != mStringList.begin());
			Assert::IsTrue(intPointerIt != mIntPointerList.begin());
			Assert::IsTrue(fooIt != mFooList.begin());

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			mIntList.PushFront(randomIntData);
			mStringList.PushFront(randomString);
			mIntPointerList.PushFront(&randomIntData);
			mFooList.PushFront(randomFoo);

			Assert::IsTrue(intIt != mIntList.begin());
			Assert::IsFalse(mIntList.begin() != mIntList.begin());

			Assert::IsTrue(stringIt != mStringList.begin());
			Assert::IsFalse(mStringList.begin() != mStringList.begin());

			Assert::IsTrue(intPointerIt != mIntPointerList.begin());
			Assert::IsFalse(mIntPointerList.begin() != mIntPointerList.begin());

			Assert::IsTrue(fooIt != mFooList.begin());
			Assert::IsFalse(mFooList.begin() != mFooList.begin());
		}

	private:
		static _CrtMemState sStartMemState;

		static const std::uint32_t numberOfTestItems = 5;

		SList<std::int32_t> mIntList;
		SList<std::string> mStringList;
		SList<std::int32_t*> mIntPointerList;
		SList<Foo> mFooList;
	};

	_CrtMemState SListTest::sStartMemState;
}