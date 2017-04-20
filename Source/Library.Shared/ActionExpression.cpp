#include "pch.h"

namespace FieaGameEngine
{

#pragma region ActionExpression Implementation

	RTTI_DEFINITIONS(ActionExpression)

	ActionExpression::ActionExpression(const std::string& name) :
		Action(name)
	{
		InitializeSignatures();
	}

	ActionExpression::ActionExpression(const ActionExpression& otherActionExpression) :
		Action(otherActionExpression), mExpression(otherActionExpression.mExpression)
	{
		FixExternalAttributes();
	}

	ActionExpression& ActionExpression::operator=(const ActionExpression& otherActionExpression)
	{
		if (this != &otherActionExpression)
		{
			Action::operator=(otherActionExpression);

			CopyPrivateDataMembers(otherActionExpression);
		}

		return *this;
	}

	ActionExpression::ActionExpression(ActionExpression&& previousActionExpression) :
		Action(previousActionExpression), mExpression(previousActionExpression.mExpression)
	{
		FixExternalAttributes();

		previousActionExpression.mExpression = std::string();
	}

	ActionExpression& ActionExpression::operator=(ActionExpression&& previousActionExpression)
	{
		if (this != &previousActionExpression)
		{
			Action::operator=(previousActionExpression);

			CopyPrivateDataMembers(previousActionExpression);

			previousActionExpression.mExpression = std::string();
		}

		return *this;
	}

	void ActionExpression::SetExpression(const std::string& expression)
	{
		ComputeRPN(expression, mExpression);
	}

	const std::string& ActionExpression::GetRPN() const
	{
		return mExpression;
	}

	const Datum& ActionExpression::Evaluate()
	{
		mValue.Clear();

		std::string buffer = "";

		Vector<Datum> valueStack;

		for (const char character : mExpression)
		{
			if (character == sTokenDelineator)
			{
				EvaluateSymbol(buffer, valueStack);
			}
			else
			{
				buffer += character;
			}
		}

		EvaluateSymbol(buffer, valueStack);

		if (!valueStack.IsEmpty())
		{
			mValue = valueStack.Front();
		}

		return mValue;
	}

	void ActionExpression::Update(class WorldState&)
	{

	}

	Scope* ActionExpression::Copy() const
	{
		return new ActionExpression(*this);
	}

	void ActionExpression::InitializeSignatures()
	{
		Action::InitializeSignatures();

		AddExternalAttribute(sActionExpressionValueKey, &mExpression, 1);
	}

	void ActionExpression::CopyPrivateDataMembers(const ActionExpression& otherActionExpression)
	{
		mExpression = otherActionExpression.mExpression;

		FixExternalAttributes();
	}

	void ActionExpression::FixExternalAttributes()
	{
		Append(sActionExpressionValueKey).SetStorage(&mExpression, 1);
	}

	void ActionExpression::ComputeRPN(const std::string& expression, std::string& destination)
	{
		destination.clear();

		std::string buffer = "";

		Vector<std::string> operatorStack;

		for (const char character : expression)
		{
			if (character == sTokenDelineator)
			{
				ProcessAsSymbol(buffer, operatorStack, destination);
			}
			else
			{
				buffer += character;
			}
		}

		ProcessAsSymbol(buffer, operatorStack, destination);

		while (!operatorStack.IsEmpty())
		{
			PopIntoDestination(operatorStack, destination);
		}
	}

	void ActionExpression::ProcessAsSymbol(std::string& buffer, Vector<std::string>& operatorStack, std::string& destination)
	{
		if (buffer.length() > 0)
		{
			if (sOperatorPrecedenceMap.ContainsKey(buffer))
			{
				if (!operatorStack.IsEmpty())
				{
					if (buffer == sRightParen)
					{
						while (!operatorStack.IsEmpty() && operatorStack[operatorStack.Size() - 1] != sLeftParen)
						{
							PopIntoDestination(operatorStack, destination);
						}

						operatorStack.PopBack();
					}
					else if (buffer == sRightBracket)
					{
						while (!operatorStack.IsEmpty() && operatorStack[operatorStack.Size() - 1] != sLeftBracket)
						{
							PopIntoDestination(operatorStack, destination);
						}
					}
					else if (buffer != sLeftParen && buffer != sLeftBracket)
					{
						if (sOperatorAssociativitiyMap[buffer] == AssociativityType::Left)
						{
							while (!operatorStack.IsEmpty() && sOperatorPrecedenceMap[buffer] >= sOperatorPrecedenceMap[operatorStack[operatorStack.Size() - 1]])
							{
								PopIntoDestination(operatorStack, destination);
							}
						}
						else
						{
							while (!operatorStack.IsEmpty() && sOperatorPrecedenceMap[buffer] > sOperatorPrecedenceMap[operatorStack[operatorStack.Size() - 1]])
							{
								PopIntoDestination(operatorStack, destination);
							}
						}
					}
				}

				if (buffer != sRightParen && buffer != sRightBracket)
				{
					operatorStack.PushBack(buffer);
				}
			}
			else
			{
				if (!operatorStack.IsEmpty() && operatorStack[operatorStack.Size() - 1] == sDot)
				{
					destination += sLiteralWrapper;
					destination += buffer + sLiteralWrapper;
				}
				else
				{
					destination += buffer;
				}
				
				destination += sTokenDelineator;
			}

			buffer.clear();
		}
	}

	void ActionExpression::PopIntoDestination(Vector<std::string>& operatorStack, std::string& destination)
	{
		destination += operatorStack[operatorStack.Size() - 1];
		destination += sTokenDelineator;

		operatorStack.PopBack();
	}

	void ActionExpression::EvaluateSymbol(std::string& buffer, Vector<Datum>& valueStack)
	{
		if (buffer.length() > 0)
		{
			valueStack;

			if (buffer[0] == sLiteralWrapper)
			{
				Datum value;
				value.SetType(Datum::DatumType::String);

				buffer = buffer.substr(0, buffer.size() - 1);
				buffer = buffer.substr(1, buffer.size() - 1);

				value.PushBackFromString(buffer);

				valueStack.PushBack(value);
			}
			else if (sOperatorPrecedenceMap.ContainsKey(buffer))
			{
				Datum& right = valueStack[valueStack.Size() - 1];
				
				Datum value;

				if (sOperationFunctionMap.ContainsKey(buffer))
				{
					Datum& left = valueStack[valueStack.Size() - 2];

					value = (const_cast<ActionExpression*>(this)->*sOperationFunctionMap[buffer])(left, right);

					valueStack.PopBack();
				}
				else
				{
					value = (const_cast<ActionExpression*>(this)->*sSingleInputOperationFunctionMap[buffer])(right);
				}

				valueStack.PopBack();

				valueStack.PushBack(value);
			}
			else
			{
				Datum value;

				if (!EvaluateAsLiteralSymbol(buffer, value))
				{
					Datum* found = Search(buffer);

					if (found != nullptr)
					{
						value = *found;
					}
					else
					{
						value = buffer;
					}
				}

				valueStack.PushBack(value);
			}

			buffer.clear();
		}
	}

	bool ActionExpression::EvaluateAsLiteralSymbol(std::string& buffer, Datum& value)
	{
		try
		{
			value.Clear();

			if (buffer.find(sDot) == std::string::npos)
			{
				value.SetType(Datum::DatumType::Integer);
			}
			else
			{
				value.SetType(Datum::DatumType::Float);
			}
			
			value.PushBackFromString(buffer);
		}
		catch (...)
		{
			value.Clear();

			return false;
		}
		
		return true;
	}

	Datum ActionExpression::DotOperation(Datum& left, Datum& right)
	{
		return *left.Get<Scope&>().Search(right.Get<std::string&>());
	}

	Datum ActionExpression::MultiplyOperation(Datum& left, Datum& right)
	{
		Datum value;

		switch (left.Type())
		{

		case Datum::DatumType::Integer:
			value = left.Get<std::int32_t&>() * right.Get<std::int32_t&>();
			break;

		case Datum::DatumType::Float:
			value = left.Get<float&>() * right.Get<float&>();
			break;

		case Datum::DatumType::Vec4:
			value = left.Get<glm::vec4&>() * right.Get<glm::vec4&>();
			break;

		case Datum::DatumType::Mat4:
			value = left.Get<glm::mat4&>() * right.Get<glm::mat4&>();
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::DivideOperation(Datum& left, Datum& right)
	{
		Datum value;

		switch (left.Type())
		{

		case Datum::DatumType::Integer:
			value = left.Get<std::int32_t&>() / right.Get<std::int32_t&>();
			break;

		case Datum::DatumType::Float:
			value = left.Get<float&>() / right.Get<float&>();
			break;

		case Datum::DatumType::Vec4:
			value = left.Get<glm::vec4&>() / right.Get<glm::vec4&>();
			break;

		case Datum::DatumType::Mat4:
			value = left.Get<glm::mat4&>() / right.Get<glm::mat4&>();
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::ModulusOperation(Datum& left, Datum& right)
	{
		Datum value;

		switch (left.Type())
		{

		case Datum::DatumType::Integer:
			value = left.Get<std::int32_t&>() % right.Get<std::int32_t&>();
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::AddOperation(Datum& left, Datum& right)
	{
		Datum value;

		switch (left.Type())
		{

		case Datum::DatumType::Integer:
			value = left.Get<std::int32_t&>() + right.Get<std::int32_t&>();
			break;

		case Datum::DatumType::Float:
			value = left.Get<float&>() + right.Get<float&>();
			break;

		case Datum::DatumType::Vec4:
			value = left.Get<glm::vec4&>() + right.Get<glm::vec4&>();
			break;

		case Datum::DatumType::Mat4:
			value = left.Get<glm::mat4&>() + right.Get<glm::mat4&>();
			break;

		case Datum::DatumType::String:
			value = left.Get<std::string&>() + right.Get<std::string&>();
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::SubtractOperation(Datum& left, Datum& right)
	{
		Datum value;

		switch (left.Type())
		{

		case Datum::DatumType::Integer:
			value = left.Get<std::int32_t&>() - right.Get<std::int32_t&>();
			break;

		case Datum::DatumType::Float:
			value = left.Get<float&>() - right.Get<float&>();
			break;

		case Datum::DatumType::Vec4:
			value = left.Get<glm::vec4&>() - right.Get<glm::vec4&>();
			break;

		case Datum::DatumType::Mat4:
			value = left.Get<glm::mat4&>() - right.Get<glm::mat4&>();
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::LessOperation(Datum& left, Datum& right)
	{
		Datum value;

		switch (left.Type())
		{

		case Datum::DatumType::Integer:
			value = left.Get<std::int32_t&>() < right.Get<std::int32_t&>();
			break;

		case Datum::DatumType::Float:
			value = left.Get<float&>() < right.Get<float&>();
			break;

		case Datum::DatumType::String:
			value = left.Get<std::string&>() < right.Get<std::string&>();
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::LessEqualOperation(Datum& left, Datum& right)
	{
		Datum value;

		switch (left.Type())
		{

		case Datum::DatumType::Integer:
			value = left.Get<std::int32_t&>() <= right.Get<std::int32_t&>();
			break;

		case Datum::DatumType::Float:
			value = left.Get<float&>() <= right.Get<float&>();
			break;

		case Datum::DatumType::String:
			value = left.Get<std::string&>() <= right.Get<std::string&>();
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::GreaterOperation(Datum& left, Datum& right)
	{
		Datum value;

		switch (left.Type())
		{

		case Datum::DatumType::Integer:
			value = left.Get<std::int32_t&>() > right.Get<std::int32_t&>();
			break;

		case Datum::DatumType::Float:
			value = left.Get<float&>() > right.Get<float&>();
			break;

		case Datum::DatumType::String:
			value = left.Get<std::string&>() > right.Get<std::string&>();
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::GreaterEqualOperation(Datum& left, Datum& right)
	{
		Datum value;

		switch (left.Type())
		{

		case Datum::DatumType::Integer:
			value = left.Get<std::int32_t&>() >= right.Get<std::int32_t&>();
			break;

		case Datum::DatumType::Float:
			value = left.Get<float&>() >= right.Get<float&>();
			break;

		case Datum::DatumType::String:
			value = left.Get<std::string&>() >= right.Get<std::string&>();
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::EqualOperation(Datum& left, Datum& right)
	{
		Datum value;
		value = left == right;

		return value;
	}

	Datum ActionExpression::NotEqualOperation(Datum& left, Datum& right)
	{
		Datum value;

		value = left != right;

		return value;
	}

	Datum ActionExpression::AndOperation(Datum& left, Datum& right)
	{
		Datum value;

		switch (left.Type())
		{

		case Datum::DatumType::Integer:
			value = left.Get<std::int32_t&>() && right.Get<std::int32_t&>();
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::OrOperation(Datum& left, Datum& right)
	{
		Datum value;

		switch (left.Type())
		{

		case Datum::DatumType::Integer:
			value = left.Get<std::int32_t&>() || right.Get<std::int32_t&>();
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::BracketOperation(Datum& left, Datum& right)
	{
		Datum value;

		switch (left.Type())
		{

		case Datum::DatumType::Integer:
			value = left.Get<std::int32_t&>(right.Get<std::int32_t&>());
			break;

		case Datum::DatumType::Float:
			value = left.Get<float&>(right.Get<std::int32_t&>());
			break;

		case Datum::DatumType::Vec4:
			value = left.Get<glm::vec4&>(right.Get<std::int32_t&>());
			break;

		case Datum::DatumType::Mat4:
			value = left.Get<glm::mat4&>(right.Get<std::int32_t&>());
			break;

		case Datum::DatumType::String:
			value = left.Get<std::string&>(right.Get<std::int32_t&>());
			break;

		case Datum::DatumType::Pointer:
			value = left.Get<RTTI*&>(right.Get<std::int32_t&>());
			break;

		case Datum::DatumType::Table:
			value = left.Get<Scope&>(right.Get<std::int32_t&>());
			break;
		}

		return value;
	}

	Datum ActionExpression::NotOperation(Datum& right)
	{
		Datum value;

		switch (right.Type())
		{

		case Datum::DatumType::Integer:
			value = ! right.Get<std::int32_t&>();
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::SinOperation(Datum& right)
	{
		Datum value;

		switch (right.Type())
		{

		case Datum::DatumType::Float:
			value = sin(right.Get<float&>());
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::CosOperation(Datum& right)
	{
		Datum value;

		switch (right.Type())
		{

		case Datum::DatumType::Float:
			value = cos(right.Get<float&>());
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::TanOperation(Datum& right)
	{
		Datum value;

		switch (right.Type())
		{

		case Datum::DatumType::Float:
			value = tan(right.Get<float&>());
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::AsinOperation(Datum& right)
	{
		Datum value;

		switch (right.Type())
		{

		case Datum::DatumType::Float:
			value = asin(right.Get<float&>());
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::AcosOperation(Datum& right)
	{
		Datum value;

		switch (right.Type())
		{

		case Datum::DatumType::Float:
			value = acos(right.Get<float&>());
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::AtanOperation(Datum& right)
	{
		Datum value;

		switch (right.Type())
		{

		case Datum::DatumType::Float:
			value = atan(right.Get<float&>());
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::Log2Operation(Datum& right)
	{
		Datum value;

		switch (right.Type())
		{

		case Datum::DatumType::Float:
			value = log2(right.Get<float&>());
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::LogOperation(Datum& right)
	{
		Datum value;

		switch (right.Type())
		{

		case Datum::DatumType::Float:
			value = log(right.Get<float&>());
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::Log10Operation(Datum& right)
	{
		Datum value;

		switch (right.Type())
		{

		case Datum::DatumType::Float:
			value = log10(right.Get<float&>());
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	Datum ActionExpression::SqrtOperation(Datum& right)
	{
		Datum value;

		switch (right.Type())
		{

		case Datum::DatumType::Float:
			value = sqrt(right.Get<float&>());
			break;

		default:
			throw std::exception("Invalid type for operation");

			break;
		}

		return value;
	}

	const HashMap<std::string, std::uint32_t> ActionExpression::sOperatorPrecedenceMap =
	{
		{ "sin", 2U },
		{ "cos", 2U },
		{ "tan", 2U },
		{ "asin", 2U },
		{ "acos", 2U },
		{ "atan", 2U },
		{ "log2", 2U },
		{ "log", 2U },
		{ "log10", 2U },
		{ "sqrt", 2U },
		{ ".", 2U },
		{ "!", 3U },
		{ "*", 5U },
		{ "/", 5U },
		{ "%", 5U },
		{ "+", 6U },
		{ "-", 6U },
		{ "<", 8U },
		{ "<=", 8U },
		{ ">", 8U },
		{ ">=", 8U },
		{ "==", 9U },
		{ "!=", 9U },
		{ "&&", 13U },
		{ "||", 14U },
		{ "(", 16U },
		{ ")", 16U },
		{ "[", 16U },
		{ "]", 16U },
	};

	const HashMap<std::string, ActionExpression::AssociativityType> ActionExpression::sOperatorAssociativitiyMap =
	{
		{ "sin", AssociativityType::Left },
		{ "cos", AssociativityType::Left },
		{ "tan", AssociativityType::Left },
		{ "asin", AssociativityType::Left },
		{ "acos", AssociativityType::Left },
		{ "atan", AssociativityType::Left },
		{ "log2", AssociativityType::Left },
		{ "log", AssociativityType::Left },
		{ "log10", AssociativityType::Left },
		{ "sqrt", AssociativityType::Left },
		{ ".", AssociativityType::Left },
		{ "!", AssociativityType::Right },
		{ "*", AssociativityType::Left },
		{ "/", AssociativityType::Left },
		{ "%", AssociativityType::Left },
		{ "+", AssociativityType::Left },
		{ "-", AssociativityType::Left },
		{ "<", AssociativityType::Left },
		{ "<=", AssociativityType::Left },
		{ ">", AssociativityType::Left },
		{ ">=", AssociativityType::Left },
		{ "==", AssociativityType::Left },
		{ "!=", AssociativityType::Left },
		{ "&&", AssociativityType::Left },
		{ "||", AssociativityType::Left },
		{ "(", AssociativityType::Right },
		{ ")", AssociativityType::Right },
		{ "[", AssociativityType::Right },
		{ "]", AssociativityType::Right },
	};

	const HashMap<std::string, ActionExpression::Operation> ActionExpression::sOperationFunctionMap =
	{
		{ ".", &ActionExpression::DotOperation },
		{ "*", &ActionExpression::MultiplyOperation },
		{ "/", &ActionExpression::DivideOperation },
		{ "%", &ActionExpression::ModulusOperation },
		{ "+", &ActionExpression::AddOperation },
		{ "-", &ActionExpression::SubtractOperation },
		{ "<", &ActionExpression::LessOperation },
		{ "<=", &ActionExpression::LessEqualOperation },
		{ ">", &ActionExpression::GreaterOperation },
		{ ">=", &ActionExpression::GreaterEqualOperation },
		{ "==", &ActionExpression::EqualOperation },
		{ "!=", &ActionExpression::NotEqualOperation },
		{ "&&", &ActionExpression::AndOperation },
		{ "||", &ActionExpression::OrOperation },
		{ "[", &ActionExpression::BracketOperation },
	};

	const HashMap<std::string, ActionExpression::SingleInputOperation> ActionExpression::sSingleInputOperationFunctionMap =
	{
		{ "sin", &ActionExpression::SinOperation },
		{ "cos", &ActionExpression::CosOperation },
		{ "tan", &ActionExpression::TanOperation },
		{ "asin", &ActionExpression::AsinOperation },
		{ "acos", &ActionExpression::AcosOperation },
		{ "atan", &ActionExpression::AtanOperation },
		{ "log2", &ActionExpression::Log2Operation },
		{ "log", &ActionExpression::LogOperation },
		{ "log10", &ActionExpression::Log10Operation },
		{ "sqrt", &ActionExpression::SqrtOperation },
		{ "!", &ActionExpression::NotOperation },
	};

	const std::string ActionExpression::sLeftParen = "(";

	const std::string ActionExpression::sRightParen = ")";

	const std::string ActionExpression::sLeftBracket = "[";

	const std::string ActionExpression::sRightBracket = "]";

	const std::string ActionExpression::sDot = ".";

	const std::string ActionExpression::sNot = "!";

	const char ActionExpression::sTokenDelineator = ' ';

	const char ActionExpression::sLiteralWrapper = '"';

	const std::string ActionExpression::sActionExpressionValueKey = "value";

	const std::string ActionExpression::sExpressionKey = "expression";

#pragma endregion ActionExpression Implementation
}