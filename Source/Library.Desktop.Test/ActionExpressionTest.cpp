#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(ActionExpressionTest)
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

		TEST_METHOD(ActionExpressionConstructor)
		{
			ActionExpression expression;

			Assert::IsTrue(expression.Name() == "");
			Assert::IsTrue(expression.IsPrescribedAttribute(ActionExpression::sActionExpressionValueKey));
		}

		TEST_METHOD(ActionExpressionDestructor)
		{
			ActionExpression expression;

			expression.SetExpression("hello world");
		}

		TEST_METHOD(ActionExpressionCopyConstructor)
		{
			std::string name = "name";
			ActionExpression expression(name);
			expression.SetExpression(name);
			
			ActionExpression otherExpression = expression;

			Assert::IsTrue(otherExpression.GetRPN() == expression.GetRPN());
			Assert::IsTrue(otherExpression.GetRPN() == name + " ");
		}

		TEST_METHOD(ActionExpressionAssignmentOperator)
		{
			std::string name = "name", otherName = "otherName";
			ActionExpression expression(name);
			expression.SetExpression(name);

			ActionExpression otherExpression(otherName);
			otherExpression.SetExpression(otherName);
			
			otherExpression = expression;

			Assert::IsTrue(otherExpression.GetRPN() == expression.GetRPN());
			Assert::IsTrue(otherExpression.GetRPN() == name + " ");
		}

		TEST_METHOD(ActionExpressionMoveConstructor)
		{
			std::string name = "name";
			ActionExpression expression(name);
			expression.SetExpression(name);

			ActionExpression otherExpression = std::move(expression);

			Assert::IsTrue(otherExpression.GetRPN() == name + " ");
			Assert::IsTrue(expression.GetRPN() == std::string());
		}

		TEST_METHOD(ActionExpressionMoveAssignmentOperator)
		{
			std::string name = "name", otherName = "otherName";
			ActionExpression expression(name);
			expression.SetExpression(name);

			ActionExpression otherExpression(otherName);
			otherExpression = std::move(expression);

			Assert::IsTrue(otherExpression.GetRPN() == name + " ");
			Assert::IsTrue(expression.GetRPN() == std::string());
		}

		TEST_METHOD(ActionExpressionSetExpression)
		{
			ActionExpression expression;

			expression.SetExpression("hello");

			Assert::IsTrue(expression.GetRPN() == "hello ");

			expression.SetExpression("5 + 2");

			Assert::IsTrue(expression.GetRPN() == "5 2 + ");

			expression.SetExpression("5 + 2 * 3");

			Assert::IsTrue(expression.GetRPN() == "5 2 3 * + ");

			expression.SetExpression("5 + 2 * 3 + 4");

			Assert::IsTrue(expression.GetRPN() == "5 2 3 * + 4 + ");

			expression.SetExpression("5 + ( ( 1 + 2 ) * 4 ) - 3");

			Assert::IsTrue(expression.GetRPN() == "5 1 2 + 4 * + 3 - ");

			expression.SetExpression("5 + ( 12 * value * scope [ index + 15 ] ) ");

			Assert::IsTrue(expression.GetRPN() == "5 12 value * scope index 15 + [ * + ");
		}

		TEST_METHOD(ActionExpressionEvaluate)
		{
			ActionExpression expression;

			expression.SetExpression("5 + 2 + 18");

			Datum value = expression.Evaluate();

			Assert::IsTrue(value == 25);

			expression.Append("thing") = 20;
			expression.SetExpression("thing");
			value = expression.Evaluate();

			Assert::IsTrue(value == 20);

			expression.AppendScope("child");
			expression["child"].Get<Scope&>().Append("number") = 15;

			expression.SetExpression("child . number");
			value = expression.Evaluate();

			Assert::IsTrue(value == 15);

			expression.SetExpression("child . number / 5 * 4");
			value = expression.Evaluate();

			Assert::IsTrue(value == 12);

			expression.SetExpression("5 + ( ( 1 + 2 ) * 4 ) - 3");
			value = expression.Evaluate();

			Assert::IsTrue(value == 14);

			expression.SetExpression("( child . number >= 15 ) && ( ( child . number - 12 ) == 3 )");
			value = expression.Evaluate();

			Assert::IsTrue(value == 1);

			expression.SetExpression("( child . number >= 15 ) && ( child . number <= 15 ) && ( child . number == 15 ) && ! ( child . number != 15 )");
			value = expression.Evaluate();

			Assert::IsTrue(value == 1);

			expression.SetExpression("( child . number [ 0 ] ) == 15");
			value = expression.Evaluate();

			Assert::IsTrue(value == 1);

			expression.SetExpression("defaultedLiteral == defaultedLiteral");
			value = expression.Evaluate();

			Assert::IsTrue(value == 1);

			expression.SetExpression("56 % 6");
			value = expression.Evaluate();

			Assert::IsTrue(value == 2);
		}

		TEST_METHOD(ActionExpressionOperators)
		{
			ActionExpression expression;
			glm::vec4 vector(1.0f, 1.0f, 1.0f, 1.0f);
			glm::mat4 matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

			expression.AppendAuxiliaryAttribute("vector") = vector;
			expression.AppendAuxiliaryAttribute("matrix") = matrix;
			Scope scope = expression.AppendScope("scope");

			auto evaluate = [&expression] { return expression.Evaluate(); };

			expression.SetExpression("2 * 3");
			Assert::IsTrue(evaluate() == 6);

			expression.SetExpression("2.0 * 3.0");
			Assert::IsTrue(evaluate() == 6.0f);

			expression.SetExpression("vector * vector");
			Assert::IsTrue(evaluate() == (vector * vector));

			expression.SetExpression("matrix * matrix");
			Assert::IsTrue(evaluate() == (matrix * matrix));

			expression.SetExpression("default * otherThing");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("6 / 3");
			Assert::IsTrue(evaluate() == 2);

			expression.SetExpression("3.0 / 2.0");
			Assert::IsTrue(evaluate() == 1.5f);

			expression.SetExpression("vector / vector");
			Assert::IsTrue(evaluate() == (vector / vector));

			expression.SetExpression("matrix / matrix");
			Assert::IsTrue(evaluate() == (matrix / matrix));

			expression.SetExpression("default / otherThing");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("40 % 7");
			Assert::IsTrue(evaluate() == 5);

			expression.SetExpression("40.0 % 7.0");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("2 + 3");
			Assert::IsTrue(evaluate() == 5);

			expression.SetExpression("2.0 + 3.0");
			Assert::IsTrue(evaluate() == 5.0f);

			expression.SetExpression("vector + vector");
			Assert::IsTrue(evaluate() == (vector + vector));

			expression.SetExpression("matrix + matrix");
			Assert::IsTrue(evaluate() == (matrix + matrix));

			expression.SetExpression("string + otherString");
			Assert::IsTrue(evaluate() == "stringotherString");

			expression.SetExpression("scope + scope");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("3 - 2");
			Assert::IsTrue(evaluate() == 1);

			expression.SetExpression("3.0 - 2.0");
			Assert::IsTrue(evaluate() == 1.0f);

			expression.SetExpression("vector - vector");
			Assert::IsTrue(evaluate() == (vector - vector));

			expression.SetExpression("matrix - matrix");
			Assert::IsTrue(evaluate() == (matrix - matrix));

			expression.SetExpression("string - otherString");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("3 < 2");
			Assert::IsTrue(evaluate() == 0);

			expression.SetExpression("3.0 < 2.0");
			Assert::IsTrue(evaluate() == 0);

			expression.SetExpression("string < otherString");
			Assert::IsTrue(evaluate() == 0);
			
			expression.SetExpression("vector < vector");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("3 <= 2");
			Assert::IsTrue(evaluate() == 0);

			expression.SetExpression("3.0 <= 2.0");
			Assert::IsTrue(evaluate() == 0);

			expression.SetExpression("string <= otherString");
			Assert::IsTrue(evaluate() == 0);

			expression.SetExpression("vector <= vector");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("3 > 2");
			Assert::IsTrue(evaluate() == 1);

			expression.SetExpression("3.0 > 2.0");
			Assert::IsTrue(evaluate() == 1);

			expression.SetExpression("string > otherString");
			Assert::IsTrue(evaluate() == 1);

			expression.SetExpression("vector > vector");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("3 >= 2");
			Assert::IsTrue(evaluate() == 1);

			expression.SetExpression("3.0 >= 2.0");
			Assert::IsTrue(evaluate() == 1);

			expression.SetExpression("string >= otherString");
			Assert::IsTrue(evaluate() == 1);

			expression.SetExpression("vector >= vector");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("0 && 1");
			Assert::IsTrue(evaluate() == 0);

			expression.SetExpression("0.0 && 1.0");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("0 || 1");
			Assert::IsTrue(evaluate() == 1);

			expression.SetExpression("0.0 || 1.0");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("! 1");
			Assert::IsTrue(evaluate() == 0);

			expression.SetExpression("! 1.0");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("0 [ 0 ]");
			Assert::IsTrue(evaluate() == 0);

			expression.SetExpression("0.0 [ 0 ]");
			Assert::IsTrue(evaluate() == 0.0f);

			expression.SetExpression("vector [ 0 ]");
			Assert::IsTrue(evaluate() == vector);

			expression.SetExpression("matrix [ 0 ]");
			Assert::IsTrue(evaluate() == matrix);

			expression.SetExpression("this [ 0 ]");
			Assert::IsTrue(evaluate() == &expression);

			expression.SetExpression("scope [ 0 ]");
			Assert::IsTrue(evaluate() == scope);

			expression.SetExpression("sin 1.0f");
			Assert::IsTrue(evaluate() == sin(1.0f));

			expression.SetExpression("sin vector");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("cos 1.0f");
			Assert::IsTrue(evaluate() == cos(1.0f));

			expression.SetExpression("cos vector");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("tan 1.0f");
			Assert::IsTrue(evaluate() == tan(1.0f));

			expression.SetExpression("tan vector");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("asin 1.0f");
			Assert::IsTrue(evaluate() == asin(1.0f));

			expression.SetExpression("asin vector");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("acos 1.0f");
			Assert::IsTrue(evaluate() == acos(1.0f));

			expression.SetExpression("acos vector");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("atan 1.0f");
			Assert::IsTrue(evaluate() == atan(1.0f));

			expression.SetExpression("atan vector");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("log2 1.0f");
			Assert::IsTrue(evaluate() == log2(1.0f));

			expression.SetExpression("log2 vector");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("log 1.0f");
			Assert::IsTrue(evaluate() == log(1.0f));

			expression.SetExpression("log vector");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("log10 1.0f");
			Assert::IsTrue(evaluate() == log10(1.0f));

			expression.SetExpression("log10 vector");
			Assert::ExpectException<std::exception>(evaluate);

			expression.SetExpression("sqrt 1.0f");
			Assert::IsTrue(evaluate() == sqrt(1.0f));

			expression.SetExpression("sqrt vector");
			Assert::ExpectException<std::exception>(evaluate);
		}

		TEST_METHOD(ActionExpressionUpdate)
		{
			ActionExpression expression;
			WorldState worldState;

			expression.Update(worldState);
		}

		TEST_METHOD(ActionExpressionCopy)
		{
			std::string stringValue = "Howdy", nextStringValue = "Hi", thirdStringValue = "Hello";
			ActionExpression expression, childOne, childTwo, childThree;

			expression.Adopt(childOne, stringValue);
			expression.Adopt(childTwo, nextStringValue);
			expression.Adopt(childThree, thirdStringValue);

			Scope* copy = expression.Copy();

			Assert::IsTrue(*copy == expression);
			Assert::IsTrue(copy->Find(stringValue)->Get<Scope&>().Is(ActionExpression::TypeIdClass()));
			Assert::IsTrue(copy->Find(nextStringValue)->Get<Scope&>().Is(ActionExpression::TypeIdClass()));
			Assert::IsTrue(copy->Find(thirdStringValue)->Get<Scope&>().Is(ActionExpression::TypeIdClass()));

			delete(copy);
		}

		TEST_METHOD(ActionExpressionRTTIMethods)
		{
			ActionExpression expression;

			Assert::IsTrue(expression.Is(ActionExpression::TypeIdClass()));
			Assert::IsTrue(expression.Is(ActionExpression::TypeName()));
			Assert::AreEqual(ActionExpression::TypeIdClass(), expression.TypeIdInstance());
			Assert::AreEqual(ActionExpression::TypeName(), expression.TypeName());

			RTTI* asRTTI = &expression;

			Assert::IsFalse(nullptr == asRTTI->QueryInterface(ActionExpression::TypeIdClass()));
			Assert::IsTrue(expression == *(asRTTI->As<ActionExpression>()));
			Assert::AreEqual(asRTTI->As<ActionExpression>()->ToString(), expression.ToString());
		}

	private:

		static _CrtMemState sStartMemState;

	};

	_CrtMemState ActionExpressionTest::sStartMemState;
}
