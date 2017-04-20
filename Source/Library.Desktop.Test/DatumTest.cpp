#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(DatumTest)
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

		TEST_METHOD(DatumDefaultConstructor)
		{
			Datum datum;

			Assert::AreEqual(0U, datum.Size());
			Assert::IsTrue(datum.Type() == Datum::DatumType::Unknown);
		}

		TEST_METHOD(DatumCopyConstructor)
		{
			std::int32_t value = rand();
			Datum datum;
			datum = value;

			Datum otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);
		}

		TEST_METHOD(DatumAssignmentOperator)
		{
			std::int32_t intValue = rand(), nextIntValue = rand(), thirdIntValue = rand();
			Foo value(intValue), nextValue(nextIntValue), thirdValue(thirdIntValue);
			Datum datum;
			datum = &value;

			Datum otherDatum;
			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);

			RTTI* storage[1];
			storage[0] = &value;

			datum.SetStorage(storage, 1);

			Assert::IsTrue(datum == otherDatum);

			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);

			datum.Clear();
			datum = &value;

			Assert::IsTrue(datum == otherDatum);

			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);

			RTTI* otherStorage[2];
			otherStorage[0] = &nextValue;
			otherStorage[1] = &thirdValue;

			datum.SetStorage(otherStorage, 2);

			Assert::IsFalse(datum == otherDatum);

			otherDatum = datum;

			Assert::IsTrue(otherDatum == datum);

			datum.Clear();
			otherDatum.Clear();
			datum.SetType(Datum::DatumType::Integer);

			datum.PushBack(intValue);
			datum.PushBack(nextIntValue);
			datum.PushBack(thirdIntValue);

			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);

			datum.Clear();
			otherDatum.Clear();
			datum.SetType(Datum::DatumType::Float);

			datum.PushBack(0.0f);
			datum.PushBack(2.0f);
			datum.PushBack(4.0f);

			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);

			datum.Clear();
			otherDatum.Clear();
			datum.SetType(Datum::DatumType::Vec4);
			glm::vec4 vecValue, nextVecValue, thirdVecValue;

			vecValue.a = 2.0f;
			nextVecValue.b = 2.0f;
			thirdVecValue.g = 2.0f;

			datum.PushBack(vecValue);
			datum.PushBack(nextVecValue);
			datum.PushBack(thirdVecValue);

			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);

			datum.Clear();
			otherDatum.Clear();
			datum.SetType(Datum::DatumType::Mat4);
			glm::mat4 matValue, nextMatValue, thirdMatValue;

			matValue[3].a = 2.0f;
			nextMatValue[2].b = 2.0f;
			thirdMatValue[1].g = 2.0f;

			datum.PushBack(matValue);
			datum.PushBack(nextMatValue);
			datum.PushBack(thirdMatValue);

			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);

			datum.Clear();
			otherDatum.Clear();
			datum.SetType(Datum::DatumType::String);
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";

			datum.PushBack(stringValue);
			datum.PushBack(nextStringValue);
			datum.PushBack(thirdStringValue);

			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);

			datum.Clear();
			otherDatum.Clear();
			datum.SetType(Datum::DatumType::Pointer);
			Foo fooValue(intValue), nextFooValue(nextIntValue), thirdFooValue(thirdIntValue);

			datum.PushBack(&fooValue);
			datum.PushBack(&nextFooValue);
			datum.PushBack(&thirdFooValue);

			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);

			datum.Clear();
			otherDatum.Clear();
			datum.SetType(Datum::DatumType::Table);
			Scope scopeValue, nextScopeValue, thirdScopeValue;

			datum.PushBack(scopeValue);
			datum.PushBack(nextScopeValue);
			datum.PushBack(thirdScopeValue);

			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);
		}

		TEST_METHOD(DatumMoveConstructor)
		{
			std::int32_t intValue = rand(), nextIntValue = rand(), thirdIntValue = rand();
			Foo value(intValue), nextValue(nextIntValue), thirdValue(thirdIntValue);

			Datum datum;

			datum.PushBack(&value);
			datum.PushBack(&nextValue);
			datum.PushBack(&thirdValue);

			Datum otherDatum = std::move(datum);

			Assert::AreEqual(0U, datum.Size());
			Assert::AreEqual(0U, datum.Capacity());
			Assert::IsTrue(Datum::DatumType::Unknown == datum.Type());
			Assert::IsTrue(&value == otherDatum.Get<RTTI*&>(0));
			Assert::IsTrue(&nextValue == otherDatum.Get<RTTI*&>(1));
			Assert::IsTrue(&thirdValue == otherDatum.Get<RTTI*&>(2));
		}

		TEST_METHOD(DatumMoveAssignmentOperator)
		{
			std::int32_t intValue = rand(), nextIntValue = rand(), thirdIntValue = rand();
			Foo value(intValue), nextValue(nextIntValue), thirdValue(thirdIntValue);

			Datum datum;

			datum.PushBack(&value);
			datum.PushBack(&nextValue);
			datum.PushBack(&thirdValue);

			Datum otherDatum;
			otherDatum = intValue;
			otherDatum = std::move(datum);

			Assert::AreEqual(0U, datum.Size());
			Assert::AreEqual(0U, datum.Capacity());
			Assert::IsTrue(Datum::DatumType::Unknown == datum.Type());
			Assert::IsTrue(&value == otherDatum.Get<RTTI*&>(0));
			Assert::IsTrue(&nextValue == otherDatum.Get<RTTI*&>(1));
			Assert::IsTrue(&thirdValue == otherDatum.Get<RTTI*&>(2));
		}

		TEST_METHOD(DatumIntegerAssignment)
		{
			std::int32_t value = rand();
			Datum datum;
			Datum otherDatum = datum = value;

			Assert::IsTrue(datum == otherDatum);
			Assert::IsTrue(datum == value);
		}

		TEST_METHOD(DatumFloatAssignment)
		{
			float value = 2.0f;
			Datum datum;
			Datum otherDatum = datum = value;

			Assert::IsTrue(datum == otherDatum);
			Assert::IsTrue(datum == value);
		}

		TEST_METHOD(DatumVectorAssignment)
		{
			glm::vec4 value;
			value[0] = 2.0f;
			Datum datum;
			Datum otherDatum = datum = value;

			Assert::IsTrue(datum == otherDatum);
			Assert::IsTrue(datum == value);
		}

		TEST_METHOD(DatumMatrixAssignment)
		{
			glm::mat4 value;
			value[0].a = 2.0f;
			Datum datum;
			Datum otherDatum = datum = value;

			Assert::IsTrue(datum == otherDatum);
			Assert::IsTrue(datum == value);
		}

		TEST_METHOD(DatumStringAssignment)
		{
			std::string value = "Howdy";
			Datum datum;
			Datum otherDatum = datum = value;

			Assert::IsTrue(datum == otherDatum);
			Assert::IsTrue(datum == value);
		}

		TEST_METHOD(DatumPointerAssignment)
		{
			std::int32_t randomValue = rand();
			Foo value(randomValue);
			Datum datum;
			Datum otherDatum = datum = &value;

			Assert::IsTrue(datum == otherDatum);
			Assert::IsTrue(datum == &value);
		}

		TEST_METHOD(DatumScopeAssignment)
		{
			Scope value;
			Datum datum;
			Datum otherDatum = datum = value;

			Assert::IsTrue(datum == otherDatum);
			Assert::IsTrue(datum == value);
		}

		TEST_METHOD(DatumDestructor)
		{
			std::int32_t intValue = rand(), nextIntValue = rand(), thirdIntValue = rand();
			Datum datum, otherDatum;

			datum = intValue;
			datum.PushBack(nextIntValue);
			datum.PushBack(thirdIntValue);

			Foo foo(intValue);
			RTTI** storage = static_cast<RTTI**>(malloc(sizeof(RTTI*)));
			storage[0] = &foo;

			otherDatum.SetStorage(storage, 1);
			free(storage);
		}

		TEST_METHOD(DatumIsExternal)
		{
			Datum datum;

			Assert::IsFalse(datum.IsExternal());

			std::int32_t value = rand();
			datum.SetStorage(&value, 1);

			Assert::IsTrue(datum.IsExternal());
		}

		TEST_METHOD(DatumType)
		{
			float value = 2.0f;
			Datum datum, otherDatum;
			
			datum.SetType(Datum::DatumType::Integer);

			Assert::IsTrue(datum.Type() == Datum::DatumType::Integer);

			otherDatum = value;

			Assert::IsTrue(otherDatum.Type() == Datum::DatumType::Float);
		}

		TEST_METHOD(DatumSetType)
		{
			Datum datum = Datum();
			Datum::DatumType type = Datum::DatumType::Integer;
			auto setTypePointer = [&datum, &type] { datum.SetType(type); };
			datum.SetType(type);

			Assert::IsTrue(datum.Type() == Datum::DatumType::Integer);

			Assert::ExpectException<std::exception>(setTypePointer);

			datum.Clear();
			datum.SetType(Datum::DatumType::Float);

			Assert::IsTrue(datum.Type() == Datum::DatumType::Float);

			datum.Clear();
			datum.SetType(Datum::DatumType::Vec4);

			Assert::IsTrue(datum.Type() == Datum::DatumType::Vec4);

			datum.Clear();
			datum.SetType(Datum::DatumType::Mat4);

			Assert::IsTrue(datum.Type() == Datum::DatumType::Mat4);

			datum.Clear();
			datum.SetType(Datum::DatumType::String);

			Assert::IsTrue(datum.Type() == Datum::DatumType::String);

			datum.Clear();
			datum.SetType(Datum::DatumType::Pointer);

			Assert::IsTrue(datum.Type() == Datum::DatumType::Pointer);
		}

		TEST_METHOD(DatumSize)
		{
			Datum datum;

			Assert::AreEqual(0U, datum.Size());

			std::int32_t value = rand();

			datum.PushBack(value);

			Assert::AreEqual(1U, datum.Size());

			datum.PushBack(value);

			Assert::AreEqual(2U, datum.Size());
		}

		TEST_METHOD(DatumCapacity)
		{
			Datum datum;

			Assert::AreEqual(0U, datum.Capacity());

			datum.SetType(Datum::DatumType::Integer);
			datum.Reserve(2);

			Assert::AreEqual(2U, datum.Capacity());

			std::int32_t value = rand();
			datum.PushBack(value);
			datum.PushBack(value);

			Assert::AreEqual(2U, datum.Capacity());

			datum.PushBack(value);

			Assert::AreEqual(3U, datum.Capacity());

			datum.Reserve(10);

			Assert::AreEqual(10U, datum.Capacity());
		}

		TEST_METHOD(DatumSetSize)
		{
			Datum datum;
			std::uint32_t size = 5;
			auto setSizePointer = [&datum, &size] { datum.SetSize(size); };
			
			Assert::ExpectException<std::exception>(setSizePointer);

			std::int32_t* storage = static_cast<std::int32_t*>(malloc(sizeof(std::int32_t) * size));
			datum.SetStorage(storage, size);

			Assert::ExpectException<std::exception>(setSizePointer);

			datum.Clear();
			std::int32_t intValue = rand();
			datum = intValue;
			datum.SetSize(size);

			Assert::AreEqual(size, datum.Size());
			Assert::AreEqual(size, datum.Capacity());

			datum.SetSize(size - 1);

			Assert::AreEqual(size - 1, datum.Size());
			Assert::AreEqual(size, datum.Capacity());

			datum.Clear();
			float floatValue = 2.0f;
			datum = floatValue;
			datum.SetSize(size);

			Assert::AreEqual(size, datum.Size());
			Assert::AreEqual(size, datum.Capacity());

			datum.SetSize(size - 1);

			Assert::AreEqual(size - 1, datum.Size());
			Assert::AreEqual(size, datum.Capacity());

			datum.Clear();
			glm::vec4 vectorValue;
			vectorValue[0] = 2.0f;
			datum = vectorValue;
			datum.SetSize(size);

			Assert::AreEqual(size, datum.Size());
			Assert::AreEqual(size, datum.Capacity());

			datum.SetSize(size - 1);

			Assert::AreEqual(size - 1, datum.Size());
			Assert::AreEqual(size, datum.Capacity());

			datum.Clear();
			glm::mat4 matrixValue;
			matrixValue[0].a = 2.0f;
			datum = matrixValue;
			datum.SetSize(size);

			Assert::AreEqual(size, datum.Size());
			Assert::AreEqual(size, datum.Capacity());

			datum.SetSize(size - 1);

			Assert::AreEqual(size - 1, datum.Size());
			Assert::AreEqual(size, datum.Capacity());

			datum.Clear();
			std::string stringValue = "Howdy";
			datum = stringValue;
			datum.SetSize(size);

			Assert::AreEqual(size, datum.Size());
			Assert::AreEqual(size, datum.Capacity());

			datum.SetSize(size - 1);

			Assert::AreEqual(size - 1, datum.Size());
			Assert::AreEqual(size, datum.Capacity());

			datum.Clear();
			Foo fooValue(intValue);
			datum = &fooValue;
			datum.SetSize(size);

			Assert::AreEqual(size, datum.Size());
			Assert::AreEqual(size, datum.Capacity());

			datum.SetSize(size - 1);

			Assert::AreEqual(size - 1, datum.Size());
			Assert::AreEqual(size, datum.Capacity());

			datum.Clear();
			Scope scopeValue;;
			datum = &scopeValue;
			datum.SetSize(size);

			Assert::AreEqual(size, datum.Size());
			Assert::AreEqual(size, datum.Capacity());

			datum.SetSize(size - 1);

			Assert::AreEqual(size - 1, datum.Size());
			Assert::AreEqual(size, datum.Capacity());

			free(storage);
		}

		TEST_METHOD(DatumClear)
		{
			Datum datum;

			datum.Clear();

			Assert::AreEqual(0U, datum.Size());
			Assert::AreEqual(0U, datum.Capacity());
			Assert::IsTrue(datum.Type() == Datum::DatumType::Unknown);

			std::int32_t intValue = rand();
			datum = intValue;
			datum.PushBack(intValue);
			datum.PushBack(intValue);

			datum.Clear();

			Assert::AreEqual(0U, datum.Size());
			Assert::AreEqual(0U, datum.Capacity());
			Assert::IsTrue(datum.Type() == Datum::DatumType::Unknown);

			float floatValue = 2.0f;
			datum = floatValue;
			datum.PushBack(floatValue);
			datum.PushBack(floatValue);

			datum.Clear();

			Assert::AreEqual(0U, datum.Size());
			Assert::AreEqual(0U, datum.Capacity());
			Assert::IsTrue(datum.Type() == Datum::DatumType::Unknown);

			glm::vec4 vectorValue;
			vectorValue[0] = 2.0f;
			datum = vectorValue;
			datum.PushBack(vectorValue);
			datum.PushBack(vectorValue);

			datum.Clear();

			Assert::AreEqual(0U, datum.Size());
			Assert::AreEqual(0U, datum.Capacity());
			Assert::IsTrue(datum.Type() == Datum::DatumType::Unknown);

			glm::mat4 matrixValue;
			matrixValue[0].a = 2.0f;
			datum = matrixValue;
			datum.PushBack(matrixValue);
			datum.PushBack(matrixValue);

			datum.Clear();

			Assert::AreEqual(0U, datum.Size());
			Assert::AreEqual(0U, datum.Capacity());
			Assert::IsTrue(datum.Type() == Datum::DatumType::Unknown);

			std::string stringValue = "Howdy";
			datum = stringValue;
			datum.PushBack(stringValue);
			datum.PushBack(stringValue);

			datum.Clear();

			Assert::AreEqual(0U, datum.Size());
			Assert::AreEqual(0U, datum.Capacity());
			Assert::IsTrue(datum.Type() == Datum::DatumType::Unknown);

			Foo fooValue(intValue);
			datum = &fooValue;
			datum.PushBack(&fooValue);
			datum.PushBack(&fooValue);

			datum.Clear();

			Assert::AreEqual(0U, datum.Size());
			Assert::AreEqual(0U, datum.Capacity());
			Assert::IsTrue(datum.Type() == Datum::DatumType::Unknown);
		}

		TEST_METHOD(DatumIntegerSetStorage)
		{
			std::int32_t values[3] = { rand(), rand(), rand() };
			
			Datum datum;
			datum.SetStorage(values, sizeof(values));

			Assert::IsTrue(datum.Type() == Datum::DatumType::Integer);
			Assert::IsTrue(datum.Capacity() == sizeof(values));
			Assert::IsTrue(datum.Size() == sizeof(values));
			Assert::AreEqual(values[0], datum.Get<int32_t&>(0));
			Assert::AreEqual(values[1], datum.Get<int32_t&>(1));
			Assert::AreEqual(values[2], datum.Get<int32_t&>(2));

			auto setStoragePointer = [&datum, &values] { datum.SetStorage(values, 0); };

			Assert::ExpectException<std::exception>(setStoragePointer);
		}

		TEST_METHOD(DatumFloatSetStorage)
		{
			float values[3] = { 2.0f, 4.0f, 6.0f };

			Datum datum;
			datum.SetStorage(values, sizeof(values));

			Assert::IsTrue(datum.Type() == Datum::DatumType::Float);
			Assert::IsTrue(datum.Capacity() == sizeof(values));
			Assert::IsTrue(datum.Size() == sizeof(values));
			Assert::AreEqual(values[0], datum.Get<float&>(0));
			Assert::AreEqual(values[1], datum.Get<float&>(1));
			Assert::AreEqual(values[2], datum.Get<float&>(2));

			auto setStoragePointer = [&datum, &values] { datum.SetStorage(values, 0); };

			Assert::ExpectException<std::exception>(setStoragePointer);
		}

		TEST_METHOD(DatumVectorSetStorage)
		{
			glm::vec4 values[3];
			values[0][0] = 2.0f;
			values[1][0] = 4.0f;
			values[2][0] = 6.0f;

			Datum datum;
			datum.SetStorage(values, sizeof(values));

			Assert::IsTrue(datum.Type() == Datum::DatumType::Vec4);
			Assert::IsTrue(datum.Capacity() == sizeof(values));
			Assert::IsTrue(datum.Size() == sizeof(values));
			Assert::IsTrue(values[0] == datum.Get<glm::vec4&>(0));
			Assert::IsTrue(values[1] == datum.Get<glm::vec4&>(1));
			Assert::IsTrue(values[2] == datum.Get<glm::vec4&>(2));

			auto setStoragePointer = [&datum, &values] { datum.SetStorage(values, 0); };

			Assert::ExpectException<std::exception>(setStoragePointer);
		}

		TEST_METHOD(DatumMatrixSetStorage)
		{
			glm::mat4 values[3];
			values[0][0].a = 2.0f;
			values[1][0].a = 4.0f;
			values[2][0].a = 6.0f;

			Datum datum;
			datum.SetStorage(values, sizeof(values));

			Assert::IsTrue(datum.Type() == Datum::DatumType::Mat4);
			Assert::IsTrue(datum.Capacity() == sizeof(values));
			Assert::IsTrue(datum.Size() == sizeof(values));
			Assert::IsTrue(values[0] == datum.Get<glm::mat4&>(0));
			Assert::IsTrue(values[1] == datum.Get<glm::mat4&>(1));
			Assert::IsTrue(values[2] == datum.Get<glm::mat4&>(2));

			auto setStoragePointer = [&datum, &values] { datum.SetStorage(values, 0); };

			Assert::ExpectException<std::exception>(setStoragePointer);
		}

		TEST_METHOD(DatumStringSetStorage)
		{
			std::string values[3] = { "Howdy", "Hi", "Hello" };

			Datum datum;
			datum.SetStorage(values, 3);

			Assert::IsTrue(datum.Type() == Datum::DatumType::String);
			Assert::IsTrue(datum.Capacity() == 3);
			Assert::IsTrue(datum.Size() == 3);
			Assert::IsTrue(values[0] == datum.Get<std::string&>(0));
			Assert::IsTrue(values[1] == datum.Get<std::string&>(1));
			Assert::IsTrue(values[2] == datum.Get<std::string&>(2));

			auto setStoragePointer = [&datum, &values] { datum.SetStorage(values, 0); };

			Assert::ExpectException<std::exception>(setStoragePointer);
		}

		TEST_METHOD(DatumPointerSetStorage)
		{
			RTTI* values[3];
			Foo fooValues[3];

			values[0] = &fooValues[0];
			values[1] = &fooValues[1];
			values[2] = &fooValues[2];

			Datum datum;
			datum.SetStorage(values, sizeof(values));

			Assert::IsTrue(datum.Type() == Datum::DatumType::Pointer);
			Assert::IsTrue(datum.Capacity() == sizeof(values));
			Assert::IsTrue(datum.Size() == sizeof(values));
			Assert::IsTrue(values[0] == datum.Get<RTTI*&>(0));
			Assert::IsTrue(values[1] == datum.Get<RTTI*&>(1));
			Assert::IsTrue(values[2] == datum.Get<RTTI*&>(2));

			auto setStoragePointer = [&datum, &values] { datum.SetStorage(values, 0); };

			Assert::ExpectException<std::exception>(setStoragePointer);
		}

		TEST_METHOD(DatumEquality)
		{
			Datum datum, otherDatum;
			std::int32_t intValue = rand();

			datum = intValue;

			Assert::IsFalse(datum == otherDatum);

			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);

			float floatValue = 2.0f;

			datum.Clear();
			datum = floatValue;

			Assert::IsFalse(datum == otherDatum);

			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);

			glm::vec4 vectorValue;
			vectorValue[0] = 2.0f;

			datum.Clear();
			datum = vectorValue;

			Assert::IsFalse(datum == otherDatum);

			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);

			glm::mat4 matValue;
			matValue[0].a = 2.0f;

			datum.Clear();
			datum = matValue;

			Assert::IsFalse(datum == otherDatum);

			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);

			std::string stringValue = "Howdy";

			datum.Clear();
			datum = stringValue;

			Assert::IsFalse(datum == otherDatum);

			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);

			Foo pointerValue(intValue);

			datum.Clear();
			datum = &pointerValue;

			Assert::IsFalse(datum == otherDatum);

			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);

			Foo* nullFoo = nullptr;

			datum.Clear();
			datum = nullFoo;

			Assert::IsFalse(datum == otherDatum);

			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);

			Scope scopeValue;

			datum.Clear();
			datum = &scopeValue;

			Assert::IsFalse(datum == otherDatum);

			otherDatum = datum;

			Assert::IsTrue(datum == otherDatum);
		}

		TEST_METHOD(DatumIntegerEquality)
		{
			Datum datum;
			std::int32_t value = rand();

			Assert::IsFalse(datum == value);

			datum = value;

			Assert::IsTrue(datum == value);
		}

		TEST_METHOD(DatumFloatEquality)
		{
			Datum datum;
			float value = 2.0f;

			Assert::IsFalse(datum == value);

			datum = value;

			Assert::IsTrue(datum == value);
		}

		TEST_METHOD(DatumVectorEquality)
		{
			Datum datum;
			glm::vec4 value;
			value[0] = 2.0f;

			Assert::IsFalse(datum == value);

			datum = value;

			Assert::IsTrue(datum == value);
		}

		TEST_METHOD(DatumMatrixEquality)
		{
			Datum datum;
			glm::mat4 value;
			value[0].a = 2.0f;

			Assert::IsFalse(datum == value);

			datum = value;

			Assert::IsTrue(datum == value);
		}

		TEST_METHOD(DatumStringEquality)
		{
			Datum datum;
			std::string value = "Howdy";

			Assert::IsFalse(datum == value);

			datum = value;

			Assert::IsTrue(datum == value);
		}

		TEST_METHOD(DatumPointerEquality)
		{
			Datum datum;
			std::int32_t intValue = rand();
			Foo value(intValue);

			Assert::IsFalse(datum == &value);

			datum = &value;

			Assert::IsTrue(datum == &value);
		}

		TEST_METHOD(DatumTableEquality)
		{
			Datum datum;
			Scope value;

			Assert::IsFalse(datum == value);

			datum = value;

			Assert::IsTrue(datum == value);
		}

		TEST_METHOD(DatumInequality)
		{
			Datum datum, otherDatum;
			std::int32_t intValue = rand();

			datum = intValue;

			Assert::IsTrue(datum != otherDatum);

			otherDatum = datum;

			Assert::IsFalse(datum != otherDatum);

			float floatValue = 2.0f;

			datum.Clear();
			datum = floatValue;

			Assert::IsTrue(datum != otherDatum);

			otherDatum = datum;

			Assert::IsFalse(datum != otherDatum);

			glm::vec4 vectorValue;
			vectorValue[0] = 2.0f;

			datum.Clear();
			datum = vectorValue;

			Assert::IsTrue(datum != otherDatum);

			otherDatum = datum;

			Assert::IsFalse(datum != otherDatum);

			glm::mat4 matValue;
			matValue[0].a = 2.0f;

			datum.Clear();
			datum = matValue;

			Assert::IsTrue(datum != otherDatum);

			otherDatum = datum;

			Assert::IsFalse(datum != otherDatum);

			std::string stringValue = "Howdy";

			datum.Clear();
			datum = stringValue;

			Assert::IsTrue(datum != otherDatum);

			otherDatum = datum;

			Assert::IsFalse(datum != otherDatum);

			Foo pointerValue(intValue);

			datum.Clear();
			datum = &pointerValue;

			Assert::IsTrue(datum != otherDatum);

			otherDatum = datum;

			Assert::IsFalse(datum != otherDatum);
		}

		TEST_METHOD(DatumIntegerInequality)
		{
			Datum datum;
			std::int32_t value = rand();

			Assert::IsTrue(datum != value);

			datum = value;

			Assert::IsFalse(datum != value);
		}

		TEST_METHOD(DatumFloatInequality)
		{
			Datum datum;
			float value = 2.0f;

			Assert::IsTrue(datum != value);

			datum = value;

			Assert::IsFalse(datum != value);
		}

		TEST_METHOD(DatumVectorInequality)
		{
			Datum datum;
			glm::vec4 value;
			value[0] = 2.0f;

			Assert::IsTrue(datum != value);

			datum = value;

			Assert::IsFalse(datum != value);
		}

		TEST_METHOD(DatumMatrixInequality)
		{
			Datum datum;
			glm::mat4 value;
			value[0].a = 2.0f;

			Assert::IsTrue(datum != value);

			datum = value;

			Assert::IsFalse(datum != value);
		}

		TEST_METHOD(DatumStringInequality)
		{
			Datum datum;
			std::string value = "Howdy";

			Assert::IsTrue(datum != value);

			datum = value;

			Assert::IsFalse(datum != value);
		}

		TEST_METHOD(DatumPointerInequality)
		{
			Datum datum;
			std::int32_t intValue = rand();
			Foo value(intValue);

			Assert::IsTrue(datum != &value);

			datum = &value;

			Assert::IsFalse(datum != &value);
		}

		TEST_METHOD(DatumTableInequality)
		{
			Datum datum;
			Scope value;

			Assert::IsTrue(datum != value);

			datum = value;

			Assert::IsFalse(datum != value);
		}

		TEST_METHOD(DatumIntegerSet)
		{
			Datum datum;
			std::int32_t value = rand(), nextValue = rand();
			std::uint32_t index = 0;

			auto setPointer = [&datum, &value, &index] { datum.Set(value, index); };

			Assert::ExpectException<std::exception>(setPointer);

			++index;

			Assert::ExpectException<std::exception>(setPointer);

			datum.PushBack(nextValue);
			datum.PushBack(nextValue);
			datum.PushBack(nextValue);

			datum.Set(value, --index);

			Assert::IsTrue(datum.Get<std::int32_t&>() == value);
		}

		TEST_METHOD(DatumFloatSet)
		{
			Datum datum;
			float value = 2.0f, nextValue = 4.0f;
			std::uint32_t index = 0;

			auto setPointer = [&datum, &value, &index] { datum.Set(value, index); };

			Assert::ExpectException<std::exception>(setPointer);

			++index;

			Assert::ExpectException<std::exception>(setPointer);

			datum.PushBack(nextValue);
			datum.PushBack(nextValue);
			datum.PushBack(nextValue);

			datum.Set(value, --index);

			Assert::IsTrue(datum.Get<float&>() == value);
		}

		TEST_METHOD(DatumVectorSet)
		{
			Datum datum;
			glm::vec4 value, nextValue;
			value[0] = 2.0f;
			nextValue[0] = 4.0f;
			std::uint32_t index = 0;

			auto setPointer = [&datum, &value, &index] { datum.Set(value, index); };

			Assert::ExpectException<std::exception>(setPointer);

			++index;

			Assert::ExpectException<std::exception>(setPointer);

			datum.PushBack(nextValue);
			datum.PushBack(nextValue);
			datum.PushBack(nextValue);

			datum.Set(value, --index);

			Assert::IsTrue(datum.Get<glm::vec4&>() == value);
		}

		TEST_METHOD(DatumMatrixSet)
		{
			Datum datum;
			glm::mat4 value, nextValue;
			value[0].a = 2.0f;
			nextValue[0].a = 4.0f;
			std::uint32_t index = 0;

			auto setPointer = [&datum, &value, &index] { datum.Set(value, index); };

			Assert::ExpectException<std::exception>(setPointer);

			++index;

			Assert::ExpectException<std::exception>(setPointer);

			datum.PushBack(nextValue);
			datum.PushBack(nextValue);
			datum.PushBack(nextValue);

			datum.Set(value, --index);

			Assert::IsTrue(datum.Get<glm::mat4&>() == value);
		}

		TEST_METHOD(DatumStringSet)
		{
			Datum datum;
			std::string value = "Howdy", nextValue = "Hi";
			std::uint32_t index = 0;

			auto setPointer = [&datum, &value, &index] { datum.Set(value, index); };

			Assert::ExpectException<std::exception>(setPointer);

			++index;

			Assert::ExpectException<std::exception>(setPointer);

			datum.PushBack(nextValue);
			datum.PushBack(nextValue);
			datum.PushBack(nextValue);

			datum.Set(value, --index);

			Assert::IsTrue(datum.Get<std::string&>() == value);
		}

		TEST_METHOD(DatumPointerSet)
		{
			Datum datum;
			std::int32_t intValue = rand(), nextIntValue = rand();
			Foo value(intValue), nextValue(nextIntValue);
			std::uint32_t index = 0;

			auto setPointer = [&datum, &value, &index] { datum.Set(&value, index); };

			Assert::ExpectException<std::exception>(setPointer);

			++index;

			Assert::ExpectException<std::exception>(setPointer);

			datum.PushBack(&nextValue);
			datum.PushBack(&nextValue);
			datum.PushBack(&nextValue);

			datum.Set(&value, --index);

			Assert::IsTrue(datum.Get<RTTI*&>() == &value);
		}

		TEST_METHOD(DatumTableSet)
		{
			Datum datum;
			Scope value, nextValue;
			std::uint32_t index = 0;

			auto setPointer = [&datum, &value, &index] { datum.Set(value, index); };

			Assert::ExpectException<std::exception>(setPointer);

			++index;

			Assert::ExpectException<std::exception>(setPointer);

			datum.PushBack(nextValue);
			datum.PushBack(nextValue);
			datum.PushBack(nextValue);

			datum.Set(value, --index);

			Assert::IsTrue(datum.Get<Scope&>() == value);
		}

		TEST_METHOD(DatumIntegerPushBack)
		{
			Datum datum;
			std::int32_t value = rand(), nextValue = rand();
			std::uint32_t capacity = 0;

			datum.SetType(Datum::DatumType::Pointer);

			auto pushBackPointer = [&datum, &value] { datum.PushBack(value); };

			Assert::ExpectException<std::exception>(pushBackPointer);
			Assert::AreEqual(capacity, datum.Capacity());

			datum.Clear();

			datum.PushBack(value);

			Assert::IsTrue(value == datum.Get<std::int32_t&>(datum.Size() - 1));
			Assert::AreEqual(++capacity, datum.Capacity());

			datum.PushBack(nextValue);

			Assert::IsTrue(nextValue == datum.Get<std::int32_t&>(datum.Size() - 1));
			Assert::AreEqual(++capacity, datum.Capacity());
		}

		TEST_METHOD(DatumFloatPushBack)
		{
			Datum datum;
			float value = 2.0f, nextValue = 4.0f;
			std::uint32_t capacity = 0;

			datum.SetType(Datum::DatumType::Pointer);

			auto pushBackPointer = [&datum, &value] { datum.PushBack(value); };

			Assert::ExpectException<std::exception>(pushBackPointer);
			Assert::AreEqual(capacity, datum.Capacity());

			datum.Clear();

			datum.PushBack(value);

			Assert::IsTrue(value == datum.Get<float&>(datum.Size() - 1));
			Assert::AreEqual(++capacity, datum.Capacity());

			datum.PushBack(nextValue);

			Assert::IsTrue(nextValue == datum.Get<float&>(datum.Size() - 1));
			Assert::AreEqual(++capacity, datum.Capacity());
		}

		TEST_METHOD(DatumVectorPushBack)
		{
			Datum datum;
			glm::vec4 value, nextValue;
			value[0] = 2.0f;
			nextValue[0] = 4.0f;
			std::uint32_t capacity = 0;

			datum.SetType(Datum::DatumType::Pointer);

			auto pushBackPointer = [&datum, &value] { datum.PushBack(value); };

			Assert::ExpectException<std::exception>(pushBackPointer);
			Assert::AreEqual(capacity, datum.Capacity());

			datum.Clear();

			datum.PushBack(value);

			Assert::IsTrue(value == datum.Get<glm::vec4&>(datum.Size() - 1));
			Assert::AreEqual(++capacity, datum.Capacity());

			datum.PushBack(nextValue);

			Assert::IsTrue(nextValue == datum.Get<glm::vec4&>(datum.Size() - 1));
			Assert::AreEqual(++capacity, datum.Capacity());
		}

		TEST_METHOD(DatumMatrixPushBack)
		{
			Datum datum;
			glm::mat4 value, nextValue;
			value[0].a = 2.0f;
			nextValue[0].a = 4.0f;
			std::uint32_t capacity = 0;

			datum.SetType(Datum::DatumType::Pointer);

			auto pushBackPointer = [&datum, &value] { datum.PushBack(value); };

			Assert::ExpectException<std::exception>(pushBackPointer);
			Assert::AreEqual(capacity, datum.Capacity());

			datum.Clear();

			datum.PushBack(value);

			Assert::IsTrue(value == datum.Get<glm::mat4&>(datum.Size() - 1));
			Assert::AreEqual(++capacity, datum.Capacity());

			datum.PushBack(nextValue);

			Assert::IsTrue(nextValue == datum.Get<glm::mat4&>(datum.Size() - 1));
			Assert::AreEqual(++capacity, datum.Capacity());
		}

		TEST_METHOD(DatumStringPushBack)
		{
			Datum datum;
			std::string value = "Howdy", nextValue = "Hi";
			std::uint32_t capacity = 0;

			datum.SetType(Datum::DatumType::Pointer);

			auto pushBackPointer = [&datum, &value] { datum.PushBack(value); };

			Assert::ExpectException<std::exception>(pushBackPointer);
			Assert::AreEqual(capacity, datum.Capacity());

			datum.Clear();

			datum.PushBack(value);

			Assert::IsTrue(value == datum.Get<std::string&>(datum.Size() - 1));
			Assert::AreEqual(++capacity, datum.Capacity());

			datum.PushBack(nextValue);

			Assert::IsTrue(nextValue == datum.Get<std::string&>(datum.Size() - 1));
			Assert::AreEqual(++capacity, datum.Capacity());
		}

		TEST_METHOD(DatumPointerPushBack)
		{
			Datum datum;
			std::int32_t intValue = rand(), nextIntValue = rand();
			Foo value(intValue), nextValue(nextIntValue);
			std::uint32_t capacity = 0;

			datum.SetType(Datum::DatumType::Integer);

			auto pushBackPointer = [&datum, &value] { datum.PushBack(&value); };

			Assert::ExpectException<std::exception>(pushBackPointer);
			Assert::AreEqual(capacity, datum.Capacity());

			datum.Clear();

			datum.PushBack(&value);

			Assert::IsTrue(&value == datum.Get<RTTI*&>(datum.Size() - 1));
			Assert::AreEqual(++capacity, datum.Capacity());

			datum.PushBack(&nextValue);

			Assert::IsTrue(&nextValue == datum.Get<RTTI*&>(datum.Size() - 1));
			Assert::AreEqual(++capacity, datum.Capacity());
		}

		TEST_METHOD(DatumTablePushBack)
		{
			Datum datum;
			Scope value, nextValue;
			std::uint32_t capacity = 0;

			datum.SetType(Datum::DatumType::Integer);

			auto pushBackPointer = [&datum, &value] { datum.PushBack(&value); };

			Assert::ExpectException<std::exception>(pushBackPointer);
			Assert::AreEqual(capacity, datum.Capacity());

			datum.Clear();

			datum.PushBack(value);

			Assert::IsTrue(value == datum.Get<Scope&>(datum.Size() - 1));
			Assert::AreEqual(++capacity, datum.Capacity());

			datum.PushBack(nextValue);

			Assert::IsTrue(nextValue == datum.Get<Scope&>(datum.Size() - 1));
			Assert::AreEqual(++capacity, datum.Capacity());
		}

		TEST_METHOD(DatumIntegerGet)
		{
			Datum datum;
			std::int32_t value = rand();
			std::uint32_t index = 0;

			auto getPointer = [&datum, &index] { datum.Get<std::int32_t&>(index); };

			Assert::ExpectException<std::exception>(getPointer);

			datum.SetType(Datum::DatumType::Pointer);
			datum.SetSize(1);

			Assert::ExpectException<std::exception>(getPointer);

			datum.Clear();
			datum = value;

			Assert::IsTrue(datum.Get<std::int32_t&>() == value);

			const Datum otherDatum = datum;

			Assert::IsTrue(otherDatum.Get<const std::int32_t&>() == value);
		}

		TEST_METHOD(DatumFloatGet)
		{
			Datum datum;
			float value = 2.0f;
			std::uint32_t index = 0;

			auto getPointer = [&datum, &index] { datum.Get<float&>(index); };

			Assert::ExpectException<std::exception>(getPointer);

			datum.SetType(Datum::DatumType::Pointer);
			datum.SetSize(1);

			Assert::ExpectException<std::exception>(getPointer);

			datum.Clear();
			datum = value;

			Assert::IsTrue(datum.Get<float&>() == value);

			const Datum otherDatum = datum;

			Assert::IsTrue(otherDatum.Get <const float&> () == value);
		}

		TEST_METHOD(DatumVectorGet)
		{
			Datum datum;
			glm::vec4 value;
			value[0] = 2.0f;
			std::uint32_t index = 0;

			auto getPointer = [&datum, &index] { datum.Get<glm::vec4&>(index); };

			Assert::ExpectException<std::exception>(getPointer);

			datum.SetType(Datum::DatumType::Pointer);
			datum.SetSize(1);

			Assert::ExpectException<std::exception>(getPointer);

			datum.Clear();
			datum = value;

			Assert::IsTrue(datum.Get<glm::vec4&>() == value);

			const Datum otherDatum = datum;

			Assert::IsTrue(otherDatum.Get<const glm::vec4&>() == value);
		}

		TEST_METHOD(DatumMatrixGet)
		{
			Datum datum;
			glm::mat4 value;
			value[0].a = 2.0f;
			std::uint32_t index = 0;

			auto getPointer = [&datum, &index] { datum.Get<glm::mat4&>(index); };

			Assert::ExpectException<std::exception>(getPointer);

			datum.SetType(Datum::DatumType::Pointer);
			datum.SetSize(1);

			Assert::ExpectException<std::exception>(getPointer);

			datum.Clear();
			datum = value;

			Assert::IsTrue(datum.Get<glm::mat4&>() == value);

			const Datum otherDatum = datum;

			Assert::IsTrue(otherDatum.Get<const glm::mat4&>() == value);
		}

		TEST_METHOD(DatumStringGet)
		{
			Datum datum;
			std::string value = "Howdy";
			std::uint32_t index = 0;

			auto getPointer = [&datum, &index] { datum.Get<std::string&>(index); };

			Assert::ExpectException<std::exception>(getPointer);

			datum.SetType(Datum::DatumType::Pointer);
			datum.SetSize(1);

			Assert::ExpectException<std::exception>(getPointer);

			datum.Clear();
			datum = value;

			Assert::IsTrue(datum.Get<std::string&>() == value);

			const Datum otherDatum = datum;

			Assert::IsTrue(otherDatum.Get<const std::string&>() == value);
		}

		TEST_METHOD(DatumPointerGet)
		{
			Datum datum;
			std::int32_t intValue = rand();
			Foo value(intValue);
			std::uint32_t index = 0;

			auto getPointer = [&datum, &index] { datum.Get<RTTI*&>(index); };

			Assert::ExpectException<std::exception>(getPointer);

			datum.SetType(Datum::DatumType::Integer);
			datum.SetSize(1);

			Assert::ExpectException<std::exception>(getPointer);

			datum.Clear();
			datum = &value;

			Assert::IsTrue(datum.Get<RTTI*&>() == &value);

			const Datum otherDatum = datum;

			Assert::IsTrue(otherDatum.Get<const RTTI*&>() == &value);
		}

		TEST_METHOD(DatumTableGet)
		{
			Datum datum;
			Scope value;
			std::uint32_t index = 0;

			auto getPointer = [&datum, &index] { datum.Get<Scope&>(index); };

			Assert::ExpectException<std::exception>(getPointer);

			datum.SetType(Datum::DatumType::Integer);
			datum.SetSize(1);

			Assert::ExpectException<std::exception>(getPointer);

			datum.Clear();
			datum = value;

			Assert::IsTrue(datum.Get<Scope&>() == value);

			const Datum otherDatum = datum;

			Assert::IsTrue(otherDatum.Get<const Scope&>() == value);
		}

		TEST_METHOD(DatumSetFromString)
		{
			char buffer[1000];
			Datum datum, otherDatum;
			std::uint32_t index = 0;

			auto setFromStringPointer = [&datum, &index] { datum.SetFromString("!", index); };

			Assert::ExpectException<std::exception>(setFromStringPointer);

			std::int32_t intValue = rand();
			datum = intValue;
			otherDatum = datum;

			Assert::ExpectException<std::exception>(setFromStringPointer);

			otherDatum.SetFromString(otherDatum.ToString(index), index);

			Assert::IsTrue(datum == otherDatum);

			datum.Clear();
			otherDatum.Clear();

			float floatValue = 2.0f;
			datum = floatValue;
			otherDatum = datum;

			Assert::ExpectException<std::exception>(setFromStringPointer);

			otherDatum.SetFromString(otherDatum.ToString(index), index);

			Assert::IsTrue(datum == otherDatum);

			datum.Clear();
			otherDatum.Clear();

			glm::vec4 vectorValue;
			vectorValue[0] = 2.0f;
			datum = vectorValue;
			otherDatum = datum;

			Assert::ExpectException<std::exception>(setFromStringPointer);

			otherDatum.SetFromString(otherDatum.ToString(index), index);

			Assert::IsTrue(datum == otherDatum);

			datum.Clear();
			otherDatum.Clear();

			glm::mat4 matrixValue;
			matrixValue[0].a = 2.0f;
			datum = matrixValue;
			otherDatum = datum;

			Assert::ExpectException<std::exception>(setFromStringPointer);

			otherDatum.SetFromString(otherDatum.ToString(index), index);

			Assert::IsTrue(datum == otherDatum);

			datum.Clear();
			otherDatum.Clear();

			std::string stringValue = "Howdy";
			datum = stringValue;
			otherDatum = datum;

			otherDatum.SetFromString(otherDatum.ToString(index), index);

			Assert::IsTrue(datum == otherDatum);

			datum.Clear();
			otherDatum.Clear();

			Foo pointerValue(intValue);
			datum = &pointerValue;
			otherDatum = datum;

			Assert::ExpectException<std::exception>(setFromStringPointer);

			datum.Clear();
			otherDatum.Clear();

			Scope tableValue;
			datum = &tableValue;
			otherDatum = datum;

			Assert::ExpectException<std::exception>(setFromStringPointer);
		}

		TEST_METHOD(DatumToString)
		{
			char buffer[1000];
			Datum datum, otherDatum;
			std::uint32_t index = 0;

			auto toStringPointer = [&datum, &index] { datum.ToString(index); };

			Assert::ExpectException<std::exception>(toStringPointer);

			std::int32_t intValue = rand();
			datum = intValue;
			otherDatum = datum;

			otherDatum.SetFromString(otherDatum.ToString(index), index);

			Assert::IsTrue(datum == otherDatum);

			datum.Clear();
			otherDatum.Clear();

			float floatValue = 2.0f;
			datum = floatValue;
			otherDatum = datum;

			otherDatum.SetFromString(otherDatum.ToString(index), index);

			Assert::IsTrue(datum == otherDatum);

			datum.Clear();
			otherDatum.Clear();

			glm::vec4 vectorValue;
			vectorValue[0] = 2.0f;
			datum = vectorValue;
			otherDatum = datum;

			otherDatum.SetFromString(otherDatum.ToString(index), index);

			Assert::IsTrue(datum == otherDatum);

			datum.Clear();
			otherDatum.Clear();

			glm::mat4 matrixValue;
			matrixValue[0].a = 2.0f;
			datum = matrixValue;
			otherDatum = datum;

			otherDatum.SetFromString(otherDatum.ToString(index), index);

			Assert::IsTrue(datum == otherDatum);

			datum.Clear();
			otherDatum.Clear();

			std::string stringValue = "Howdy";
			datum = stringValue;
			otherDatum = datum;

			otherDatum.SetFromString(otherDatum.ToString(index), index);

			Assert::IsTrue(datum == otherDatum);

			datum.Clear();
			otherDatum.Clear();

			Foo pointerValue(intValue);
			datum = &pointerValue;
			otherDatum = datum;

			Assert::IsTrue(datum.ToString() == otherDatum.ToString());

			datum.Clear();
			otherDatum.Clear();

			Scope tableValue;
			datum = &tableValue;
			otherDatum = datum;

			Assert::IsTrue(datum.ToString() == otherDatum.ToString());
		}

		TEST_METHOD(DatumReserve)
		{
			Datum datum = Datum();
			std::uint32_t capacity = 1;

			auto reservePointer = [&datum, &capacity] { datum.Reserve(capacity); };

			Assert::ExpectException<std::exception>(reservePointer);

			datum.SetType(Datum::DatumType::Integer);
			datum.Reserve(capacity);

			Assert::AreEqual(capacity, datum.Capacity());

			datum.Clear();
			datum.SetType(Datum::DatumType::Float);
			datum.Reserve(capacity);

			Assert::AreEqual(capacity, datum.Capacity());

			datum.Clear();
			datum.SetType(Datum::DatumType::Vec4);
			datum.Reserve(capacity);

			Assert::AreEqual(capacity, datum.Capacity());

			datum.Clear();
			datum.SetType(Datum::DatumType::Mat4);
			datum.Reserve(capacity);

			Assert::AreEqual(capacity, datum.Capacity());

			datum.Clear();
			datum.SetType(Datum::DatumType::String);
			datum.Reserve(capacity);

			Assert::AreEqual(capacity, datum.Capacity());

			datum.Clear();
			datum.SetType(Datum::DatumType::Pointer);
			datum.Reserve(capacity);

			Assert::AreEqual(capacity, datum.Capacity());

			datum.Clear();
			datum.SetType(Datum::DatumType::Table);
			datum.Reserve(capacity);

			Assert::AreEqual(capacity, datum.Capacity());

			std::int32_t* storage = static_cast<std::int32_t*>(malloc(sizeof(std::int32_t)));
			datum.Clear();
			datum.SetStorage(storage, 1);

			Assert::ExpectException<std::exception>(reservePointer);

			free(storage);
		}

		TEST_METHOD(DatumRemove)
		{
			Scope scope, nextScope, thirdScope;

			Datum datum;

			datum.PushBack(scope);
			datum.PushBack(nextScope);
			datum.PushBack(thirdScope);

			datum.Remove(nextScope);

			Assert::IsTrue(&datum.Get<Scope&>(1U) != &nextScope);
			Assert::AreEqual(2U, datum.Size());
		}

	private:

		static _CrtMemState sStartMemState;
	};

	_CrtMemState DatumTest::sStartMemState;
}
