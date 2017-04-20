#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(VectorTest)
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

		class TestResizeStrategy : public ResizeStrategy
		{

		public:

			virtual std::uint32_t operator()(std::uint32_t, std::uint32_t) const
			{
				return 2;
			}

			virtual ResizeStrategy* clone() const
			{
				return new TestResizeStrategy(*this);
			}
		};

		class InvalidResizeStrategy : public ResizeStrategy
		{

		public:

			virtual std::uint32_t operator()(std::uint32_t, std::uint32_t) const
			{
				return 0;
			}

			virtual ResizeStrategy* clone() const
			{
				return new InvalidResizeStrategy(*this);
			}
		};

		TEST_METHOD(VectorDefaultConstructor)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			Assert::AreEqual(Vector<std::int32_t>::defaultCapacity, intVector.Capacity());
			Assert::IsTrue(intVector.IsEmpty());

			Assert::AreEqual(Vector<std::string>::defaultCapacity, stringVector.Capacity());
			Assert::IsTrue(stringVector.IsEmpty());

			Assert::AreEqual(Vector<std::int32_t*>::defaultCapacity, pointerVector.Capacity());
			Assert::IsTrue(pointerVector.IsEmpty());

			Assert::AreEqual(Vector<Foo>::defaultCapacity, fooVector.Capacity());
			Assert::IsTrue(fooVector.IsEmpty());
		}

		TEST_METHOD(VectorAccessor)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::int32_t randomIntData = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi"), thirdRandomString = std::string("hello");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			intVector.PushBack(randomIntData);
			intVector.PushBack(nextRandomInt);
			intVector.PushBack(thirdRandomInt);

			stringVector.PushBack(randomString);
			stringVector.PushBack(nextRandomString);
			stringVector.PushBack(thirdRandomString);

			pointerVector.PushBack(&randomIntData);
			pointerVector.PushBack(&nextRandomInt);
			pointerVector.PushBack(&thirdRandomInt);

			fooVector.PushBack(randomFoo);
			fooVector.PushBack(nextRandomFoo);
			fooVector.PushBack(thirdRandomFoo);

			Assert::AreEqual(randomIntData, intVector[0]);
			Assert::AreEqual(nextRandomInt, intVector[1]);
			Assert::AreEqual(thirdRandomInt, intVector[2]);

			Assert::AreEqual(randomString, stringVector[0]);
			Assert::AreEqual(nextRandomString, stringVector[1]);
			Assert::AreEqual(thirdRandomString, stringVector[2]);

			Assert::AreEqual(&randomIntData, pointerVector[0]);
			Assert::AreEqual(&nextRandomInt, pointerVector[1]);
			Assert::AreEqual(&thirdRandomInt, pointerVector[2]);

			Assert::IsTrue(randomFoo == fooVector[0]);
			Assert::IsTrue(nextRandomFoo == fooVector[1]);
			Assert::IsTrue(thirdRandomFoo == fooVector[2]);

			std::uint32_t vectorCapacity = intVector.Capacity();

			auto intAccessorPointer = [&intVector, vectorCapacity] { intVector.operator[](vectorCapacity); };
			auto stringAccessorPointer = [&stringVector, vectorCapacity] { stringVector.operator[](vectorCapacity); };
			auto pointerAccessorPointer = [&pointerVector, vectorCapacity] { pointerVector.operator[](vectorCapacity); };
			auto fooAccessorPointer = [&fooVector, vectorCapacity] { fooVector.operator[](vectorCapacity); };

			Assert::ExpectException<std::exception>(intAccessorPointer);
			Assert::ExpectException<std::exception>(stringAccessorPointer);
			Assert::ExpectException<std::exception>(pointerAccessorPointer);
			Assert::ExpectException<std::exception>(fooAccessorPointer);

			const Vector<std::int32_t> constIntVector;
			const Vector<std::string> constStringVector;
			const Vector<std::int32_t*> constPointerVector;
			const Vector<Foo> constFooVector;

			auto constIntAccessorPointer = [&constIntVector, vectorCapacity] { constIntVector.operator[](vectorCapacity); };
			auto constStringAccessorPointer = [&constStringVector, vectorCapacity] { constStringVector.operator[](vectorCapacity); };
			auto constPointerAccessorPointer = [&constPointerVector, vectorCapacity] { constPointerVector.operator[](vectorCapacity); };
			auto constFooAccessorPointer = [&constFooVector, vectorCapacity] { constFooVector.operator[](vectorCapacity); };

			Assert::ExpectException<std::exception>(constIntAccessorPointer);
			Assert::ExpectException<std::exception>(constStringAccessorPointer);
			Assert::ExpectException<std::exception>(constPointerAccessorPointer);
			Assert::ExpectException<std::exception>(constFooAccessorPointer);
		}

		TEST_METHOD(VectorPopBack)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::int32_t randomIntData = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi"), thirdRandomString = std::string("hello");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			std::uint32_t vectorSize = intVector.Size();

			intVector.PopBack();
			stringVector.PopBack();
			pointerVector.PopBack();
			fooVector.PopBack();

			Assert::AreEqual(vectorSize, intVector.Size());
			Assert::AreEqual(vectorSize, stringVector.Size());
			Assert::AreEqual(vectorSize, pointerVector.Size());
			Assert::AreEqual(vectorSize, fooVector.Size());

			intVector.PushBack(randomIntData);
			stringVector.PushBack(randomString);
			pointerVector.PushBack(&randomIntData);
			fooVector.PushBack(randomFoo);

			vectorSize = intVector.Size();

			intVector.PopBack();
			stringVector.PopBack();
			pointerVector.PopBack();
			fooVector.PopBack();

			Assert::AreEqual(vectorSize - 1, intVector.Size());
			Assert::IsTrue(intVector.end() == intVector.Find(randomIntData));

			Assert::AreEqual(vectorSize - 1, stringVector.Size());
			Assert::IsTrue(stringVector.end() == stringVector.Find(randomString));

			Assert::AreEqual(vectorSize - 1, pointerVector.Size());
			Assert::IsTrue(pointerVector.end() == pointerVector.Find(&randomIntData));

			Assert::AreEqual(vectorSize - 1, fooVector.Size());
			Assert::IsTrue(fooVector.end() == fooVector.Find(randomFoo));

			intVector.PushBack(nextRandomInt);
			intVector.PushBack(thirdRandomInt);

			stringVector.PushBack(nextRandomString);
			stringVector.PushBack(thirdRandomString);

			pointerVector.PushBack(&nextRandomInt);
			pointerVector.PushBack(&thirdRandomInt);

			fooVector.PushBack(nextRandomFoo);
			fooVector.PushBack(thirdRandomFoo);

			vectorSize = intVector.Size();

			intVector.PopBack();
			stringVector.PopBack();
			pointerVector.PopBack();
			fooVector.PopBack();

			Assert::AreEqual(vectorSize - 1, intVector.Size());
			Assert::IsTrue(intVector.end() == intVector.Find(thirdRandomInt));
			Assert::IsTrue(intVector.begin() == intVector.Find(nextRandomInt));

			Assert::AreEqual(vectorSize - 1, stringVector.Size());
			Assert::IsTrue(stringVector.end() == stringVector.Find(thirdRandomString));
			Assert::IsTrue(stringVector.begin() == stringVector.Find(nextRandomString));

			Assert::AreEqual(vectorSize - 1, pointerVector.Size());
			Assert::IsTrue(pointerVector.end() == pointerVector.Find(&thirdRandomInt));
			Assert::IsTrue(pointerVector.begin() == pointerVector.Find(&nextRandomInt));

			Assert::AreEqual(vectorSize - 1, fooVector.Size());
			Assert::IsTrue(fooVector.end() == fooVector.Find(thirdRandomFoo));
			Assert::IsTrue(fooVector.begin() == fooVector.Find(nextRandomFoo));
		}

		TEST_METHOD(VectorIsEmpty)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			Assert::IsTrue(intVector.IsEmpty());
			Assert::IsTrue(stringVector.IsEmpty());
			Assert::IsTrue(pointerVector.IsEmpty());
			Assert::IsTrue(fooVector.IsEmpty());

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			intVector.PushBack(randomIntData);
			stringVector.PushBack(randomString);
			pointerVector.PushBack(&randomIntData);
			fooVector.PushBack(randomFoo);

			Assert::IsFalse(intVector.IsEmpty());
			Assert::IsFalse(stringVector.IsEmpty());
			Assert::IsFalse(pointerVector.IsEmpty());
			Assert::IsFalse(fooVector.IsEmpty());
		}

		TEST_METHOD(VectorFront)
		{
			std::int32_t randomIntData = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi"), thirdRandomString = std::string("hello");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			auto intFrontPointer = [&intVector] { intVector.Front(); };
			auto stringFrontPointer = [&stringVector] { stringVector.Front(); };
			auto pointerFrontPointer = [&pointerVector] { pointerVector.Front(); };
			auto fooFrontPointer = [&fooVector] { fooVector.Front(); };

			Assert::ExpectException<std::exception>(intFrontPointer);
			Assert::ExpectException<std::exception>(stringFrontPointer);
			Assert::ExpectException<std::exception>(pointerFrontPointer);
			Assert::ExpectException<std::exception>(fooFrontPointer);

			intVector.PushBack(randomIntData);
			stringVector.PushBack(randomString);
			pointerVector.PushBack(&randomIntData);
			fooVector.PushBack(randomFoo);

			Assert::AreEqual(randomIntData, intVector.Front());
			Assert::AreEqual(randomString, stringVector.Front());
			Assert::AreEqual(&randomIntData, pointerVector.Front());
			Assert::IsTrue(randomFoo == fooVector.Front());

			intVector.PushBack(nextRandomInt);
			stringVector.PushBack(nextRandomString);
			pointerVector.PushBack(&nextRandomInt);
			fooVector.PushBack(nextRandomFoo);

			Assert::AreEqual(randomIntData, intVector.Front());
			Assert::AreEqual(randomString, stringVector.Front());
			Assert::AreEqual(&randomIntData, pointerVector.Front());
			Assert::IsTrue(randomFoo == fooVector.Front());

			intVector.PushBack(thirdRandomInt);
			stringVector.PushBack(thirdRandomString);
			pointerVector.PushBack(&thirdRandomInt);
			fooVector.PushBack(thirdRandomFoo);

			Assert::AreEqual(randomIntData, intVector.Front());
			Assert::AreEqual(randomString, stringVector.Front());
			Assert::AreEqual(&randomIntData, pointerVector.Front());
			Assert::IsTrue(randomFoo == fooVector.Front());

			const Vector<std::int32_t> constIntVector;
			const Vector<std::string> constStringVector;
			const Vector<std::int32_t*> constPointerVector;
			const Vector<Foo> constFooVector;

			auto constIntFrontPointer = [&constIntVector] { constIntVector.Front(); };
			auto constStringFrontPointer = [&constStringVector] { constStringVector.Front(); };
			auto constPointerFrontPointer = [&constPointerVector] { constPointerVector.Front(); };
			auto constFooFrontPointer = [&constFooVector] { constFooVector.Front(); };

			Assert::ExpectException<std::exception>(constIntFrontPointer);
			Assert::ExpectException<std::exception>(constStringFrontPointer);
			Assert::ExpectException<std::exception>(constPointerFrontPointer);
			Assert::ExpectException<std::exception>(constFooFrontPointer);
		}

		TEST_METHOD(VectorBack)
		{
			std::int32_t randomIntData = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi"), thirdRandomString = std::string("hello");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			auto intBackPointer = [&intVector] { intVector.Back(); };
			auto stringBackPointer = [&stringVector] { stringVector.Back(); };
			auto pointerBackPointer = [&pointerVector] { pointerVector.Back(); };
			auto fooBackPointer = [&fooVector] { fooVector.Back(); };

			Assert::ExpectException<std::exception>(intBackPointer);
			Assert::ExpectException<std::exception>(stringBackPointer);
			Assert::ExpectException<std::exception>(pointerBackPointer);
			Assert::ExpectException<std::exception>(fooBackPointer);

			intVector.PushBack(randomIntData);
			stringVector.PushBack(randomString);
			pointerVector.PushBack(&randomIntData);
			fooVector.PushBack(randomFoo);

			Assert::AreEqual(randomIntData, intVector.Back());
			Assert::AreEqual(randomString, stringVector.Back());
			Assert::AreEqual(&randomIntData, pointerVector.Back());
			Assert::IsTrue(randomFoo == intVector.Back());
			
			intVector.PushBack(nextRandomInt);
			stringVector.PushBack(nextRandomString);
			pointerVector.PushBack(&nextRandomInt);
			fooVector.PushBack(nextRandomFoo);

			Assert::AreEqual(nextRandomInt, intVector.Back());
			Assert::AreEqual(nextRandomString, stringVector.Back());
			Assert::AreEqual(&nextRandomInt, pointerVector.Back());
			Assert::IsTrue(nextRandomFoo == fooVector.Back());

			intVector.PushBack(thirdRandomInt);
			stringVector.PushBack(thirdRandomString);
			pointerVector.PushBack(&thirdRandomInt);
			fooVector.PushBack(thirdRandomFoo);

			Assert::AreEqual(thirdRandomInt, intVector.Back());
			Assert::AreEqual(thirdRandomString, stringVector.Back());
			Assert::AreEqual(&thirdRandomInt, pointerVector.Back());
			Assert::IsTrue(thirdRandomFoo == fooVector.Back());

			const Vector<std::int32_t> constIntVector;
			const Vector<std::string> constStringVector;
			const Vector<std::int32_t*> constPointerVector;
			const Vector<Foo> constFooVector;

			auto constIntBackPointer = [&constIntVector] { constIntVector.Back(); };
			auto constStringBackPointer = [&constStringVector] { constStringVector.Back(); };
			auto constPointerBackPointer = [&constPointerVector] { constPointerVector.Back(); };
			auto constFooBackPointer = [&constFooVector] { constFooVector.Back(); };

			Assert::ExpectException<std::exception>(constIntBackPointer);
			Assert::ExpectException<std::exception>(constStringBackPointer);
			Assert::ExpectException<std::exception>(constPointerBackPointer);
			Assert::ExpectException<std::exception>(constFooBackPointer);
		}

		TEST_METHOD(VectorSize)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::uint32_t size = 0;

			Assert::AreEqual(size, intVector.Size());
			Assert::AreEqual(size, stringVector.Size());
			Assert::AreEqual(size, pointerVector.Size());
			Assert::AreEqual(size, fooVector.Size());

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			intVector.PushBack(randomIntData);
			stringVector.PushBack(randomString);
			pointerVector.PushBack(&randomIntData);
			fooVector.PushBack(randomFoo);

			++size;

			Assert::AreEqual(size, intVector.Size());
			Assert::AreEqual(size, stringVector.Size());
			Assert::AreEqual(size, pointerVector.Size());
			Assert::AreEqual(size, fooVector.Size());

			intVector.PopBack();
			stringVector.PopBack();
			pointerVector.PopBack();
			fooVector.PopBack();
			
			--size;

			Assert::AreEqual(size, intVector.Size());
			Assert::AreEqual(size, stringVector.Size());
			Assert::AreEqual(size, pointerVector.Size());
			Assert::AreEqual(size, fooVector.Size());
		}

		TEST_METHOD(VectorCapacity)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::uint32_t capacity = Vector<std::uint32_t>::defaultCapacity;

			Assert::AreEqual(capacity, intVector.Capacity());
			Assert::AreEqual(capacity, stringVector.Capacity());
			Assert::AreEqual(capacity, pointerVector.Capacity());
			Assert::AreEqual(capacity, fooVector.Capacity());

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			for (std::uint32_t inserts = 0; inserts <= capacity; ++inserts)
			{
				intVector.PushBack(randomIntData);
				stringVector.PushBack(randomString);
				pointerVector.PushBack(&randomIntData);
				fooVector.PushBack(randomFoo);
			}

			Assert::AreNotEqual(capacity, intVector.Capacity());
			Assert::AreNotEqual(capacity, stringVector.Capacity());
			Assert::AreNotEqual(capacity, pointerVector.Capacity());
			Assert::AreNotEqual(capacity, fooVector.Capacity());
		}

		TEST_METHOD(Vectorbegin)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::int32_t randomIntData = rand(), nextRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt);

			intVector.PushBack(randomIntData);
			stringVector.PushBack(randomString);
			pointerVector.PushBack(&randomIntData);
			fooVector.PushBack(randomFoo);

			Assert::IsTrue(*intVector.begin() == randomIntData);
			Assert::IsTrue(*stringVector.begin() == randomString);
			Assert::IsTrue(*pointerVector.begin() == &randomIntData);
			Assert::IsTrue(*fooVector.begin() == randomFoo);

			intVector.PushBack(nextRandomInt);
			stringVector.PushBack(nextRandomString);
			pointerVector.PushBack(&nextRandomInt);
			fooVector.PushBack(nextRandomFoo);

			Assert::IsTrue(*intVector.begin() == randomIntData);
			Assert::IsTrue(*stringVector.begin() == randomString);
			Assert::IsTrue(*pointerVector.begin() == &randomIntData);
			Assert::IsTrue(*fooVector.begin() == randomFoo);
		}

		TEST_METHOD(Vectorend)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			Assert::IsTrue(intVector.begin() == intVector.end());
			Assert::IsTrue(stringVector.begin() == stringVector.end());
			Assert::IsTrue(pointerVector.begin() == pointerVector.end());
			Assert::IsTrue(fooVector.begin() == fooVector.end());

			intVector.PushBack(randomIntData);
			stringVector.PushBack(randomString);
			pointerVector.PushBack(&randomIntData);
			fooVector.PushBack(randomFoo);

			Assert::IsFalse(intVector.begin() == intVector.end());
			Assert::IsFalse(stringVector.begin() == stringVector.end());
			Assert::IsFalse(pointerVector.begin() == pointerVector.end());
			Assert::IsFalse(fooVector.begin() == fooVector.end());
		}

		TEST_METHOD(VectorPushBack)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::uint32_t vectorSize = intVector.Size();

			std::int32_t randomIntData = rand(), nextRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt);

			InvalidResizeStrategy invalidResizeStrategy;
			ResizeStrategy defaultResizeStrategy;

			intVector.SetResizeStrategy(&invalidResizeStrategy);
			stringVector.SetResizeStrategy(&invalidResizeStrategy);
			pointerVector.SetResizeStrategy(&invalidResizeStrategy);
			fooVector.SetResizeStrategy(&invalidResizeStrategy);

			for (std::uint32_t inserts = (intVector.Capacity() - intVector.Size()); inserts > 0; --inserts)
			{
				intVector.PushBack(randomIntData);
				stringVector.PushBack(randomString);
				pointerVector.PushBack(&randomIntData);
				fooVector.PushBack(randomFoo);
			}

			auto intPushBackPointer = [&intVector, randomIntData] { intVector.PushBack(randomIntData); };
			auto stringPushBackPointer = [&stringVector, randomString] { stringVector.PushBack(randomString); };
			auto pointerPushBackPointer = [&pointerVector, &randomIntData] { pointerVector.PushBack(&randomIntData); };
			auto fooPushBackPointer = [&fooVector, randomFoo] { fooVector.PushBack(randomFoo); };

			Assert::ExpectException<std::exception>(intPushBackPointer);
			Assert::ExpectException<std::exception>(stringPushBackPointer);
			Assert::ExpectException<std::exception>(pointerPushBackPointer);
			Assert::ExpectException<std::exception>(fooPushBackPointer);

			intVector.Clear();
			stringVector.Clear();
			pointerVector.Clear();
			fooVector.Clear();

			intVector.SetResizeStrategy(&defaultResizeStrategy);
			stringVector.SetResizeStrategy(&defaultResizeStrategy);
			pointerVector.SetResizeStrategy(&defaultResizeStrategy);
			fooVector.SetResizeStrategy(&defaultResizeStrategy);

			Vector<std::int32_t>::Iterator intIt = intVector.PushBack(randomIntData);
			Vector<std::string>::Iterator stringIt = stringVector.PushBack(randomString);
			Vector<std::int32_t*>::Iterator pointerIt = pointerVector.PushBack(&randomIntData);
			Vector<Foo>::Iterator fooIt = fooVector.PushBack(randomFoo);

			Assert::AreEqual(randomIntData, *intIt);
			Assert::AreEqual(randomString, *stringIt);
			Assert::AreEqual(&randomIntData, *pointerIt);
			Assert::IsTrue(randomFoo == *fooIt);

			++vectorSize;

			Assert::AreEqual(vectorSize, intVector.Size());
			Assert::AreEqual(randomIntData, intVector.Back());

			Assert::AreEqual(vectorSize, stringVector.Size());
			Assert::AreEqual(randomString, stringVector.Back());

			Assert::AreEqual(vectorSize, pointerVector.Size());
			Assert::AreEqual(&randomIntData, pointerVector.Back());

			Assert::AreEqual(vectorSize, fooVector.Size());
			Assert::IsTrue(randomFoo == fooVector.Back());

			intVector.PushBack(nextRandomInt);
			stringVector.PushBack(nextRandomString);
			pointerVector.PushBack(&nextRandomInt);
			fooVector.PushBack(nextRandomFoo);

			++vectorSize;

			Assert::AreEqual(vectorSize, intVector.Size());
			Assert::AreEqual(nextRandomInt, intVector.Back());

			Assert::AreEqual(vectorSize, stringVector.Size());
			Assert::AreEqual(nextRandomString, stringVector.Back());

			Assert::AreEqual(vectorSize, pointerVector.Size());
			Assert::AreEqual(&nextRandomInt, pointerVector.Back());

			Assert::AreEqual(vectorSize, fooVector.Size());
			Assert::IsTrue(nextRandomFoo == fooVector.Back());

			std::uint32_t vectorCapacity = intVector.Capacity();
			vectorSize = intVector.Size();

			for (std::int32_t insertsLeft = vectorCapacity - vectorSize; insertsLeft >= 0; --insertsLeft)
			{
				intVector.PushBack(randomIntData);
				stringVector.PushBack(randomString);
				pointerVector.PushBack(&randomIntData);
				fooVector.PushBack(randomFoo);
			}

			Assert::AreNotEqual(vectorCapacity, intVector.Capacity());
			Assert::AreNotEqual(vectorCapacity, stringVector.Capacity());
			Assert::AreNotEqual(vectorCapacity, pointerVector.Capacity());
			Assert::AreNotEqual(vectorCapacity, fooVector.Capacity());
		}

		TEST_METHOD(VectorCopyConstructor)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			intVector.PushBack(randomIntData);
			stringVector.PushBack(randomString);
			pointerVector.PushBack(&randomIntData);
			fooVector.PushBack(randomFoo);

			Vector<std::int32_t> otherIntVector(intVector);
			Vector<std::string> otherStringVector(stringVector);
			Vector<std::int32_t*> otherPointerVector(pointerVector);
			Vector<Foo> otherFooVector(fooVector);

			Assert::AreEqual(intVector.Size(), otherIntVector.Size());
			Assert::AreEqual(intVector.Capacity(), otherIntVector.Capacity());
			Assert::AreEqual(intVector.Front(), otherIntVector.Front());

			Assert::AreEqual(stringVector.Size(), otherStringVector.Size());
			Assert::AreEqual(stringVector.Capacity(), otherStringVector.Capacity());
			Assert::AreEqual(stringVector.Front(), otherStringVector.Front());

			Assert::AreEqual(pointerVector.Size(), otherPointerVector.Size());
			Assert::AreEqual(pointerVector.Capacity(), otherPointerVector.Capacity());
			Assert::AreEqual(pointerVector.Front(), otherPointerVector.Front());

			Assert::AreEqual(fooVector.Size(), otherFooVector.Size());
			Assert::AreEqual(fooVector.Capacity(), otherFooVector.Capacity());
			Assert::IsTrue(fooVector.Front() == otherFooVector.Front());
		}

		TEST_METHOD(VectorAssignment)
		{
			Vector<std::int32_t> intVector, otherIntVector;
			Vector<std::string> stringVector, otherStringVector;
			Vector<std::int32_t*> pointerVector, otherPointerVector;
			Vector<Foo> fooVector, otherFooVector;

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			intVector.PushBack(randomIntData);
			stringVector.PushBack(randomString);
			pointerVector.PushBack(&randomIntData);
			fooVector.PushBack(randomFoo);

			otherIntVector = intVector;
			otherStringVector = stringVector;
			otherPointerVector = pointerVector;
			otherFooVector = fooVector;

			Assert::AreEqual(intVector.Size(), otherIntVector.Size());
			Assert::AreEqual(intVector.Capacity(), otherIntVector.Capacity());
			Assert::AreEqual(intVector.Front(), otherIntVector.Front());

			Assert::AreEqual(stringVector.Size(), otherStringVector.Size());
			Assert::AreEqual(stringVector.Capacity(), otherStringVector.Capacity());
			Assert::AreEqual(stringVector.Front(), otherStringVector.Front());

			Assert::AreEqual(pointerVector.Size(), otherPointerVector.Size());
			Assert::AreEqual(pointerVector.Capacity(), otherPointerVector.Capacity());
			Assert::AreEqual(pointerVector.Front(), otherPointerVector.Front());

			Assert::AreEqual(fooVector.Size(), otherFooVector.Size());
			Assert::AreEqual(fooVector.Capacity(), otherFooVector.Capacity());
			Assert::IsTrue(fooVector.Front() == otherFooVector.Front());
		}

		TEST_METHOD(VectorAt)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::int32_t randomIntData = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi"), thirdRandomString = std::string("hello");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			intVector.PushBack(randomIntData);
			intVector.PushBack(nextRandomInt);
			intVector.PushBack(thirdRandomInt);

			stringVector.PushBack(randomString);
			stringVector.PushBack(nextRandomString);
			stringVector.PushBack(thirdRandomString);

			pointerVector.PushBack(&randomIntData);
			pointerVector.PushBack(&nextRandomInt);
			pointerVector.PushBack(&thirdRandomInt);

			fooVector.PushBack(randomFoo);
			fooVector.PushBack(nextRandomFoo);
			fooVector.PushBack(thirdRandomFoo);

			Assert::AreEqual(randomIntData, intVector.At(0));
			Assert::AreEqual(nextRandomInt, intVector.At(1));
			Assert::AreEqual(thirdRandomInt, intVector.At(2));

			Assert::AreEqual(randomString, stringVector.At(0));
			Assert::AreEqual(nextRandomString, stringVector.At(1));
			Assert::AreEqual(thirdRandomString, stringVector.At(2));

			Assert::AreEqual(&randomIntData, pointerVector.At(0));
			Assert::AreEqual(&nextRandomInt, pointerVector.At(1));
			Assert::AreEqual(&thirdRandomInt, pointerVector.At(2));

			Assert::IsTrue(randomFoo == fooVector.At(0));
			Assert::IsTrue(nextRandomFoo == fooVector.At(1));
			Assert::IsTrue(thirdRandomFoo == fooVector.At(2));

			std::uint32_t vectorCapacity = intVector.Capacity();

			auto intAtPointer = [&intVector, vectorCapacity] { intVector.At(vectorCapacity); };
			auto stringAtPointer = [&stringVector, vectorCapacity] { stringVector.At(vectorCapacity); };
			auto pointerAtPointer = [&pointerVector, vectorCapacity] { pointerVector.At(vectorCapacity); };
			auto fooAtPointer = [&fooVector, vectorCapacity] { fooVector.At(vectorCapacity); };

			Assert::ExpectException<std::exception>(intAtPointer);
			Assert::ExpectException<std::exception>(stringAtPointer);
			Assert::ExpectException<std::exception>(pointerAtPointer);
			Assert::ExpectException<std::exception>(fooAtPointer);

			const Vector<std::int32_t> constIntVector;
			const Vector<std::string> constStringVector;
			const Vector<std::int32_t*> constPointerVector;
			const Vector<Foo> constFooVector;

			auto constIntAccessorPointer = [&constIntVector, vectorCapacity] { constIntVector.At(vectorCapacity); };
			auto constStringAccessorPointer = [&constStringVector, vectorCapacity] { constStringVector.At(vectorCapacity); };
			auto constPointerAccessorPointer = [&constPointerVector, vectorCapacity] { constPointerVector.At(vectorCapacity); };
			auto constFooAccessorPointer = [&constFooVector, vectorCapacity] { constFooVector.At(vectorCapacity); };

			Assert::ExpectException<std::exception>(constIntAccessorPointer);
			Assert::ExpectException<std::exception>(constStringAccessorPointer);
			Assert::ExpectException<std::exception>(constPointerAccessorPointer);
			Assert::ExpectException<std::exception>(constFooAccessorPointer);
		}

		TEST_METHOD(VectorDestructor)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::int32_t randomIntData = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi"), thirdRandomString = std::string("hello");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			intVector.PushBack(randomIntData);
			intVector.PushBack(nextRandomInt);
			intVector.PushBack(thirdRandomInt);

			stringVector.PushBack(randomString);
			stringVector.PushBack(nextRandomString);
			stringVector.PushBack(thirdRandomString);

			pointerVector.PushBack(&randomIntData);
			pointerVector.PushBack(&nextRandomInt);
			pointerVector.PushBack(&thirdRandomInt);

			fooVector.PushBack(randomFoo);
			fooVector.PushBack(nextRandomFoo);
			fooVector.PushBack(thirdRandomFoo);
		}

		TEST_METHOD(VectorShrinkToFit)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::int32_t randomIntData = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi"), thirdRandomString = std::string("hello");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			for (std::uint32_t inserts = intVector.defaultCapacity + 1; inserts > 0; --inserts)
			{
				intVector.PushBack(randomIntData);
				stringVector.PushBack(randomString);
				pointerVector.PushBack(&randomIntData);
				fooVector.PushBack(randomFoo);
			}

			intVector.ShrinkToFit();
			stringVector.ShrinkToFit();
			pointerVector.ShrinkToFit();
			fooVector.ShrinkToFit();

			Assert::AreEqual(intVector.Size(), intVector.Capacity());
			Assert::AreEqual(stringVector.Size(), stringVector.Capacity());
			Assert::AreEqual(pointerVector.Size(), pointerVector.Capacity());
			Assert::AreEqual(fooVector.Size(), fooVector.Capacity());
		}

		TEST_METHOD(VectorReserve)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::int32_t randomIntData = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi"), thirdRandomString = std::string("hello");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			std::uint32_t capacity = intVector.Capacity();

			++capacity;

			intVector.Reserve(capacity);
			stringVector.Reserve(capacity);
			pointerVector.Reserve(capacity);
			fooVector.Reserve(capacity);

			Assert::AreEqual(capacity, intVector.Capacity());
			Assert::AreEqual(capacity, stringVector.Capacity());
			Assert::AreEqual(capacity, pointerVector.Capacity());
			Assert::AreEqual(capacity, fooVector.Capacity());

			--capacity;

			intVector.Reserve(capacity);
			stringVector.Reserve(capacity);
			pointerVector.Reserve(capacity);
			fooVector.Reserve(capacity);

			Assert::AreNotEqual(capacity, intVector.Capacity());
			Assert::AreNotEqual(capacity, stringVector.Capacity());
			Assert::AreNotEqual(capacity, pointerVector.Capacity());
			Assert::AreNotEqual(capacity, fooVector.Capacity());

			intVector.PushBack(randomIntData);
			intVector.PushBack(nextRandomInt);
			intVector.PushBack(thirdRandomInt);

			stringVector.PushBack(randomString);
			stringVector.PushBack(nextRandomString);
			stringVector.PushBack(thirdRandomString);

			pointerVector.PushBack(&randomIntData);
			pointerVector.PushBack(&nextRandomInt);
			pointerVector.PushBack(&thirdRandomInt);

			fooVector.PushBack(randomFoo);
			fooVector.PushBack(nextRandomFoo);
			fooVector.PushBack(thirdRandomFoo);

			++capacity;

			intVector.Reserve(capacity);
			stringVector.Reserve(capacity);
			pointerVector.Reserve(capacity);
			fooVector.Reserve(capacity);

			Assert::AreEqual(randomIntData, intVector[0]);
			Assert::AreEqual(nextRandomInt, intVector[1]);
			Assert::AreEqual(thirdRandomInt, intVector[2]);

			Assert::AreEqual(randomString, stringVector[0]);
			Assert::AreEqual(nextRandomString, stringVector[1]);
			Assert::AreEqual(thirdRandomString, stringVector[2]);

			Assert::AreEqual(&randomIntData, pointerVector[0]);
			Assert::AreEqual(&nextRandomInt, pointerVector[1]);
			Assert::AreEqual(&thirdRandomInt, pointerVector[2]);

			Assert::IsTrue(randomFoo == fooVector[0]);
			Assert::IsTrue(nextRandomFoo == fooVector[1]);
			Assert::IsTrue(thirdRandomFoo == fooVector[2]);
		}
		
		TEST_METHOD(VectorFind)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::int32_t randomIntData = rand(), nextRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt);

			Assert::IsTrue(intVector.end() == intVector.Find(randomIntData));
			Assert::IsTrue(stringVector.end() == stringVector.Find(randomString));
			Assert::IsTrue(pointerVector.end() == pointerVector.Find(&randomIntData));
			Assert::IsTrue(fooVector.end() == fooVector.Find(randomFoo));

			intVector.PushBack(randomIntData);
			intVector.PushBack(nextRandomInt);

			stringVector.PushBack(randomString);
			stringVector.PushBack(nextRandomString);

			pointerVector.PushBack(&randomIntData);
			pointerVector.PushBack(&nextRandomInt);

			fooVector.PushBack(randomFoo);
			fooVector.PushBack(nextRandomFoo);

			Assert::IsTrue(intVector.begin() == intVector.Find(randomIntData));
			Assert::IsTrue(stringVector.begin() == stringVector.Find(randomString));
			Assert::IsTrue(pointerVector.begin() == pointerVector.Find(&randomIntData));
			Assert::IsTrue(fooVector.begin() == fooVector.Find(randomFoo));
		}

		TEST_METHOD(VectorClear)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::int32_t randomIntData = rand(), nextRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi"), thirdRandomString = std::string("hello");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt);

			intVector.PushBack(randomIntData);
			intVector.PushBack(nextRandomInt);

			stringVector.PushBack(randomString);
			stringVector.PushBack(nextRandomString);

			pointerVector.PushBack(&randomIntData);
			pointerVector.PushBack(&nextRandomInt);

			fooVector.PushBack(randomFoo);
			fooVector.PushBack(nextRandomFoo);

			intVector.Clear();
			stringVector.Clear();
			pointerVector.Clear();
			fooVector.Clear();

			Assert::IsTrue(intVector.IsEmpty());
			Assert::IsTrue(stringVector.IsEmpty());
			Assert::IsTrue(pointerVector.IsEmpty());
			Assert::IsTrue(fooVector.IsEmpty());

			Assert::IsTrue(intVector.end() == intVector.Find(randomIntData));
			Assert::IsTrue(intVector.end() == intVector.Find(nextRandomInt));

			Assert::IsTrue(stringVector.end() == stringVector.Find(randomString));
			Assert::IsTrue(stringVector.end() == stringVector.Find(randomString));

			Assert::IsTrue(pointerVector.end() == pointerVector.Find(&randomIntData));
			Assert::IsTrue(pointerVector.end() == pointerVector.Find(&nextRandomInt));

			Assert::IsTrue(fooVector.end() == fooVector.Find(randomFoo));
			Assert::IsTrue(fooVector.end() == fooVector.Find(nextRandomFoo));
		}

		TEST_METHOD(VectorRemove)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::int32_t randomIntData = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi"), thirdRandomString = std::string("hello");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			intVector.PushBack(randomIntData);
			intVector.PushBack(nextRandomInt);
			intVector.PushBack(thirdRandomInt);

			stringVector.PushBack(randomString);
			stringVector.PushBack(nextRandomString);
			stringVector.PushBack(thirdRandomString);

			pointerVector.PushBack(&randomIntData);
			pointerVector.PushBack(&nextRandomInt);
			pointerVector.PushBack(&thirdRandomInt);

			fooVector.PushBack(randomFoo);
			fooVector.PushBack(nextRandomFoo);
			fooVector.PushBack(thirdRandomFoo);

			std::uint32_t vectorSize = intVector.Size();

			intVector.Remove(nextRandomInt);
			stringVector.Remove(nextRandomString);
			pointerVector.Remove(&nextRandomInt);
			fooVector.Remove(nextRandomFoo);
			
			--vectorSize;

			Assert::AreEqual(vectorSize, intVector.Size());
			Assert::IsTrue(intVector.end() == intVector.Find(nextRandomInt));

			Assert::AreEqual(vectorSize, stringVector.Size());
			Assert::IsTrue(stringVector.end() == stringVector.Find(nextRandomString));

			Assert::AreEqual(vectorSize, pointerVector.Size());
			Assert::IsTrue(pointerVector.end() == pointerVector.Find(&nextRandomInt));

			Assert::AreEqual(vectorSize, fooVector.Size());
			Assert::IsTrue(fooVector.end() == fooVector.Find(nextRandomFoo));

			intVector.Remove(nextRandomInt);
			stringVector.Remove(nextRandomString);
			pointerVector.Remove(&nextRandomInt);
			fooVector.Remove(nextRandomFoo);

			Assert::AreEqual(vectorSize, intVector.Size());
			Assert::AreEqual(vectorSize, stringVector.Size());
			Assert::AreEqual(vectorSize, pointerVector.Size());
			Assert::AreEqual(vectorSize, fooVector.Size());

			intVector.Remove(randomIntData);
			stringVector.Remove(randomString);
			pointerVector.Remove(&randomIntData);
			fooVector.Remove(randomFoo);

			--vectorSize;

			Assert::AreEqual(vectorSize, intVector.Size());
			Assert::IsTrue(intVector.end() == intVector.Find(randomIntData));

			Assert::AreEqual(vectorSize, stringVector.Size());
			Assert::IsTrue(stringVector.end() == stringVector.Find(randomString));

			Assert::AreEqual(vectorSize, pointerVector.Size());
			Assert::IsTrue(pointerVector.end() == pointerVector.Find(&randomIntData));

			Assert::AreEqual(vectorSize, fooVector.Size());
			Assert::IsTrue(fooVector.end() == fooVector.Find(randomFoo));

			intVector.Remove(thirdRandomInt);
			stringVector.Remove(thirdRandomString);
			pointerVector.Remove(&thirdRandomInt);
			fooVector.Remove(thirdRandomFoo);

			--vectorSize;

			Assert::AreEqual(vectorSize, intVector.Size());
			Assert::IsTrue(intVector.end() == intVector.Find(thirdRandomInt));

			Assert::AreEqual(vectorSize, stringVector.Size());
			Assert::IsTrue(stringVector.end() == stringVector.Find(thirdRandomString));

			Assert::AreEqual(vectorSize, pointerVector.Size());
			Assert::IsTrue(pointerVector.end() == pointerVector.Find(&thirdRandomInt));

			Assert::AreEqual(vectorSize, fooVector.Size());
			Assert::IsTrue(fooVector.end() == fooVector.Find(thirdRandomFoo));

			intVector.Remove(nextRandomInt);
			stringVector.Remove(nextRandomString);
			pointerVector.Remove(&nextRandomInt);
			fooVector.Remove(nextRandomFoo);

			Assert::AreEqual(vectorSize, intVector.Size());
			Assert::IsTrue(intVector.end() == intVector.Find(nextRandomInt));

			Assert::AreEqual(vectorSize, stringVector.Size());
			Assert::IsTrue(stringVector.end() == stringVector.Find(nextRandomString));

			Assert::AreEqual(vectorSize, pointerVector.Size());
			Assert::IsTrue(pointerVector.end() == pointerVector.Find(&nextRandomInt));

			Assert::AreEqual(vectorSize, fooVector.Size());
			Assert::IsTrue(fooVector.end() == fooVector.Find(nextRandomFoo));
		}

		TEST_METHOD(VectorSetResizeStrategy)
		{
			TestResizeStrategy testResizeStrategy;

			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			intVector.SetResizeStrategy(&testResizeStrategy);
			stringVector.SetResizeStrategy(&testResizeStrategy);
			pointerVector.SetResizeStrategy(&testResizeStrategy);
			fooVector.SetResizeStrategy(&testResizeStrategy);

			std::uint32_t currentCapacity = intVector.Capacity();

			for (std::uint32_t inserts = (intVector.Capacity() - intVector.Size()) + 1; inserts > 0; --inserts)
			{
				intVector.PushBack(randomIntData);
				stringVector.PushBack(randomString);
				pointerVector.PushBack(&randomIntData);
				fooVector.PushBack(randomFoo);
			}

			std::uint32_t increment = testResizeStrategy(currentCapacity, currentCapacity);

			Assert::AreEqual(currentCapacity + increment, intVector.Capacity());
			Assert::AreEqual(currentCapacity + increment, stringVector.Capacity());
			Assert::AreEqual(currentCapacity + increment, pointerVector.Capacity());
			Assert::AreEqual(currentCapacity + increment, fooVector.Capacity());

			currentCapacity = intVector.Capacity();

			for (std::uint32_t inserts = (intVector.Capacity() - intVector.Size()) + 1; inserts > 0; --inserts)
			{
				intVector.PushBack(randomIntData);
				stringVector.PushBack(randomString);
				pointerVector.PushBack(&randomIntData);
				fooVector.PushBack(randomFoo);
			}

			increment = testResizeStrategy(currentCapacity, currentCapacity);

			Assert::AreEqual(currentCapacity + increment, intVector.Capacity());
			Assert::AreEqual(currentCapacity + increment, stringVector.Capacity());
			Assert::AreEqual(currentCapacity + increment, pointerVector.Capacity());
			Assert::AreEqual(currentCapacity + increment, fooVector.Capacity());
		}

		TEST_METHOD(VectorRemoveRange)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::int32_t randomIntData = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi"), thirdRandomString = std::string("hello");
			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			intVector.PushBack(randomIntData);
			intVector.PushBack(nextRandomInt);
			intVector.PushBack(thirdRandomInt);
			intVector.PushBack(randomIntData);
			intVector.PushBack(nextRandomInt);
			intVector.PushBack(thirdRandomInt);

			stringVector.PushBack(randomString);
			stringVector.PushBack(nextRandomString);
			stringVector.PushBack(thirdRandomString);
			stringVector.PushBack(randomString);
			stringVector.PushBack(nextRandomString);
			stringVector.PushBack(thirdRandomString);

			pointerVector.PushBack(&randomIntData);
			pointerVector.PushBack(&nextRandomInt);
			pointerVector.PushBack(&thirdRandomInt);
			pointerVector.PushBack(&randomIntData);
			pointerVector.PushBack(&nextRandomInt);
			pointerVector.PushBack(&thirdRandomInt);

			fooVector.PushBack(randomFoo);
			fooVector.PushBack(nextRandomFoo);
			fooVector.PushBack(thirdRandomFoo);
			fooVector.PushBack(randomFoo);
			fooVector.PushBack(nextRandomFoo);
			fooVector.PushBack(thirdRandomFoo);

			std::uint32_t vectorSize = intVector.Size();

			intVector.Remove(intVector.end(), intVector.begin());
			stringVector.Remove(stringVector.end(), stringVector.begin());
			pointerVector.Remove(pointerVector.end(), pointerVector.begin());
			fooVector.Remove(fooVector.end(), fooVector.begin());

			Assert::AreEqual(vectorSize, intVector.Size());
			Assert::AreEqual(vectorSize, stringVector.Size());
			Assert::AreEqual(vectorSize, pointerVector.Size());
			Assert::AreEqual(vectorSize, fooVector.Size());

			intVector.Remove(intVector.Find(randomIntData), intVector.Find(randomIntData));
			stringVector.Remove(stringVector.Find(randomString), stringVector.Find(randomString));
			pointerVector.Remove(pointerVector.Find(&randomIntData), pointerVector.Find(&randomIntData));
			fooVector.Remove(fooVector.Find(randomFoo), fooVector.Find(randomFoo));

			--vectorSize;

			Assert::AreEqual(vectorSize, intVector.Size());
			Assert::AreEqual(nextRandomInt, intVector.At(0));
			Assert::AreEqual(thirdRandomInt, intVector.At(1));
			Assert::AreEqual(randomIntData, intVector.At(2));
			Assert::AreEqual(nextRandomInt, intVector.At(3));
			Assert::AreEqual(thirdRandomInt, intVector.At(4));

			Assert::AreEqual(vectorSize, stringVector.Size());
			Assert::AreEqual(nextRandomString, stringVector.At(0));
			Assert::AreEqual(thirdRandomString, stringVector.At(1));
			Assert::AreEqual(randomString, stringVector.At(2));
			Assert::AreEqual(nextRandomString, stringVector.At(3));
			Assert::AreEqual(thirdRandomString, stringVector.At(4));

			Assert::AreEqual(vectorSize, pointerVector.Size());
			Assert::AreEqual(&nextRandomInt, pointerVector.At(0));
			Assert::AreEqual(&thirdRandomInt, pointerVector.At(1));
			Assert::AreEqual(&randomIntData, pointerVector.At(2));
			Assert::AreEqual(&nextRandomInt, pointerVector.At(3));
			Assert::AreEqual(&thirdRandomInt, pointerVector.At(4));

			Assert::IsTrue(vectorSize == fooVector.Size());
			Assert::IsTrue(nextRandomFoo == fooVector.At(0));
			Assert::IsTrue(thirdRandomFoo == fooVector.At(1));
			Assert::IsTrue(randomFoo == fooVector.At(2));
			Assert::IsTrue(nextRandomFoo == fooVector.At(3));
			Assert::IsTrue(thirdRandomFoo == fooVector.At(4));

			intVector.Remove(intVector.begin(), intVector.Find(randomIntData));
			stringVector.Remove(stringVector.begin(), stringVector.Find(randomString));
			pointerVector.Remove(pointerVector.begin(), pointerVector.Find(&randomIntData));
			fooVector.Remove(fooVector.begin(), fooVector.Find(randomFoo));

			vectorSize -= 3;

			Assert::AreEqual(vectorSize, intVector.Size());
			Assert::AreEqual(nextRandomInt, intVector.At(0));
			Assert::AreEqual(thirdRandomInt, intVector.At(1));

			Assert::AreEqual(vectorSize, stringVector.Size());
			Assert::AreEqual(nextRandomString, stringVector.At(0));
			Assert::AreEqual(thirdRandomString, stringVector.At(1));

			Assert::AreEqual(vectorSize, pointerVector.Size());
			Assert::AreEqual(&nextRandomInt, pointerVector.At(0));
			Assert::AreEqual(&thirdRandomInt, pointerVector.At(1));

			Assert::IsTrue(vectorSize == fooVector.Size());
			Assert::IsTrue(nextRandomFoo == fooVector.At(0));
			Assert::IsTrue(thirdRandomFoo == fooVector.At(1));

			intVector.PushBack(thirdRandomInt);
			intVector.PushBack(randomIntData);
			intVector.PushBack(nextRandomInt);

			stringVector.PushBack(thirdRandomString);
			stringVector.PushBack(randomString);
			stringVector.PushBack(nextRandomString);

			pointerVector.PushBack(&thirdRandomInt);
			pointerVector.PushBack(&randomIntData);
			pointerVector.PushBack(&nextRandomInt);

			fooVector.PushBack(thirdRandomFoo);
			fooVector.PushBack(randomFoo);
			fooVector.PushBack(nextRandomFoo);

			vectorSize = intVector.Size();

			intVector.Remove(intVector.Find(thirdRandomInt), intVector.Find(randomIntData));
			stringVector.Remove(stringVector.Find(thirdRandomString), stringVector.Find(randomString));
			pointerVector.Remove(pointerVector.Find(&thirdRandomInt), pointerVector.Find(&randomIntData));
			fooVector.Remove(fooVector.Find(thirdRandomFoo), fooVector.Find(randomFoo));

			vectorSize -= 3;

			Assert::AreEqual(vectorSize, intVector.Size());
			Assert::AreEqual(nextRandomInt, intVector.At(0));
			Assert::AreEqual(nextRandomInt, intVector.At(1));

			Assert::AreEqual(vectorSize, stringVector.Size());
			Assert::AreEqual(nextRandomString, stringVector.At(0));
			Assert::AreEqual(nextRandomString, stringVector.At(1));

			Assert::AreEqual(vectorSize, pointerVector.Size());
			Assert::AreEqual(&nextRandomInt, pointerVector.At(0));
			Assert::AreEqual(&nextRandomInt, pointerVector.At(1));

			Assert::AreEqual(vectorSize, fooVector.Size());
			Assert::IsTrue(nextRandomFoo == fooVector.At(0));
			Assert::IsTrue(nextRandomFoo == fooVector.At(1));
		}

		TEST_METHOD(VectorMoveConstructor)
		{
			std::int32_t randomIntData = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi"), thirdRandomString = std::string("hello");

			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			intVector.PushBack(randomIntData);
			intVector.PushBack(nextRandomInt);
			intVector.PushBack(thirdRandomInt);

			stringVector.PushBack(randomString);
			stringVector.PushBack(nextRandomString);
			stringVector.PushBack(thirdRandomString);

			pointerVector.PushBack(&randomIntData);
			pointerVector.PushBack(&nextRandomInt);
			pointerVector.PushBack(&thirdRandomInt);

			fooVector.PushBack(randomFoo);
			fooVector.PushBack(nextRandomFoo);
			fooVector.PushBack(thirdRandomFoo);

			Vector<std::int32_t> otherIntVector = std::move(intVector);
			Vector<std::string> otherStringVector = std::move(stringVector);
			Vector<std::int32_t*> otherPointerVector = std::move(pointerVector);
			Vector<Foo> otherFooVector = std::move(fooVector);

			Assert::IsTrue(intVector.IsEmpty());
			Assert::IsTrue(stringVector.IsEmpty());
			Assert::IsTrue(pointerVector.IsEmpty());
			Assert::IsTrue(fooVector.IsEmpty());

			Assert::IsFalse(otherIntVector.IsEmpty());
			Assert::IsFalse(otherStringVector.IsEmpty());
			Assert::IsFalse(otherPointerVector.IsEmpty());
			Assert::IsFalse(otherFooVector.IsEmpty());

			Assert::IsTrue(otherIntVector.At(0) == randomIntData);
			Assert::IsTrue(otherStringVector[0] == randomString);
			Assert::IsTrue(otherPointerVector[0] == &randomIntData);
			Assert::IsTrue(otherFooVector[0] == randomFoo);

			Assert::IsTrue(otherIntVector[1] == nextRandomInt);
			Assert::IsTrue(otherStringVector[1] == nextRandomString);
			Assert::IsTrue(otherPointerVector[1] == &nextRandomInt);
			Assert::IsTrue(otherFooVector[1] == nextRandomFoo);

			Assert::IsTrue(otherIntVector[2] == thirdRandomInt);
			Assert::IsTrue(otherStringVector[2] == thirdRandomString);
			Assert::IsTrue(otherPointerVector[2] == &thirdRandomInt);
			Assert::IsTrue(otherFooVector[2] == thirdRandomFoo);
		}

		TEST_METHOD(VectorMoveAssignmentOperator)
		{
			std::int32_t randomIntData = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			std::string randomString = std::string("howdy"), nextRandomString = std::string("hi"), thirdRandomString = std::string("hello");

			Foo randomFoo = Foo(randomIntData), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			intVector.PushBack(randomIntData);
			intVector.PushBack(nextRandomInt);
			intVector.PushBack(thirdRandomInt);

			stringVector.PushBack(randomString);
			stringVector.PushBack(nextRandomString);
			stringVector.PushBack(thirdRandomString);

			pointerVector.PushBack(&randomIntData);
			pointerVector.PushBack(&nextRandomInt);
			pointerVector.PushBack(&thirdRandomInt);

			fooVector.PushBack(randomFoo);
			fooVector.PushBack(nextRandomFoo);
			fooVector.PushBack(thirdRandomFoo);

			Vector<std::int32_t> otherIntVector;
			Vector<std::string> otherStringVector;
			Vector<std::int32_t*> otherPointerVector;
			Vector<Foo> otherFooVector;

			otherIntVector = std::move(intVector);
			otherStringVector = std::move(stringVector);
			otherPointerVector = std::move(pointerVector);
			otherFooVector = std::move(fooVector);

			Assert::IsTrue(intVector.IsEmpty());
			Assert::IsTrue(stringVector.IsEmpty());
			Assert::IsTrue(pointerVector.IsEmpty());
			Assert::IsTrue(fooVector.IsEmpty());

			Assert::IsFalse(otherIntVector.IsEmpty());
			Assert::IsFalse(otherStringVector.IsEmpty());
			Assert::IsFalse(otherPointerVector.IsEmpty());
			Assert::IsFalse(otherFooVector.IsEmpty());

			Assert::IsTrue(otherIntVector[0] == randomIntData);
			Assert::IsTrue(otherStringVector[0] == randomString);
			Assert::IsTrue(otherPointerVector[0] == &randomIntData);
			Assert::IsTrue(otherFooVector[0] == randomFoo);

			Assert::IsTrue(otherIntVector[1] == nextRandomInt);
			Assert::IsTrue(otherStringVector[1] == nextRandomString);
			Assert::IsTrue(otherPointerVector[1] == &nextRandomInt);
			Assert::IsTrue(otherFooVector[1] == nextRandomFoo);

			Assert::IsTrue(otherIntVector[2] == thirdRandomInt);
			Assert::IsTrue(otherStringVector[2] == thirdRandomString);
			Assert::IsTrue(otherPointerVector[2] == &thirdRandomInt);
			Assert::IsTrue(otherFooVector[2] == thirdRandomFoo);
		}

		TEST_METHOD(VectorIteratorDefaultConstructor)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			Vector<std::int32_t>::Iterator intIt, otherIntIt;
			Vector<std::string>::Iterator stringIt, otherStringIt;
			Vector<std::int32_t*>::Iterator pointerIt, otherPointerIt;
			Vector<Foo>::Iterator fooIt, otherFooIt;

			Assert::IsTrue(intIt != intVector.end());
			Assert::IsTrue(intIt == otherIntIt);

			Assert::IsTrue(stringIt != stringVector.end());
			Assert::IsTrue(stringIt == otherStringIt);

			Assert::IsTrue(pointerIt != pointerVector.end());
			Assert::IsTrue(pointerIt == otherPointerIt);

			Assert::IsTrue(fooIt != fooVector.end());
			Assert::IsTrue(fooIt == otherFooIt);

			auto intItDereferencePointer = [intIt] { intIt.operator*(); };
			auto stringItDereferencePointer = [stringIt] { stringIt.operator*(); };
			auto pointerItDereferencePointer = [pointerIt] { pointerIt.operator*(); };
			auto fooItDereferencePointer = [fooIt] { fooIt.operator*(); };

			Assert::ExpectException<std::exception>(intItDereferencePointer);
			Assert::ExpectException<std::exception>(stringItDereferencePointer);
			Assert::ExpectException<std::exception>(pointerItDereferencePointer);
			Assert::ExpectException<std::exception>(fooItDereferencePointer);
		}

		TEST_METHOD(VectorIteratorComparisonOperator)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			Assert::IsTrue(intVector.end() == intVector.end());
			Assert::IsTrue(intVector.begin() == intVector.end());

			Assert::IsTrue(stringVector.end() == stringVector.end());
			Assert::IsTrue(stringVector.begin() == stringVector.end());

			Assert::IsTrue(pointerVector.end() == pointerVector.end());
			Assert::IsTrue(pointerVector.begin() == pointerVector.end());

			Assert::IsTrue(fooVector.end() == fooVector.end());
			Assert::IsTrue(fooVector.begin() == fooVector.end());

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			intVector.PushBack(randomIntData);
			stringVector.PushBack(randomString);
			pointerVector.PushBack(&randomIntData);
			fooVector.PushBack(randomFoo);

			Assert::IsFalse(intVector.begin() == intVector.end());
			Assert::IsFalse(stringVector.begin() == stringVector.end());
			Assert::IsFalse(pointerVector.begin() == pointerVector.end());
			Assert::IsFalse(fooVector.begin() == fooVector.end());
		}

		TEST_METHOD(VectorIteratorInequalityOperator)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			Assert::IsFalse(intVector.end() != intVector.end());
			Assert::IsFalse(intVector.begin() != intVector.end());

			Assert::IsFalse(stringVector.end() != stringVector.end());
			Assert::IsFalse(stringVector.begin() != stringVector.end());

			Assert::IsFalse(pointerVector.end() != pointerVector.end());
			Assert::IsFalse(pointerVector.begin() != pointerVector.end());

			Assert::IsFalse(fooVector.end() != fooVector.end());
			Assert::IsFalse(fooVector.begin() != fooVector.end());

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			intVector.PushBack(randomIntData);
			stringVector.PushBack(randomString);
			pointerVector.PushBack(&randomIntData);
			fooVector.PushBack(randomFoo);

			Assert::IsTrue(intVector.begin() != intVector.end());
			Assert::IsTrue(stringVector.begin() != stringVector.end());
			Assert::IsTrue(pointerVector.begin() != pointerVector.end());
			Assert::IsTrue(fooVector.begin() != fooVector.end());
		}

		TEST_METHOD(VectorIteratorPostIncrement)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			intVector.PushBack(randomIntData);
			stringVector.PushBack(randomString);
			pointerVector.PushBack(&randomIntData);
			fooVector.PushBack(randomFoo);

			Vector<std::int32_t>::Iterator intIt = intVector.end();
			Vector<std::string>::Iterator stringIt = stringVector.end();
			Vector<std::int32_t*>::Iterator pointerIt = pointerVector.end();
			Vector<Foo>::Iterator fooIt = fooVector.end();

			Assert::IsTrue(intIt++ == intVector.end());
			Assert::IsTrue(intIt == intVector.end());

			Assert::IsTrue(stringIt++ == stringVector.end());
			Assert::IsTrue(stringIt == stringVector.end());

			Assert::IsTrue(pointerIt++ == pointerVector.end());
			Assert::IsTrue(pointerIt == pointerVector.end());

			Assert::IsTrue(fooIt++ == fooVector.end());
			Assert::IsTrue(fooIt == fooVector.end());

			intIt = intVector.begin();
			stringIt = stringVector.begin();
			pointerIt = pointerVector.begin();
			fooIt = fooVector.begin();

			Assert::IsFalse((intIt++) == intVector.end());
			Assert::IsTrue(intIt == intVector.end());

			Assert::IsFalse((stringIt++) == stringVector.end());
			Assert::IsTrue(stringIt == stringVector.end());

			Assert::IsFalse((pointerIt++) == pointerVector.end());
			Assert::IsTrue(pointerIt == pointerVector.end());

			Assert::IsFalse((fooIt++) == fooVector.end());
			Assert::IsTrue(fooIt == fooVector.end());

			intIt = Vector<std::int32_t>::Iterator();
			stringIt = Vector<std::string>::Iterator();
			pointerIt = Vector<std::int32_t*>::Iterator();
			fooIt = Vector<Foo>::Iterator();

			auto intItIncrementPointer = [&intIt] { intIt++; };
			auto stringItIncrementPointer = [&stringIt] { stringIt++; };
			auto pointerItIncrementPointer = [&pointerIt] { pointerIt++; };
			auto fooItIncrementPointer = [&fooIt] { fooIt++; };

			Assert::ExpectException<std::exception>(intItIncrementPointer);
			Assert::ExpectException<std::exception>(stringItIncrementPointer);
			Assert::ExpectException<std::exception>(pointerItIncrementPointer);
			Assert::ExpectException<std::exception>(fooItIncrementPointer);
		}

		TEST_METHOD(VectorIteratorPreIncrement)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			intVector.PushBack(randomIntData);
			stringVector.PushBack(randomString);
			pointerVector.PushBack(&randomIntData);
			fooVector.PushBack(randomFoo);

			Vector<std::int32_t>::Iterator intIt = intVector.end();
			Vector<std::string>::Iterator stringIt = stringVector.end();
			Vector<std::int32_t*>::Iterator pointerIt = pointerVector.end();
			Vector<Foo>::Iterator fooIt = fooVector.end();

			Assert::IsTrue(++intIt == intVector.end());
			Assert::IsTrue(intIt == intVector.end());

			Assert::IsTrue(++stringIt == stringVector.end());
			Assert::IsTrue(stringIt == stringVector.end());

			Assert::IsTrue(++pointerIt == pointerVector.end());
			Assert::IsTrue(pointerIt == pointerVector.end());

			Assert::IsTrue(++fooIt == fooVector.end());
			Assert::IsTrue(fooIt == fooVector.end());

			intIt = intVector.begin();
			stringIt = stringVector.begin();
			pointerIt = pointerVector.begin();
			fooIt = fooVector.begin();

			Assert::IsTrue((++intIt) == intVector.end());
			Assert::IsTrue(intIt == intVector.end());

			Assert::IsTrue((++stringIt) == stringVector.end());
			Assert::IsTrue(stringIt == stringVector.end());

			Assert::IsTrue((++pointerIt) == pointerVector.end());
			Assert::IsTrue(pointerIt == pointerVector.end());

			Assert::IsTrue((++fooIt) == fooVector.end());
			Assert::IsTrue(fooIt == fooVector.end());

			intIt = Vector<std::int32_t>::Iterator();
			stringIt = Vector<std::string>::Iterator();
			pointerIt = Vector<std::int32_t*>::Iterator();
			fooIt = Vector<Foo>::Iterator();

			auto intItIncrementPointer = [&intIt] { ++intIt; };
			auto stringItIncrementPointer = [&stringIt] { ++stringIt; };
			auto pointerItIncrementPointer = [&pointerIt] { ++pointerIt; };
			auto fooItIncrementPointer = [&fooIt] { ++fooIt; };

			Assert::ExpectException<std::exception>(intItIncrementPointer);
			Assert::ExpectException<std::exception>(stringItIncrementPointer);
			Assert::ExpectException<std::exception>(pointerItIncrementPointer);
			Assert::ExpectException<std::exception>(fooItIncrementPointer);
		}

		TEST_METHOD(VectorIteratorDereference)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			Vector<std::int32_t>::Iterator intIt = intVector.end();
			Vector<std::string>::Iterator stringIt = stringVector.end();
			Vector<std::int32_t*>::Iterator pointerIt = pointerVector.end();
			Vector<Foo>::Iterator fooIt = fooVector.end();

			auto intItDereferencePointer = [&intIt] { *intIt; };
			auto stringItDereferencePointer = [&stringIt] { *stringIt; };
			auto pointerItDereferencePointer = [&pointerIt] { *pointerIt; };
			auto fooItDereferencePointer = [&fooIt] { *fooIt; };

			Assert::ExpectException<std::exception>(intItDereferencePointer);
			Assert::ExpectException<std::exception>(stringItDereferencePointer);
			Assert::ExpectException<std::exception>(pointerItDereferencePointer);
			Assert::ExpectException<std::exception>(fooItDereferencePointer);

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			intVector.PushBack(randomIntData);
			stringVector.PushBack(randomString);
			pointerVector.PushBack(&randomIntData);
			fooVector.PushBack(randomFoo);

			Assert::AreEqual(randomIntData, *intVector.begin());
			Assert::AreEqual(randomString, *stringVector.begin());
			Assert::AreEqual(&randomIntData, *pointerVector.begin());
			Assert::IsTrue(randomFoo == *fooVector.begin());

			intIt = Vector<std::int32_t>::Iterator();
			stringIt = Vector<std::string>::Iterator();
			pointerIt = Vector<std::int32_t*>::Iterator();
			fooIt = Vector<Foo>::Iterator();

			Assert::ExpectException<std::exception>(intItDereferencePointer);
			Assert::ExpectException<std::exception>(stringItDereferencePointer);
			Assert::ExpectException<std::exception>(pointerItDereferencePointer);
			Assert::ExpectException<std::exception>(fooItDereferencePointer);

			const Vector<std::int32_t>::Iterator constIntIt = Vector<std::int32_t>::Iterator();
			const Vector<std::string>::Iterator constStringIt = Vector<std::string>::Iterator();
			const Vector<std::int32_t*>::Iterator constPointerIt = Vector<std::int32_t*>::Iterator();
			const Vector<Foo>::Iterator constFooIt = Vector<Foo>::Iterator();

			auto constIntItDereferencePointer = [&constIntIt] { *constIntIt; };
			auto constStringItDereferencePointer = [&constStringIt] { *constStringIt; };
			auto constPointerItDereferencePointer = [&constPointerIt] { *constPointerIt; };
			auto constFooItDereferencePointer = [&constFooIt] { *constFooIt; };

			Assert::ExpectException<std::exception>(constIntItDereferencePointer);
			Assert::ExpectException<std::exception>(constStringItDereferencePointer);
			Assert::ExpectException<std::exception>(constPointerItDereferencePointer);
			Assert::ExpectException<std::exception>(constFooItDereferencePointer);
		}

		TEST_METHOD(VectorIteratorCopyConstructor)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			Vector<std::int32_t>::Iterator intIt;
			Vector<std::string>::Iterator stringIt;
			Vector<std::int32_t*>::Iterator pointerIt;
			Vector<Foo>::Iterator fooIt;

			Vector<std::int32_t>::Iterator otherIntIt(intIt);
			Vector<std::string>::Iterator otherStringIt(stringIt);
			Vector<std::int32_t*>::Iterator otherPointerIt(pointerIt);
			Vector<Foo>::Iterator otherFooIt(fooIt);

			Assert::IsTrue(intIt == otherIntIt);
			Assert::IsTrue(stringIt == otherStringIt);
			Assert::IsTrue(pointerIt == otherPointerIt);
			Assert::IsTrue(fooIt == otherFooIt);

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			intVector.PushBack(randomIntData);
			stringVector.PushBack(randomString);
			pointerVector.PushBack(&randomIntData);
			fooVector.PushBack(randomFoo);

			intIt = intVector.begin();
			stringIt = stringVector.begin();
			pointerIt = pointerVector.begin();
			fooIt = fooVector.begin();

			Vector<std::int32_t>::Iterator thirdIntIt(intIt);
			Vector<std::string>::Iterator thirdStringIt(stringIt);
			Vector<std::int32_t*>::Iterator thirdPointerIt(pointerIt);
			Vector<Foo>::Iterator thirdFooIt(fooIt);

			Assert::IsTrue(intIt == thirdIntIt);
			Assert::IsTrue(stringIt == thirdStringIt);
			Assert::IsTrue(pointerIt == thirdPointerIt);
			Assert::IsTrue(fooIt == thirdFooIt);
		}

		TEST_METHOD(VectorIteratorAssignment)
		{
			Vector<std::int32_t> intVector;
			Vector<std::string> stringVector;
			Vector<std::int32_t*> pointerVector;
			Vector<Foo> fooVector;

			Vector<std::int32_t>::Iterator intIt;
			Vector<std::string>::Iterator stringIt;
			Vector<std::int32_t*>::Iterator pointerIt;
			Vector<Foo>::Iterator fooIt;

			std::int32_t randomIntData = rand();
			std::string randomString = std::string("howdy");
			Foo randomFoo = Foo(randomIntData);

			intVector.PushBack(randomIntData);
			stringVector.PushBack(randomString);
			pointerVector.PushBack(&randomIntData);
			fooVector.PushBack(randomFoo);

			intIt = intVector.begin();
			stringIt = stringVector.begin();
			pointerIt = pointerVector.begin();
			fooIt = fooVector.begin();

			Assert::IsTrue(intVector.begin() == intIt);
			Assert::IsTrue(stringVector.begin() == stringIt);
			Assert::IsTrue(pointerVector.begin() == pointerIt);
			Assert::IsTrue(fooVector.begin() == fooIt);

			intIt = intVector.end();
			stringIt = stringVector.end();
			pointerIt = pointerVector.end();
			fooIt = fooVector.end();

			Assert::IsTrue(intVector.end() == intIt);
			Assert::IsTrue(stringVector.end() == stringIt);
			Assert::IsTrue(pointerVector.end() == pointerIt);
			Assert::IsTrue(fooVector.end() == fooIt);
		}

	private:

		static _CrtMemState sStartMemState;
	};

	_CrtMemState VectorTest::sStartMemState;
}