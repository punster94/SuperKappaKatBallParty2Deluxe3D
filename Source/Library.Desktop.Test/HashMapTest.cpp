#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(HashMapTest)
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

		TEST_METHOD(HashMapConstructor)
		{
			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			Assert::AreEqual(mZero, intMap.Size());
			Assert::AreEqual(mZero, pointerMap.Size());
			Assert::AreEqual(mZero, charMap.Size());
			Assert::AreEqual(mZero, stringMap.Size());
			Assert::AreEqual(mZero, fooMap.Size());
		}

		TEST_METHOD(HashMapCopyConstructor)
		{
			std::int32_t randomInt = rand();
			char* randomChars = "howdy";
			std::string randomString = std::string(randomChars);
			Foo randomFoo = Foo(randomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			HashMap<std::int32_t, std::int32_t> otherIntMap(intMap);
			HashMap<std::int32_t*, std::int32_t> otherPointerMap(pointerMap);
			HashMap<char*, std::int32_t> otherCharMap(charMap);
			HashMap<std::string, std::int32_t> otherStringMap(stringMap);
			HashMap<Foo, std::int32_t, FooHashFunctor> otherFooMap(fooMap);

			Assert::AreEqual(intMap.Size(), otherIntMap.Size());
			Assert::AreEqual(pointerMap.Size(), otherPointerMap.Size());
			Assert::AreEqual(charMap.Size(), otherCharMap.Size());
			Assert::AreEqual(stringMap.Size(), otherStringMap.Size());
			Assert::AreEqual(fooMap.Size(), otherFooMap.Size());

			intMap[randomInt] = randomInt;
			pointerMap[&randomInt] = randomInt;
			charMap[randomChars] = randomInt;
			stringMap[randomString] = randomInt;
			fooMap[randomFoo] = randomInt;

			HashMap<std::int32_t, std::int32_t> thirdIntMap(intMap);
			HashMap<std::int32_t*, std::int32_t> thirdPointerMap(pointerMap);
			HashMap<char*, std::int32_t> thirdCharMap(charMap);
			HashMap<std::string, std::int32_t> thirdStringMap(stringMap);
			HashMap<Foo, std::int32_t, FooHashFunctor> thirdFooMap(fooMap);

			Assert::AreEqual(intMap.Size(), thirdIntMap.Size());
			Assert::AreEqual(pointerMap.Size(), thirdPointerMap.Size());
			Assert::AreEqual(charMap.Size(), thirdCharMap.Size());
			Assert::AreEqual(stringMap.Size(), thirdStringMap.Size());
			Assert::AreEqual(fooMap.Size(), thirdFooMap.Size());

			Assert::AreEqual(intMap[randomInt], thirdIntMap[randomInt]);
			Assert::AreEqual(pointerMap[&randomInt], thirdPointerMap[&randomInt]);
			Assert::AreEqual(charMap[randomChars], thirdCharMap[randomChars]);
			Assert::AreEqual(stringMap[randomString], thirdStringMap[randomString]);
			Assert::AreEqual(fooMap[randomFoo], thirdFooMap[randomFoo]);
		}

		TEST_METHOD(HashMapMoveConstructor)
		{
			std::int32_t randomInt = rand();
			char* randomChars = "howdy";
			std::string randomString = std::string(randomChars);
			Foo randomFoo = Foo(randomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			intMap[randomInt] = randomInt;
			pointerMap[&randomInt] = randomInt;
			charMap[randomChars] = randomInt;
			stringMap[randomString] = randomInt;
			fooMap[randomFoo] = randomInt;

			HashMap<std::int32_t, std::int32_t> otherIntMap = std::move(intMap);
			HashMap<std::int32_t*, std::int32_t> otherPointerMap = std::move(pointerMap);
			HashMap<char*, std::int32_t> otherCharMap = std::move(charMap);
			HashMap<std::string, std::int32_t> otherStringMap = std::move(stringMap);
			HashMap<Foo, std::int32_t, FooHashFunctor> otherFooMap = std::move(fooMap);

			Assert::AreEqual(0U, intMap.Size());
			Assert::AreEqual(0U, pointerMap.Size());
			Assert::AreEqual(0U, charMap.Size());
			Assert::AreEqual(0U, stringMap.Size());
			Assert::AreEqual(0U, fooMap.Size());

			Assert::AreEqual(randomInt, otherIntMap[randomInt]);
			Assert::AreEqual(randomInt, otherPointerMap[&randomInt]);
			Assert::AreEqual(randomInt, otherCharMap[randomChars]);
			Assert::AreEqual(randomInt, otherStringMap[randomString]);
			Assert::AreEqual(randomInt, otherFooMap[randomFoo]);
		}

		TEST_METHOD(HashMapMoveAssignmentOperator)
		{
			std::int32_t randomInt = rand();
			char* randomChars = "howdy";
			std::string randomString = std::string(randomChars);
			Foo randomFoo = Foo(randomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			intMap[randomInt] = randomInt;
			pointerMap[&randomInt] = randomInt;
			charMap[randomChars] = randomInt;
			stringMap[randomString] = randomInt;
			fooMap[randomFoo] = randomInt;

			HashMap<std::int32_t, std::int32_t> otherIntMap;
			HashMap<std::int32_t*, std::int32_t> otherPointerMap;
			HashMap<char*, std::int32_t> otherCharMap;
			HashMap<std::string, std::int32_t> otherStringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> otherFooMap;

			otherIntMap = std::move(intMap);
			otherPointerMap = std::move(pointerMap);
			otherCharMap = std::move(charMap);
			otherStringMap = std::move(stringMap);
			otherFooMap = std::move(fooMap);

			Assert::AreEqual(0U, intMap.Size());
			Assert::AreEqual(0U, pointerMap.Size());
			Assert::AreEqual(0U, charMap.Size());
			Assert::AreEqual(0U, stringMap.Size());
			Assert::AreEqual(0U, fooMap.Size());

			Assert::AreEqual(randomInt, otherIntMap[randomInt]);
			Assert::AreEqual(randomInt, otherPointerMap[&randomInt]);
			Assert::AreEqual(randomInt, otherCharMap[randomChars]);
			Assert::AreEqual(randomInt, otherStringMap[randomString]);
			Assert::AreEqual(randomInt, otherFooMap[randomFoo]);
		}

		TEST_METHOD(HashMapDestructor)
		{
			std::int32_t randomInt = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			char* randomChars = "howdy", *nextRandomChars = "hi", *thirdRandomChars = "hello";
			std::string randomString = std::string(randomChars), nextRandomString = std::string(nextRandomChars), thirdRandomString = std::string(thirdRandomChars);
			Foo randomFoo = Foo(randomInt), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			intMap[randomInt] = randomInt;
			intMap[nextRandomInt] = nextRandomInt;
			intMap[thirdRandomInt] = thirdRandomInt;

			pointerMap[&randomInt] = randomInt;
			pointerMap[&nextRandomInt] = nextRandomInt;
			pointerMap[&thirdRandomInt] = thirdRandomInt;

			charMap[randomChars] = randomInt;
			charMap[nextRandomChars] = nextRandomInt;
			charMap[thirdRandomChars] = thirdRandomInt;

			stringMap[randomString] = randomInt;
			stringMap[nextRandomString] = nextRandomInt;
			stringMap[thirdRandomString] = thirdRandomInt;

			fooMap[randomFoo] = randomInt;
			fooMap[nextRandomFoo] = nextRandomInt;
			fooMap[thirdRandomFoo] = thirdRandomInt;
		}

		TEST_METHOD(HashMapAssignmentOperator)
		{
			std::int32_t randomInt = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			char* randomChars = "howdy", *nextRandomChars = "hi", *thirdRandomChars = "hello";
			std::string randomString = std::string(randomChars), nextRandomString = std::string(nextRandomChars), thirdRandomString = std::string(thirdRandomChars);
			Foo randomFoo = Foo(randomInt), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			HashMap<std::int32_t, std::int32_t> intMap(mBucketCount);
			HashMap<std::int32_t*, std::int32_t> pointerMap(mBucketCount);
			HashMap<char*, std::int32_t> charMap(mBucketCount);
			HashMap<std::string, std::int32_t> stringMap(mBucketCount);
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap(mBucketCount);

			HashMap<std::int32_t, std::int32_t> otherIntMap(mOtherBucketCount);
			HashMap<std::int32_t*, std::int32_t> otherPointerMap(mOtherBucketCount);
			HashMap<char*, std::int32_t> otherCharMap(mOtherBucketCount);
			HashMap<std::string, std::int32_t> otherStringMap(mOtherBucketCount);
			HashMap<Foo, std::int32_t, FooHashFunctor> otherFooMap(mOtherBucketCount);

			otherIntMap = intMap;
			otherPointerMap = pointerMap;
			otherCharMap = charMap;
			otherStringMap = stringMap;
			otherFooMap = fooMap;

			Assert::AreEqual(intMap.Size(), otherIntMap.Size());
			Assert::AreEqual(pointerMap.Size(), otherPointerMap.Size());
			Assert::AreEqual(charMap.Size(), otherCharMap.Size());
			Assert::AreEqual(stringMap.Size(), otherStringMap.Size());
			Assert::AreEqual(fooMap.Size(), otherFooMap.Size());

			intMap[randomInt] = randomInt;
			intMap[nextRandomInt] = nextRandomInt;
			intMap[thirdRandomInt] = thirdRandomInt;

			pointerMap[&randomInt] = randomInt;
			pointerMap[&nextRandomInt] = nextRandomInt;
			pointerMap[&thirdRandomInt] = thirdRandomInt;

			charMap[randomChars] = randomInt;
			charMap[nextRandomChars] = nextRandomInt;
			charMap[thirdRandomChars] = thirdRandomInt;

			stringMap[randomString] = randomInt;
			stringMap[nextRandomString] = nextRandomInt;
			stringMap[thirdRandomString] = thirdRandomInt;

			fooMap[randomFoo] = randomInt;
			fooMap[nextRandomFoo] = nextRandomInt;
			fooMap[thirdRandomFoo] = thirdRandomInt;

			otherIntMap = intMap;
			otherPointerMap = pointerMap;
			otherCharMap = charMap;
			otherStringMap = stringMap;
			otherFooMap = fooMap;

			Assert::AreEqual(intMap.Size(), otherIntMap.Size());
			Assert::AreEqual(pointerMap.Size(), otherPointerMap.Size());
			Assert::AreEqual(charMap.Size(), otherCharMap.Size());
			Assert::AreEqual(stringMap.Size(), otherStringMap.Size());
			Assert::AreEqual(fooMap.Size(), otherFooMap.Size());

			Assert::AreEqual(intMap[randomInt], otherIntMap[randomInt]);
			Assert::AreEqual(intMap[nextRandomInt], otherIntMap[nextRandomInt]);
			Assert::AreEqual(intMap[thirdRandomInt], otherIntMap[thirdRandomInt]);

			Assert::AreEqual(pointerMap[&randomInt], otherPointerMap[&randomInt]);
			Assert::AreEqual(pointerMap[&nextRandomInt], otherPointerMap[&nextRandomInt]);
			Assert::AreEqual(pointerMap[&thirdRandomInt], otherPointerMap[&thirdRandomInt]);

			Assert::AreEqual(charMap[randomChars], otherCharMap[randomChars]);
			Assert::AreEqual(charMap[nextRandomChars], otherCharMap[nextRandomChars]);
			Assert::AreEqual(charMap[thirdRandomChars], otherCharMap[thirdRandomChars]);

			Assert::AreEqual(stringMap[randomString], otherStringMap[randomString]);
			Assert::AreEqual(stringMap[nextRandomString], otherStringMap[nextRandomString]);
			Assert::AreEqual(stringMap[thirdRandomString], otherStringMap[thirdRandomString]);

			Assert::AreEqual(fooMap[randomFoo], otherFooMap[randomFoo]);
			Assert::AreEqual(fooMap[nextRandomFoo], otherFooMap[nextRandomFoo]);
			Assert::AreEqual(fooMap[thirdRandomFoo], otherFooMap[thirdRandomFoo]);
		}

		TEST_METHOD(HashMapFind)
		{
			std::int32_t randomInt = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			char* randomChars = "howdy", *nextRandomChars = "hi", *thirdRandomChars = "hello";
			std::string randomString = std::string(randomChars), nextRandomString = std::string(nextRandomChars), thirdRandomString = std::string(thirdRandomChars);
			Foo randomFoo = Foo(randomInt), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			Assert::IsTrue(intMap.end() == intMap.Find(randomInt));
			Assert::IsTrue(pointerMap.end() == pointerMap.Find(&randomInt));
			Assert::IsTrue(charMap.end() == charMap.Find(randomChars));
			Assert::IsTrue(stringMap.end() == stringMap.Find(randomString));
			Assert::IsTrue(fooMap.end() == fooMap.Find(randomFoo));
			
			intMap[randomInt] = randomInt;
			intMap[nextRandomInt] = nextRandomInt;
			intMap[thirdRandomInt] = thirdRandomInt;

			pointerMap[&randomInt] = randomInt;
			pointerMap[&nextRandomInt] = nextRandomInt;
			pointerMap[&thirdRandomInt] = thirdRandomInt;

			charMap[randomChars] = randomInt;
			charMap[nextRandomChars] = nextRandomInt;
			charMap[thirdRandomChars] = thirdRandomInt;

			stringMap[randomString] = randomInt;
			stringMap[nextRandomString] = nextRandomInt;
			stringMap[thirdRandomString] = thirdRandomInt;

			fooMap[randomFoo] = randomInt;
			fooMap[nextRandomFoo] = nextRandomInt;
			fooMap[thirdRandomFoo] = thirdRandomInt;

			Assert::IsTrue(intMap.end() != intMap.Find(randomInt));
			Assert::IsTrue(intMap.end() != intMap.Find(nextRandomInt));
			Assert::IsTrue(intMap.end() != intMap.Find(thirdRandomInt));
			Assert::AreEqual(randomInt, (*(intMap.Find(randomInt))).second);

			Assert::IsTrue(pointerMap.end() != pointerMap.Find(&randomInt));
			Assert::IsTrue(pointerMap.end() != pointerMap.Find(&nextRandomInt));
			Assert::IsTrue(pointerMap.end() != pointerMap.Find(&thirdRandomInt));
			Assert::AreEqual(randomInt, (*(pointerMap.Find(&randomInt))).second);

			Assert::IsTrue(charMap.end() != charMap.Find(randomChars));
			Assert::IsTrue(charMap.end() != charMap.Find(nextRandomChars));
			Assert::IsTrue(charMap.end() != charMap.Find(thirdRandomChars));
			Assert::AreEqual(randomInt, (*(charMap.Find(randomChars))).second);

			Assert::IsTrue(stringMap.end() != stringMap.Find(randomString));
			Assert::IsTrue(stringMap.end() != stringMap.Find(nextRandomString));
			Assert::IsTrue(stringMap.end() != stringMap.Find(thirdRandomString));
			Assert::AreEqual(randomInt, (*(stringMap.Find(randomString))).second);

			Assert::IsTrue(fooMap.end() != fooMap.Find(randomFoo));
			Assert::IsTrue(fooMap.end() != fooMap.Find(nextRandomFoo));
			Assert::IsTrue(fooMap.end() != fooMap.Find(thirdRandomFoo));
			Assert::AreEqual(randomInt, (*(fooMap.Find(randomFoo))).second);
		}

		TEST_METHOD(HashMapInsert)
		{
			std::int32_t randomInt = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			char* randomChars = "howdy", *nextRandomChars = "hi", *thirdRandomChars = "hello";
			std::string randomString = std::string(randomChars), nextRandomString = std::string(nextRandomChars), thirdRandomString = std::string(thirdRandomChars);
			Foo randomFoo = Foo(randomInt), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			std::pair<std::int32_t, std::int32_t> intPair(randomInt, randomInt);
			std::pair<std::int32_t, std::int32_t> nextIntPair(nextRandomInt, nextRandomInt);
			std::pair<std::int32_t, std::int32_t> thirdIntPair(thirdRandomInt, thirdRandomInt);

			std::pair<std::int32_t*, std::int32_t> pointerPair(&randomInt, randomInt);
			std::pair<std::int32_t*, std::int32_t> nextPointerPair(&nextRandomInt, nextRandomInt);
			std::pair<std::int32_t*, std::int32_t> thirdPointerPair(&thirdRandomInt, thirdRandomInt);

			std::pair<char*, std::int32_t> charsPair(randomChars, randomInt);
			std::pair<char*, std::int32_t> nextCharsPair(nextRandomChars, nextRandomInt);
			std::pair<char*, std::int32_t> thirdCharsPair(thirdRandomChars, thirdRandomInt);

			std::pair<std::string, std::int32_t> stringPair(randomString, randomInt);
			std::pair<std::string, std::int32_t> nextStringPair(nextRandomString, nextRandomInt);
			std::pair<std::string, std::int32_t> thirdStringPair(thirdRandomString, thirdRandomInt);

			std::pair<Foo, std::int32_t> fooPair(randomFoo, randomInt);
			std::pair<Foo, std::int32_t> nextFooPair(nextRandomFoo, nextRandomInt);
			std::pair<Foo, std::int32_t> thirdFooPair(thirdRandomFoo, thirdRandomInt);

			std::uint32_t size = intMap.Size();

			++size;

			Assert::IsTrue((*(intMap.Insert(intPair))).second == randomInt);
			Assert::AreEqual(size, intMap.Size());

			Assert::IsTrue((*(pointerMap.Insert(pointerPair))).second == randomInt);
			Assert::AreEqual(size, pointerMap.Size());

			Assert::IsTrue((*(charMap.Insert(charsPair))).second == randomInt);
			Assert::AreEqual(size, charMap.Size());

			Assert::IsTrue((*(stringMap.Insert(stringPair))).second == randomInt);
			Assert::AreEqual(size, stringMap.Size());

			Assert::IsTrue((*(fooMap.Insert(fooPair))).second == randomInt);
			Assert::AreEqual(size, fooMap.Size());

			++size;

			Assert::IsTrue((*(intMap.Insert(nextIntPair))).second == nextRandomInt);
			Assert::AreEqual(size, intMap.Size());

			Assert::IsTrue((*(pointerMap.Insert(nextPointerPair))).second == nextRandomInt);
			Assert::AreEqual(size, pointerMap.Size());

			Assert::IsTrue((*(charMap.Insert(nextCharsPair))).second == nextRandomInt);
			Assert::AreEqual(size, charMap.Size());

			Assert::IsTrue((*(stringMap.Insert(nextStringPair))).second == nextRandomInt);
			Assert::AreEqual(size, stringMap.Size());

			Assert::IsTrue((*(fooMap.Insert(nextFooPair))).second == nextRandomInt);
			Assert::AreEqual(size, fooMap.Size());

			++size;

			Assert::IsTrue((*(intMap.Insert(thirdIntPair))).second == thirdRandomInt);
			Assert::AreEqual(size, intMap.Size());

			Assert::IsTrue((*(pointerMap.Insert(thirdPointerPair))).second == thirdRandomInt);
			Assert::AreEqual(size, pointerMap.Size());

			Assert::IsTrue((*(charMap.Insert(thirdCharsPair))).second == thirdRandomInt);
			Assert::AreEqual(size, charMap.Size());

			Assert::IsTrue((*(stringMap.Insert(thirdStringPair))).second == thirdRandomInt);
			Assert::AreEqual(size, stringMap.Size());

			Assert::IsTrue((*(fooMap.Insert(thirdFooPair))).second == thirdRandomInt);
			Assert::AreEqual(size, fooMap.Size());

			intMap.Insert(intPair);
			pointerMap.Insert(pointerPair);
			charMap.Insert(charsPair);
			stringMap.Insert(stringPair);
			fooMap.Insert(fooPair);

			Assert::AreEqual(size, intMap.Size());
			Assert::AreEqual(size, pointerMap.Size());
			Assert::AreEqual(size, charMap.Size());
			Assert::AreEqual(size, stringMap.Size());
			Assert::AreEqual(size, fooMap.Size());
		}

		TEST_METHOD(HashMapAccessorOperator)
		{
			std::int32_t randomInt = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			char* randomChars = "howdy", *nextRandomChars = "hi", *thirdRandomChars = "hello";
			std::string randomString = std::string(randomChars), nextRandomString = std::string(nextRandomChars), thirdRandomString = std::string(thirdRandomChars);
			Foo randomFoo = Foo(randomInt), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			std::uint32_t size = intMap.Size();

			intMap[randomInt];
			pointerMap[&randomInt];
			charMap[randomChars];
			stringMap[randomString];
			fooMap[randomFoo];

			++size;

			Assert::AreEqual(size, intMap.Size());
			Assert::AreEqual(size, pointerMap.Size());
			Assert::AreEqual(size, charMap.Size());
			Assert::AreEqual(size, stringMap.Size());
			Assert::AreEqual(size, fooMap.Size());

			intMap[randomInt] = randomInt;
			intMap[nextRandomInt] = nextRandomInt;

			pointerMap[&randomInt] = randomInt;
			pointerMap[&nextRandomInt] = nextRandomInt;

			charMap[randomChars] = randomInt;
			charMap[nextRandomChars] = nextRandomInt;

			stringMap[randomString] = randomInt;
			stringMap[nextRandomString] = nextRandomInt;

			fooMap[randomFoo] = randomInt;
			fooMap[nextRandomFoo] = nextRandomInt;

			Assert::IsTrue(intMap.ContainsKey(randomInt));
			Assert::IsTrue(intMap.ContainsKey(nextRandomInt));
			Assert::IsFalse(intMap.ContainsKey(thirdRandomInt));

			Assert::IsTrue(pointerMap.ContainsKey(&randomInt));
			Assert::IsTrue(pointerMap.ContainsKey(&nextRandomInt));
			Assert::IsFalse(pointerMap.ContainsKey(&thirdRandomInt));

			Assert::IsTrue(charMap.ContainsKey(randomChars));
			Assert::IsTrue(charMap.ContainsKey(nextRandomChars));
			Assert::IsFalse(charMap.ContainsKey(thirdRandomChars));

			Assert::IsTrue(stringMap.ContainsKey(randomString));
			Assert::IsTrue(stringMap.ContainsKey(nextRandomString));
			Assert::IsFalse(stringMap.ContainsKey(thirdRandomString));

			Assert::IsTrue(fooMap.ContainsKey(randomFoo));
			Assert::IsTrue(fooMap.ContainsKey(nextRandomFoo));
			Assert::IsFalse(fooMap.ContainsKey(thirdRandomFoo));

			const HashMap<std::int32_t, std::int32_t> constIntMap(intMap);
			const HashMap<std::int32_t*, std::int32_t> constPointerMap(pointerMap);
			const HashMap<char*, std::int32_t> constCharMap(charMap);
			const HashMap<std::string, std::int32_t> constStringMap(stringMap);
			const HashMap<Foo, std::int32_t, FooHashFunctor> constFooMap(fooMap);

			auto constIntMapAccessorPointer = [&constIntMap, &thirdRandomInt] { constIntMap.operator[](thirdRandomInt); };
			auto constPointerMapAccessorPointer = [&constPointerMap, &thirdRandomInt] { constPointerMap.operator[](&thirdRandomInt); };
			auto constCharMapAccessorPointer = [&constCharMap, &thirdRandomChars] { constCharMap.operator[](thirdRandomChars); };
			auto constStringMapAccessorPointer = [&constStringMap, &thirdRandomString] { constStringMap.operator[](thirdRandomString); };
			auto constFooMapAccessorPointer = [&constFooMap, &thirdRandomFoo] { constFooMap.operator[](thirdRandomFoo); };

			Assert::ExpectException<std::exception>(constIntMapAccessorPointer);
			Assert::ExpectException<std::exception>(constPointerMapAccessorPointer);
			Assert::ExpectException<std::exception>(constCharMapAccessorPointer);
			Assert::ExpectException<std::exception>(constStringMapAccessorPointer);
			Assert::ExpectException<std::exception>(constFooMapAccessorPointer);

			Assert::IsTrue(randomInt == constIntMap[randomInt]);
			Assert::IsTrue(randomInt == constPointerMap[&randomInt]);
			Assert::IsTrue(randomInt == constCharMap[randomChars]);
			Assert::IsTrue(randomInt == constStringMap[randomString]);
			Assert::IsTrue(randomInt == constFooMap[randomFoo]);
		}

		TEST_METHOD(HashMapRemove)
		{
			std::int32_t randomInt = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			char* randomChars = "howdy", *nextRandomChars = "hi", *thirdRandomChars = "hello";
			std::string randomString = std::string(randomChars), nextRandomString = std::string(nextRandomChars), thirdRandomString = std::string(thirdRandomChars);
			Foo randomFoo = Foo(randomInt), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			intMap[randomInt] = randomInt;
			intMap[nextRandomInt] = nextRandomInt;

			pointerMap[&randomInt] = randomInt;
			pointerMap[&nextRandomInt] = nextRandomInt;

			charMap[randomChars] = randomInt;
			charMap[nextRandomChars] = nextRandomInt;

			stringMap[randomString] = randomInt;
			stringMap[nextRandomString] = nextRandomInt;

			fooMap[randomFoo] = randomInt;
			fooMap[nextRandomFoo] = nextRandomInt;
			
			std::uint32_t size = intMap.Size();

			intMap.Remove(thirdRandomInt);
			pointerMap.Remove(&thirdRandomInt);
			charMap.Remove(thirdRandomChars);
			stringMap.Remove(thirdRandomString);
			fooMap.Remove(thirdRandomFoo);

			Assert::AreEqual(size, intMap.Size());
			Assert::AreEqual(size, pointerMap.Size());
			Assert::AreEqual(size, charMap.Size());
			Assert::AreEqual(size, stringMap.Size());
			Assert::AreEqual(size, fooMap.Size());

			intMap.Remove(randomInt);
			pointerMap.Remove(&randomInt);
			charMap.Remove(randomChars);
			stringMap.Remove(randomString);
			fooMap.Remove(randomFoo);

			--size;

			Assert::AreEqual(size, intMap.Size());
			Assert::AreEqual(size, pointerMap.Size());
			Assert::AreEqual(size, charMap.Size());
			Assert::AreEqual(size, stringMap.Size());
			Assert::AreEqual(size, fooMap.Size());

			Assert::IsTrue(intMap.end() == intMap.Find(randomInt));
			Assert::IsTrue(pointerMap.end() == pointerMap.Find(&randomInt));
			Assert::IsTrue(charMap.end() == charMap.Find(randomChars));
			Assert::IsTrue(stringMap.end() == stringMap.Find(randomString));
			Assert::IsTrue(fooMap.end() == fooMap.Find(randomFoo));
		}

		TEST_METHOD(HashMapClear)
		{
			std::int32_t randomInt = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			char* randomChars = "howdy", *nextRandomChars = "hi", *thirdRandomChars = "hello";
			std::string randomString = std::string(randomChars), nextRandomString = std::string(nextRandomChars), thirdRandomString = std::string(thirdRandomChars);
			Foo randomFoo = Foo(randomInt), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			std::uint32_t size = intMap.Size();

			intMap[randomInt] = randomInt;
			intMap[nextRandomInt] = nextRandomInt;
			intMap[thirdRandomInt] = thirdRandomInt;

			pointerMap[&randomInt] = randomInt;
			pointerMap[&nextRandomInt] = nextRandomInt;
			pointerMap[&thirdRandomInt] = thirdRandomInt;

			charMap[randomChars] = randomInt;
			charMap[nextRandomChars] = nextRandomInt;
			charMap[thirdRandomChars] = thirdRandomInt;

			stringMap[randomString] = randomInt;
			stringMap[nextRandomString] = nextRandomInt;
			stringMap[thirdRandomString] = thirdRandomInt;

			fooMap[randomFoo] = randomInt;
			fooMap[nextRandomFoo] = nextRandomInt;
			fooMap[thirdRandomFoo] = thirdRandomInt;

			intMap.Clear();
			pointerMap.Clear();
			charMap.Clear();
			stringMap.Clear();
			fooMap.Clear();

			Assert::AreEqual(size, intMap.Size());
			Assert::AreEqual(size, pointerMap.Size());
			Assert::AreEqual(size, charMap.Size());
			Assert::AreEqual(size, stringMap.Size());
			Assert::AreEqual(size, fooMap.Size());
		}

		TEST_METHOD(HashMapSize)
		{
			std::int32_t randomInt = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			char* randomChars = "howdy", *nextRandomChars = "hi", *thirdRandomChars = "hello";
			std::string randomString = std::string(randomChars), nextRandomString = std::string(nextRandomChars), thirdRandomString = std::string(thirdRandomChars);
			Foo randomFoo = Foo(randomInt), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			std::uint32_t size = 0;

			Assert::AreEqual(size, intMap.Size());
			Assert::AreEqual(size, pointerMap.Size());
			Assert::AreEqual(size, charMap.Size());
			Assert::AreEqual(size, stringMap.Size());
			Assert::AreEqual(size, fooMap.Size());

			intMap[randomInt] = randomInt;
			intMap[nextRandomInt] = nextRandomInt;
			intMap[thirdRandomInt] = thirdRandomInt;

			pointerMap[&randomInt] = randomInt;
			pointerMap[&nextRandomInt] = nextRandomInt;
			pointerMap[&thirdRandomInt] = thirdRandomInt;

			charMap[randomChars] = randomInt;
			charMap[nextRandomChars] = nextRandomInt;
			charMap[thirdRandomChars] = thirdRandomInt;

			stringMap[randomString] = randomInt;
			stringMap[nextRandomString] = nextRandomInt;
			stringMap[thirdRandomString] = thirdRandomInt;

			fooMap[randomFoo] = randomInt;
			fooMap[nextRandomFoo] = nextRandomInt;
			fooMap[thirdRandomFoo] = thirdRandomInt;

			size += 3;

			Assert::AreEqual(size, intMap.Size());
			Assert::AreEqual(size, pointerMap.Size());
			Assert::AreEqual(size, charMap.Size());
			Assert::AreEqual(size, stringMap.Size());
			Assert::AreEqual(size, fooMap.Size());

			intMap.Clear();
			pointerMap.Clear();
			charMap.Clear();
			stringMap.Clear();
			fooMap.Clear();

			size = 0;

			Assert::AreEqual(size, intMap.Size());
			Assert::AreEqual(size, pointerMap.Size());
			Assert::AreEqual(size, charMap.Size());
			Assert::AreEqual(size, stringMap.Size());
			Assert::AreEqual(size, fooMap.Size());
		}

		TEST_METHOD(HashMapContainsKey)
		{
			std::int32_t randomInt = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			char* randomChars = "howdy", *nextRandomChars = "hi", *thirdRandomChars = "hello";
			std::string randomString = std::string(randomChars), nextRandomString = std::string(nextRandomChars), thirdRandomString = std::string(thirdRandomChars);
			Foo randomFoo = Foo(randomInt), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			Assert::IsFalse(intMap.ContainsKey(randomInt));
			Assert::IsFalse(pointerMap.ContainsKey(&randomInt));
			Assert::IsFalse(charMap.ContainsKey(randomChars));
			Assert::IsFalse(stringMap.ContainsKey(randomString));
			Assert::IsFalse(fooMap.ContainsKey(randomFoo));

			intMap[randomInt] = randomInt;
			intMap[nextRandomInt] = nextRandomInt;
			intMap[thirdRandomInt] = thirdRandomInt;

			pointerMap[&randomInt] = randomInt;
			pointerMap[&nextRandomInt] = nextRandomInt;
			pointerMap[&thirdRandomInt] = thirdRandomInt;

			charMap[randomChars] = randomInt;
			charMap[nextRandomChars] = nextRandomInt;
			charMap[thirdRandomChars] = thirdRandomInt;

			stringMap[randomString] = randomInt;
			stringMap[nextRandomString] = nextRandomInt;
			stringMap[thirdRandomString] = thirdRandomInt;

			fooMap[randomFoo] = randomInt;
			fooMap[nextRandomFoo] = nextRandomInt;
			fooMap[thirdRandomFoo] = thirdRandomInt;

			Assert::IsTrue(intMap.ContainsKey(randomInt));
			Assert::IsTrue(intMap.ContainsKey(nextRandomInt));
			Assert::IsTrue(intMap.ContainsKey(thirdRandomInt));

			Assert::IsTrue(pointerMap.ContainsKey(&randomInt));
			Assert::IsTrue(pointerMap.ContainsKey(&nextRandomInt));
			Assert::IsTrue(pointerMap.ContainsKey(&thirdRandomInt));

			Assert::IsTrue(charMap.ContainsKey(randomChars));
			Assert::IsTrue(charMap.ContainsKey(nextRandomChars));
			Assert::IsTrue(charMap.ContainsKey(thirdRandomChars));

			Assert::IsTrue(stringMap.ContainsKey(randomString));
			Assert::IsTrue(stringMap.ContainsKey(nextRandomString));
			Assert::IsTrue(stringMap.ContainsKey(thirdRandomString));

			Assert::IsTrue(fooMap.ContainsKey(randomFoo));
			Assert::IsTrue(fooMap.ContainsKey(nextRandomFoo));
			Assert::IsTrue(fooMap.ContainsKey(thirdRandomFoo));

			intMap.Remove(randomInt);
			pointerMap.Remove(&randomInt);
			charMap.Remove(randomChars);
			stringMap.Remove(randomString);
			fooMap.Remove(randomFoo);

			Assert::IsFalse(intMap.ContainsKey(randomInt));
			Assert::IsTrue(intMap.ContainsKey(nextRandomInt));
			Assert::IsTrue(intMap.ContainsKey(thirdRandomInt));

			Assert::IsFalse(pointerMap.ContainsKey(&randomInt));
			Assert::IsTrue(pointerMap.ContainsKey(&nextRandomInt));
			Assert::IsTrue(pointerMap.ContainsKey(&thirdRandomInt));

			Assert::IsFalse(charMap.ContainsKey(randomChars));
			Assert::IsTrue(charMap.ContainsKey(nextRandomChars));
			Assert::IsTrue(charMap.ContainsKey(thirdRandomChars));

			Assert::IsFalse(stringMap.ContainsKey(randomString));
			Assert::IsTrue(stringMap.ContainsKey(nextRandomString));
			Assert::IsTrue(stringMap.ContainsKey(thirdRandomString));

			Assert::IsFalse(fooMap.ContainsKey(randomFoo));
			Assert::IsTrue(fooMap.ContainsKey(nextRandomFoo));
			Assert::IsTrue(fooMap.ContainsKey(thirdRandomFoo));
		}

		TEST_METHOD(HashMapBegin)
		{
			std::int32_t randomInt = rand();
			char* randomChars = "howdy";
			std::string randomString = std::string(randomChars);
			Foo randomFoo = Foo(randomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			Assert::IsTrue(intMap.end() == intMap.begin());
			Assert::IsTrue(pointerMap.end() == pointerMap.begin());
			Assert::IsTrue(charMap.end() == charMap.begin());
			Assert::IsTrue(stringMap.end() == stringMap.begin());
			Assert::IsTrue(fooMap.end() == fooMap.begin());

			intMap[randomInt] = randomInt;
			pointerMap[&randomInt] = randomInt;
			charMap[randomChars] = randomInt;
			stringMap[randomString] = randomInt;
			fooMap[randomFoo] = randomInt;

			Assert::IsTrue(intMap.end() != intMap.begin());
			Assert::IsTrue(pointerMap.end() != pointerMap.begin());
			Assert::IsTrue(charMap.end() != charMap.begin());
			Assert::IsTrue(stringMap.end() != stringMap.begin());
			Assert::IsTrue(fooMap.end() != fooMap.begin());

			Assert::AreEqual(randomInt, (*(intMap.begin())).second);
			Assert::AreEqual(randomInt, (*(pointerMap.begin())).second);
			Assert::AreEqual(randomInt, (*(charMap.begin())).second);
			Assert::AreEqual(randomInt, (*(stringMap.begin())).second);
			Assert::AreEqual(randomInt, (*(fooMap.begin())).second);
		}

		TEST_METHOD(HashMapEnd)
		{
			std::int32_t randomInt = rand();
			char* randomChars = "howdy";
			std::string randomString = std::string(randomChars);
			Foo randomFoo = Foo(randomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			HashMap<std::int32_t, std::int32_t>::Iterator intIt = intMap.end();
			HashMap<std::int32_t*, std::int32_t>::Iterator pointerIt = pointerMap.end();
			HashMap<char*, std::int32_t>::Iterator charIt = charMap.end();
			HashMap<std::string, std::int32_t>::Iterator stringIt = stringMap.end();
			HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator fooIt = fooMap.end();

			Assert::IsTrue(intIt == intMap.begin());
			Assert::IsTrue(pointerIt == pointerMap.begin());
			Assert::IsTrue(charIt == charMap.begin());
			Assert::IsTrue(stringIt == stringMap.begin());
			Assert::IsTrue(fooIt == fooMap.begin());

			intMap[randomInt] = randomInt;
			pointerMap[&randomInt] = randomInt;
			charMap[randomChars] = randomInt;
			stringMap[randomString] = randomInt;
			fooMap[randomFoo] = randomInt;

			Assert::IsTrue(intIt == intMap.end());
			Assert::IsTrue(pointerIt == pointerMap.end());
			Assert::IsTrue(charIt == charMap.end());
			Assert::IsTrue(stringIt == stringMap.end());
			Assert::IsTrue(fooIt == fooMap.end());

			Assert::IsTrue(intMap.end() != intMap.begin());
			Assert::IsTrue(pointerMap.end() != pointerMap.begin());
			Assert::IsTrue(charMap.end() != charMap.begin());
			Assert::IsTrue(stringMap.end() != stringMap.begin());
			Assert::IsTrue(fooMap.end() != fooMap.begin());
		}

		TEST_METHOD(HashMapIteratorDefaultConstructor)
		{
			HashMap<std::int32_t, std::int32_t>::Iterator intIt;
			HashMap<std::int32_t*, std::int32_t>::Iterator pointerIt;
			HashMap<char*, std::int32_t>::Iterator charIt;
			HashMap<std::string, std::int32_t>::Iterator stringIt;
			HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator fooIt;

			HashMap<std::int32_t, std::int32_t>::Iterator otherIntIt;
			HashMap<std::int32_t*, std::int32_t>::Iterator otherPointerIt;
			HashMap<char*, std::int32_t>::Iterator otherCharIt;
			HashMap<std::string, std::int32_t>::Iterator otherStringIt;
			HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator otherFooIt;

			Assert::IsTrue(intIt == otherIntIt);
			Assert::IsTrue(pointerIt == otherPointerIt);
			Assert::IsTrue(charIt == otherCharIt);
			Assert::IsTrue(stringIt == otherStringIt);
			Assert::IsTrue(fooIt == otherFooIt);
		}

		TEST_METHOD(HashMapIteratorCopyConstructor)
		{
			std::int32_t randomInt = rand();
			char* randomChars = "howdy";
			std::string randomString = std::string(randomChars);
			Foo randomFoo = Foo(randomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			intMap[randomInt] = randomInt;
			pointerMap[&randomInt] = randomInt;
			charMap[randomChars] = randomInt;
			stringMap[randomString] = randomInt;
			fooMap[randomFoo] = randomInt;

			HashMap<std::int32_t, std::int32_t>::Iterator intIt = intMap.begin();
			HashMap<std::int32_t*, std::int32_t>::Iterator pointerIt = pointerMap.begin();
			HashMap<char*, std::int32_t>::Iterator charIt = charMap.begin();
			HashMap<std::string, std::int32_t>::Iterator stringIt = stringMap.begin();
			HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator fooIt = fooMap.begin();

			HashMap<std::int32_t, std::int32_t>::Iterator otherIntIt(intIt);
			HashMap<std::int32_t*, std::int32_t>::Iterator otherPointerIt(pointerIt);
			HashMap<char*, std::int32_t>::Iterator otherCharIt(charIt);
			HashMap<std::string, std::int32_t>::Iterator otherStringIt(stringIt);
			HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator otherFooIt(fooIt);

			Assert::IsTrue(intIt == otherIntIt);
			Assert::IsTrue(pointerIt == otherPointerIt);
			Assert::IsTrue(charIt == otherCharIt);
			Assert::IsTrue(stringIt == otherStringIt);
			Assert::IsTrue(fooIt == otherFooIt);
		}

		TEST_METHOD(HashMapIteratorPostIncrement)
		{
			std::int32_t randomInt = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			char* randomChars = "howdy", *nextRandomChars = "hi", *thirdRandomChars = "hello";
			std::string randomString = std::string(randomChars), nextRandomString = std::string(nextRandomChars), thirdRandomString = std::string(thirdRandomChars);
			Foo randomFoo = Foo(randomInt), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			intMap[randomInt] = randomInt;
			intMap[nextRandomInt] = nextRandomInt;
			intMap[thirdRandomInt] = thirdRandomInt;

			pointerMap[&randomInt] = randomInt;
			pointerMap[&nextRandomInt] = nextRandomInt;
			pointerMap[&thirdRandomInt] = thirdRandomInt;

			charMap[randomChars] = randomInt;
			charMap[nextRandomChars] = nextRandomInt;
			charMap[thirdRandomChars] = thirdRandomInt;

			stringMap[randomString] = randomInt;
			stringMap[nextRandomString] = nextRandomInt;
			stringMap[thirdRandomString] = thirdRandomInt;

			fooMap[randomFoo] = randomInt;
			fooMap[nextRandomFoo] = nextRandomInt;
			fooMap[thirdRandomFoo] = thirdRandomInt;

			HashMap<std::int32_t, std::int32_t>::Iterator intIt = intMap.begin();
			HashMap<std::int32_t*, std::int32_t>::Iterator pointerIt = pointerMap.begin();
			HashMap<char*, std::int32_t>::Iterator charIt = charMap.begin();
			HashMap<std::string, std::int32_t>::Iterator stringIt = stringMap.begin();
			HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator fooIt = fooMap.begin();

			HashMap<std::int32_t, std::int32_t>::Iterator otherIntIt(intIt);
			HashMap<std::int32_t*, std::int32_t>::Iterator otherPointerIt(pointerIt);
			HashMap<char*, std::int32_t>::Iterator otherCharIt(charIt);
			HashMap<std::string, std::int32_t>::Iterator otherStringIt(stringIt);
			HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator otherFooIt(fooIt);

			Assert::IsTrue(intIt++ == otherIntIt);
			Assert::IsTrue(pointerIt++ == otherPointerIt);
			Assert::IsTrue(charIt++ == otherCharIt);
			Assert::IsTrue(stringIt++ == otherStringIt);
			Assert::IsTrue(fooIt++ == otherFooIt);

			Assert::IsTrue(intIt != otherIntIt);
			Assert::IsTrue(pointerIt != otherPointerIt);
			Assert::IsTrue(charIt != otherCharIt);
			Assert::IsTrue(stringIt != otherStringIt);
			Assert::IsTrue(fooIt != otherFooIt);
		}

		TEST_METHOD(HashMapIteratorPreIncrement)
		{
			std::int32_t randomInt = rand(), nextRandomInt = rand(), thirdRandomInt = rand();
			char* randomChars = "howdy", *nextRandomChars = "hi", *thirdRandomChars = "hello";
			std::string randomString = std::string(randomChars), nextRandomString = std::string(nextRandomChars), thirdRandomString = std::string(thirdRandomChars);
			Foo randomFoo = Foo(randomInt), nextRandomFoo = Foo(nextRandomInt), thirdRandomFoo = Foo(thirdRandomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			intMap[randomInt] = randomInt;
			intMap[nextRandomInt] = nextRandomInt;
			intMap[thirdRandomInt] = thirdRandomInt;

			pointerMap[&randomInt] = randomInt;
			pointerMap[&nextRandomInt] = nextRandomInt;
			pointerMap[&thirdRandomInt] = thirdRandomInt;

			charMap[randomChars] = randomInt;
			charMap[nextRandomChars] = nextRandomInt;
			charMap[thirdRandomChars] = thirdRandomInt;

			stringMap[randomString] = randomInt;
			stringMap[nextRandomString] = nextRandomInt;
			stringMap[thirdRandomString] = thirdRandomInt;

			fooMap[randomFoo] = randomInt;
			fooMap[nextRandomFoo] = nextRandomInt;
			fooMap[thirdRandomFoo] = thirdRandomInt;

			HashMap<std::int32_t, std::int32_t>::Iterator intIt;
			HashMap<std::int32_t*, std::int32_t>::Iterator pointerIt;
			HashMap<char*, std::int32_t>::Iterator charIt;
			HashMap<std::string, std::int32_t>::Iterator stringIt;
			HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator fooIt;

			auto intItPreIncrementPointer = [&intIt] { intIt.operator++(); };
			auto pointerItPreIncrementPointer = [&pointerIt] { pointerIt.operator++(); };
			auto charsItPreIncrementPointer = [&charIt] { charIt.operator++(); };
			auto stringItPreIncrementPointer = [&stringIt] { stringIt.operator++(); };
			auto fooItPreIncrementPointer = [&fooIt] { fooIt.operator++(); };

			Assert::ExpectException<std::exception>(intItPreIncrementPointer);
			Assert::ExpectException<std::exception>(pointerItPreIncrementPointer);
			Assert::ExpectException<std::exception>(charsItPreIncrementPointer);
			Assert::ExpectException<std::exception>(stringItPreIncrementPointer);
			Assert::ExpectException<std::exception>(fooItPreIncrementPointer);

			intIt = intMap.begin();
			pointerIt = pointerMap.begin();
			charIt = charMap.begin();
			stringIt = stringMap.begin();
			fooIt = fooMap.begin();

			HashMap<std::int32_t, std::int32_t>::Iterator otherIntIt(intIt);
			HashMap<std::int32_t*, std::int32_t>::Iterator otherPointerIt(pointerIt);
			HashMap<char*, std::int32_t>::Iterator otherCharIt(charIt);
			HashMap<std::string, std::int32_t>::Iterator otherStringIt(stringIt);
			HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator otherFooIt(fooIt);

			Assert::IsTrue(++intIt != otherIntIt);
			Assert::IsTrue(++intIt != intMap.end());
			Assert::IsTrue(++intIt == intMap.end());

			Assert::IsTrue(++pointerIt != otherPointerIt);
			Assert::IsTrue(++pointerIt != pointerMap.end());
			Assert::IsTrue(++pointerIt == pointerMap.end());

			Assert::IsTrue(++charIt != otherCharIt);
			Assert::IsTrue(++charIt != charMap.end());
			Assert::IsTrue(++charIt == charMap.end());

			Assert::IsTrue(++stringIt != otherStringIt);
			Assert::IsTrue(++stringIt != stringMap.end());
			Assert::IsTrue(++stringIt == stringMap.end());

			Assert::IsTrue(++fooIt != otherFooIt);
			Assert::IsTrue(++fooIt != fooMap.end());
			Assert::IsTrue(++fooIt == fooMap.end());
		}

		TEST_METHOD(HashMapIteratorDereference)
		{
			std::int32_t randomInt = rand();
			char* randomChars = "howdy";
			std::string randomString = std::string(randomChars);
			Foo randomFoo = Foo(randomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			intMap[randomInt] = randomInt;
			pointerMap[&randomInt] = randomInt;
			charMap[randomChars] = randomInt;
			stringMap[randomString] = randomInt;
			fooMap[randomFoo] = randomInt;

			HashMap<std::int32_t, std::int32_t>::Iterator intIt;
			HashMap<std::int32_t*, std::int32_t>::Iterator pointerIt;
			HashMap<char*, std::int32_t>::Iterator charIt;
			HashMap<std::string, std::int32_t>::Iterator stringIt;
			HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator fooIt;

			auto intItDereferencePointer = [&intIt] { intIt.operator*(); };
			auto pointerItDereferencePointer = [&pointerIt] { pointerIt.operator*(); };
			auto charsItDereferencePointer = [&charIt] { charIt.operator*(); };
			auto stringItDereferencePointer = [&stringIt] { stringIt.operator*(); };
			auto fooItDereferencePointer = [&fooIt] { fooIt.operator*(); };

			Assert::ExpectException<std::exception>(intItDereferencePointer);
			Assert::ExpectException<std::exception>(pointerItDereferencePointer);
			Assert::ExpectException<std::exception>(charsItDereferencePointer);
			Assert::ExpectException<std::exception>(stringItDereferencePointer);
			Assert::ExpectException<std::exception>(fooItDereferencePointer);

			intIt = intMap.end();
			pointerIt = pointerMap.end();
			charIt = charMap.end();
			stringIt = stringMap.end();
			fooIt = fooMap.end();

			Assert::ExpectException<std::exception>(intItDereferencePointer);
			Assert::ExpectException<std::exception>(pointerItDereferencePointer);
			Assert::ExpectException<std::exception>(charsItDereferencePointer);
			Assert::ExpectException<std::exception>(stringItDereferencePointer);
			Assert::ExpectException<std::exception>(fooItDereferencePointer);

			Assert::IsTrue(randomInt == (*(intMap.Find(randomInt))).second);
			Assert::IsTrue(randomInt == (*(pointerMap.Find(&randomInt))).second);
			Assert::IsTrue(randomInt == (*(charMap.Find(randomChars))).second);
			Assert::IsTrue(randomInt == (*(stringMap.Find(randomString))).second);
			Assert::IsTrue(randomInt == (*(fooMap.Find(randomFoo))).second);

			const HashMap<std::int32_t, std::int32_t>::Iterator constIntIt;
			const HashMap<std::int32_t*, std::int32_t>::Iterator constPointerIt;
			const HashMap<char*, std::int32_t>::Iterator constCharIt;
			const HashMap<std::string, std::int32_t>::Iterator constStringIt;
			const HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator constFooIt;

			auto constIntItDereferencePointer = [&constIntIt] { constIntIt.operator*(); };
			auto constPointerItDereferencePointer = [&constPointerIt] { constPointerIt.operator*(); };
			auto constCharsItDereferencePointer = [&constCharIt] { constCharIt.operator*(); };
			auto constStringItDereferencePointer = [&constStringIt] { constStringIt.operator*(); };
			auto constFooItDereferencePointer = [&constFooIt] { constFooIt.operator*(); };

			Assert::ExpectException<std::exception>(constIntItDereferencePointer);
			Assert::ExpectException<std::exception>(constPointerItDereferencePointer);
			Assert::ExpectException<std::exception>(constCharsItDereferencePointer);
			Assert::ExpectException<std::exception>(constStringItDereferencePointer);
			Assert::ExpectException<std::exception>(constFooItDereferencePointer);
		}

		TEST_METHOD(HashMapIteratorArrow)
		{
			std::int32_t randomInt = rand();
			char* randomChars = "howdy";
			std::string randomString = std::string(randomChars);
			Foo randomFoo = Foo(randomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			intMap[randomInt] = randomInt;
			pointerMap[&randomInt] = randomInt;
			charMap[randomChars] = randomInt;
			stringMap[randomString] = randomInt;
			fooMap[randomFoo] = randomInt;

			HashMap<std::int32_t, std::int32_t>::Iterator intIt;
			HashMap<std::int32_t*, std::int32_t>::Iterator pointerIt;
			HashMap<char*, std::int32_t>::Iterator charIt;
			HashMap<std::string, std::int32_t>::Iterator stringIt;
			HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator fooIt;

			auto intItArrowPointer = [&intIt] { intIt.operator->(); };
			auto pointerItArrowPointer = [&pointerIt] { pointerIt.operator->(); };
			auto charsItArrowPointer = [&charIt] { charIt.operator->(); };
			auto stringItArrowPointer = [&stringIt] { stringIt.operator->(); };
			auto fooItArrowPointer = [&fooIt] { fooIt.operator->(); };

			Assert::ExpectException<std::exception>(intItArrowPointer);
			Assert::ExpectException<std::exception>(pointerItArrowPointer);
			Assert::ExpectException<std::exception>(charsItArrowPointer);
			Assert::ExpectException<std::exception>(stringItArrowPointer);
			Assert::ExpectException<std::exception>(fooItArrowPointer);

			intIt = intMap.end();
			pointerIt = pointerMap.end();
			charIt = charMap.end();
			stringIt = stringMap.end();
			fooIt = fooMap.end();

			Assert::ExpectException<std::exception>(intItArrowPointer);
			Assert::ExpectException<std::exception>(pointerItArrowPointer);
			Assert::ExpectException<std::exception>(charsItArrowPointer);
			Assert::ExpectException<std::exception>(stringItArrowPointer);
			Assert::ExpectException<std::exception>(fooItArrowPointer);

			Assert::IsTrue(randomInt == (intMap.Find(randomInt).operator->())->second);
			Assert::IsTrue(randomInt == (pointerMap.Find(&randomInt).operator->())->second);
			Assert::IsTrue(randomInt == (charMap.Find(randomChars).operator->())->second);
			Assert::IsTrue(randomInt == (stringMap.Find(randomString).operator->())->second);
			Assert::IsTrue(randomInt == (fooMap.Find(randomFoo).operator->())->second);

			const HashMap<std::int32_t, std::int32_t>::Iterator constIntIt;
			const HashMap<std::int32_t*, std::int32_t>::Iterator constPointerIt;
			const HashMap<char*, std::int32_t>::Iterator constCharIt;
			const HashMap<std::string, std::int32_t>::Iterator constStringIt;
			const HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator constFooIt;

			auto constIntItArrowPointer = [&constIntIt] { constIntIt.operator->(); };
			auto constPointerItArrowPointer = [&constPointerIt] { constPointerIt.operator->(); };
			auto constCharItArrowPointer = [&constCharIt] { constCharIt.operator->(); };
			auto constStringItArrowPointer = [&constStringIt] { constStringIt.operator->(); };
			auto constFooItArrowPointer = [&constFooIt] { constFooIt.operator->(); };

			Assert::ExpectException<std::exception>(constIntItArrowPointer);
			Assert::ExpectException<std::exception>(constPointerItArrowPointer);
			Assert::ExpectException<std::exception>(constCharItArrowPointer);
			Assert::ExpectException<std::exception>(constStringItArrowPointer);
			Assert::ExpectException<std::exception>(constFooItArrowPointer);
		}

		TEST_METHOD(HashMapIteratorEquality)
		{
			std::int32_t randomInt = rand();
			char* randomChars = "howdy";
			std::string randomString = std::string(randomChars);
			Foo randomFoo = Foo(randomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			intMap[randomInt] = randomInt;
			pointerMap[&randomInt] = randomInt;
			charMap[randomChars] = randomInt;
			stringMap[randomString] = randomInt;
			fooMap[randomFoo] = randomInt;

			HashMap<std::int32_t, std::int32_t>::Iterator intIt;
			HashMap<std::int32_t*, std::int32_t>::Iterator pointerIt;
			HashMap<char*, std::int32_t>::Iterator charIt;
			HashMap<std::string, std::int32_t>::Iterator stringIt;
			HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator fooIt;

			HashMap<std::int32_t, std::int32_t>::Iterator otherIntIt;
			HashMap<std::int32_t*, std::int32_t>::Iterator otherPointerIt;
			HashMap<char*, std::int32_t>::Iterator otherCharIt;
			HashMap<std::string, std::int32_t>::Iterator otherStringIt;
			HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator otherFooIt;

			Assert::IsTrue(intIt == otherIntIt);
			Assert::IsTrue(pointerIt == otherPointerIt);
			Assert::IsTrue(charIt == otherCharIt);
			Assert::IsTrue(stringIt == otherStringIt);
			Assert::IsTrue(fooIt == otherFooIt);

			intIt = intMap.begin();
			pointerIt = pointerMap.begin();
			charIt = charMap.begin();
			stringIt = stringMap.begin();
			fooIt = fooMap.begin();

			Assert::IsFalse(intIt == otherIntIt);
			Assert::IsFalse(pointerIt == otherPointerIt);
			Assert::IsFalse(charIt == otherCharIt);
			Assert::IsFalse(stringIt == otherStringIt);
			Assert::IsFalse(fooIt == otherFooIt);

			otherIntIt = intMap.end();
			otherPointerIt = pointerMap.end();
			otherCharIt = charMap.end();
			otherStringIt = stringMap.end();
			otherFooIt = fooMap.end();

			Assert::IsFalse(intIt == otherIntIt);
			Assert::IsFalse(pointerIt == otherPointerIt);
			Assert::IsFalse(charIt == otherCharIt);
			Assert::IsFalse(stringIt == otherStringIt);
			Assert::IsFalse(fooIt == otherFooIt);
		}

		TEST_METHOD(HashMapIteratorInequality)
		{
			std::int32_t randomInt = rand();
			char* randomChars = "howdy";
			std::string randomString = std::string(randomChars);
			Foo randomFoo = Foo(randomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			intMap[randomInt] = randomInt;
			pointerMap[&randomInt] = randomInt;
			charMap[randomChars] = randomInt;
			stringMap[randomString] = randomInt;
			fooMap[randomFoo] = randomInt;

			HashMap<std::int32_t, std::int32_t>::Iterator intIt;
			HashMap<std::int32_t*, std::int32_t>::Iterator pointerIt;
			HashMap<char*, std::int32_t>::Iterator charIt;
			HashMap<std::string, std::int32_t>::Iterator stringIt;
			HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator fooIt;

			HashMap<std::int32_t, std::int32_t>::Iterator otherIntIt;
			HashMap<std::int32_t*, std::int32_t>::Iterator otherPointerIt;
			HashMap<char*, std::int32_t>::Iterator otherCharIt;
			HashMap<std::string, std::int32_t>::Iterator otherStringIt;
			HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator otherFooIt;

			Assert::IsFalse(intIt != otherIntIt);
			Assert::IsFalse(pointerIt != otherPointerIt);
			Assert::IsFalse(charIt != otherCharIt);
			Assert::IsFalse(stringIt != otherStringIt);
			Assert::IsFalse(fooIt != otherFooIt);

			intIt = intMap.begin();
			pointerIt = pointerMap.begin();
			charIt = charMap.begin();
			stringIt = stringMap.begin();
			fooIt = fooMap.begin();

			Assert::IsTrue(intIt != otherIntIt);
			Assert::IsTrue(pointerIt != otherPointerIt);
			Assert::IsTrue(charIt != otherCharIt);
			Assert::IsTrue(stringIt != otherStringIt);
			Assert::IsTrue(fooIt != otherFooIt);

			otherIntIt = intMap.end();
			otherPointerIt = pointerMap.end();
			otherCharIt = charMap.end();
			otherStringIt = stringMap.end();
			otherFooIt = fooMap.end();

			Assert::IsTrue(intIt != otherIntIt);
			Assert::IsTrue(pointerIt != otherPointerIt);
			Assert::IsTrue(charIt != otherCharIt);
			Assert::IsTrue(stringIt != otherStringIt);
			Assert::IsTrue(fooIt != otherFooIt);
		}

		TEST_METHOD(HashMapIteratorAssignment)
		{
			std::int32_t randomInt = rand();
			char* randomChars = "howdy";
			std::string randomString = std::string(randomChars);
			Foo randomFoo = Foo(randomInt);

			HashMap<std::int32_t, std::int32_t> intMap;
			HashMap<std::int32_t*, std::int32_t> pointerMap;
			HashMap<char*, std::int32_t> charMap;
			HashMap<std::string, std::int32_t> stringMap;
			HashMap<Foo, std::int32_t, FooHashFunctor> fooMap;

			intMap[randomInt] = randomInt;
			pointerMap[&randomInt] = randomInt;
			charMap[randomChars] = randomInt;
			stringMap[randomString] = randomInt;
			fooMap[randomFoo] = randomInt;

			HashMap<std::int32_t, std::int32_t>::Iterator intIt;
			HashMap<std::int32_t*, std::int32_t>::Iterator pointerIt;
			HashMap<char*, std::int32_t>::Iterator charIt;
			HashMap<std::string, std::int32_t>::Iterator stringIt;
			HashMap<Foo, std::int32_t, FooHashFunctor>::Iterator fooIt;

			intIt = intMap.begin();
			pointerIt = pointerMap.begin();
			charIt = charMap.begin();
			stringIt = stringMap.begin();
			fooIt = fooMap.begin();

			Assert::IsTrue(intIt == intMap.begin());
			Assert::IsTrue(pointerIt == pointerMap.begin());
			Assert::IsTrue(charIt == charMap.begin());
			Assert::IsTrue(stringIt == stringMap.begin());
			Assert::IsTrue(fooIt == fooMap.begin());

			intIt = intMap.end();
			pointerIt = pointerMap.end();
			charIt = charMap.end();
			stringIt = stringMap.end();
			fooIt = fooMap.end();

			Assert::IsTrue(intIt == intMap.end());
			Assert::IsTrue(pointerIt == pointerMap.end());
			Assert::IsTrue(charIt == charMap.end());
			Assert::IsTrue(stringIt == stringMap.end());
			Assert::IsTrue(fooIt == fooMap.end());
		}

	private:

		static _CrtMemState sStartMemState;

		const static std::int32_t mBucketCount = 11;
		const static std::int32_t mOtherBucketCount = 13;
		const static std::uint32_t mZero = 0;
	};

	_CrtMemState HashMapTest::sStartMemState;
}