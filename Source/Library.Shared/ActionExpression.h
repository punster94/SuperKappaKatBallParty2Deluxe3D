#pragma once

#include "Action.h"
#include "Datum.h"

namespace FieaGameEngine
{
	/** ActionExpression class
	 *	An Action class that evaluates an expression stored in a string.
	 */
	class ActionExpression final : public Action
	{
		/** ActionExpression RTTI_DECLARATIONS Macro Call
		 *	Declares virtual RTTI methods that ActionExpression defines.
		 */
		RTTI_DECLARATIONS(ActionExpression, Action)

	public:

		/** ActionExpression Constructor
		 *	Constructs an ActionExpression of the given name.
		 *	@param name a constant string reference to name the ActionExpression with, defaulted to "".
		 */
		explicit ActionExpression(const std::string& name = "");

		/** ActionExpression Destructor
		 *	Destroys the ActionExpression.
		 *	Defaulted.
		 */
		virtual ~ActionExpression() = default;

		/** ActionExpression Copy Constructor
		 *	Constructs an ActionExpression as a copy of the given ActionExpression.
		 *	@param otherActionExpression the constant ActionExpression reference to copy into this ActionExpression.
		 */
		ActionExpression(const ActionExpression& otherActionExpression);

		/** ActionExpression Assignment Operator
		 *	Copies the contents of the given ActionExpression into this ActionExpression.
		 *	@param otherActionExpression the constant ActionExpression reference to copy into this ActionExpression.
		 *	@return an ActionExpression reference to this ActionExpression after assignment.
		 */
		ActionExpression& operator=(const ActionExpression& otherActionExpression);

		/** ActionExpression Move Constructor
		 *	Constructs a copy of the given ActionExpression, nullifying the given ActionExpression.
		 *	@param previousActionExpression the ActionExpression double reference to copy into this ActionExpression.
		 */
		ActionExpression(ActionExpression&& previousActionExpression);

		/** ActionExpression Move Assignment Operator
		 *	Copies the contents of the given ActionExpression into this ActionExpression, nullifying the given ActionExpression.
		 *	@param previousActionExpression the ActionExpression double reference to copy into this ActionExpression.
		 *	@return an ActionExpression reference to this ActionExpression after assignment.
		 */
		ActionExpression& operator=(ActionExpression&& previonsActionExpression);

		/** ActionExpression SetExpression
		 *	Sets the string expression by computing the RPN version of the passed string.
		 *	@param expression a constant string reference to calculate the RPN of.
		 */
		void SetExpression(const std::string& expression);

		/** ActionExpression GetRPN
		 *	Returns the RPN of the last expression string passed in.
		 *	@return a constant string reference 
		 */
		const std::string& GetRPN() const;

		/** ActionExpression Evaluate
		 *	Evaluates the current RPN string stored in this ActionExpression.
		 *	@return a constant Datum reference containing the value of the expression.
		 */
		const Datum& Evaluate();

		/** ActionExpression Update
		 *	Overrides the base class Update method to allow instantiation.
		 *	@param worldState a WorldState reference to the current state of the world.
		 */
		virtual void Update(class WorldState& worldState) override;

		/** ActionExpression Copy
		 *	Creates a copy of this ActionExpression as a Scope pointer.
		 *	@return a Scope pointer constructed as an ActionExpression.
		 */
		virtual class Scope* Copy() const override;

	private:

		/** ActionExpression InitializeSignatures
		 *	Initializes the prescribed attributes for instances of this class.
		 */
		void InitializeSignatures();

		/** ActionExpression CopyPrivateDataMembers
		 *	Copies the data members of the given ActionExpression that are not within the Scope hash.
		 *	@param otherActionExpression a constant ActionExpression reference to copy data members from.
		 */
		void CopyPrivateDataMembers(const ActionExpression& otherActionExpression);

		/** ActionExpression FixExternalAttributes
		 *	Fixes the external attribute pointers within this ActionExpression after being copied to.
		 */
		void FixExternalAttributes();

		/** ActionExpression ComputeRPN
		 *	Computes the RPN version of a given string, setting a passed reference to the result.
		 *	@param expression a constant string representing the expression to compute.
		 *	@param destination a string reference to store the resulting RPN in.
		 */
		void ComputeRPN(const std::string& expression, std::string& destination);

		/** ActionExpression ProcessAsSymbol
		 *	Processes a given string as a symbol of the scripting language.
		 *	@param buffer a string reference representing the symbol.
		 *	@param operatorStack a reference to a Vector of strings that contains operations during evaluation.
		 */
		void ProcessAsSymbol(std::string& buffer, Vector<std::string>& operatorStack, std::string& destination);

		/** ActionExpression PopIntoDestination
		 *	Captures the top operation on the passed operator stack and adds it to the destination string.
		 *	@param operatorStack a reference to a Vector of strings that contains operation during evaluation.
		 *	@param destination a reference to a string that contains the built up result of evaluation.
		 */
		void PopIntoDestination(Vector<std::string>& operatorStack, std::string& destination);

		/** ActionExpression EvaluateSymbol
		 *	Determines how to interpret the given string, adding its value to the value stack.
		 *	@param buffer a string reference containing the symbol.
		 *	@param valueStack a reference to a Vector of Datum instances to push the resulting value to.
		 */
		void EvaluateSymbol(std::string& buffer, Vector<Datum>& valueStack);

		/** ActionExpression EvaluateAsLiteralSymbol
		 *	Attempts to evaluate the given string as a literal value, returning false if unable.
		 *	@param buffer a string reference containing the symbol.
		 *	@param value a Datum reference to store the result in.
		 *	@return a boolean representing whether the symbol was evaluated as a literal value.
		 */
		bool EvaluateAsLiteralSymbol(std::string& buffer, Datum& value);

		/** ActionExpression DotOperation
		 *	Evaluates the dot operation given the two datums to operate on.
		 *	@exception will fail if the left side is not a Scope and the right is not a member of that Scope.
		 *	@param left a Datum reference representing the left side of the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum DotOperation(Datum& left, Datum& right);

		/** ActionExpression MultiplyOperation
		 *	Evaluates the multiply operation given the two datums to operate on.
		 *	@exception will fail if the left Datum's DatumType does not respond to the operation.
		 *	@param left a Datum reference representing the left side of the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum MultiplyOperation(Datum& left, Datum& right);

		/** ActionExpression DivideOperation
		 *	Evaluates the division operation given the two datums to operate on.
		 *	@exception will fail if the left Datum's DatumType does not respond to the operation.
		 *	@param left a Datum reference representing the left side of the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum DivideOperation(Datum& left, Datum& right);

		/** ActionExpression ModulusOperation
		 *	Evaluates the modulus operation given the two datums to operate on.
		 *	@exception will fail if the left Datum's DatumType does not respond to the operation.
		 *	@param left a Datum reference representing the left side of the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum ModulusOperation(Datum& left, Datum& right);

		/** ActionExpression AddOperation
		 *	Evaluates the addition operation given the two datums to operate on.
		 *	@exception will fail if the left Datum's DatumType does not respond to the operation.
		 *	@param left a Datum reference representing the left side of the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum AddOperation(Datum& left, Datum& right);

		/** ActionExpression SubtractOperation
		 *	Evaluates the subtraction operation given the two datums to operate on.
		 *	@exception will fail if the left Datum's DatumType does not respond to the operation.
		 *	@param left a Datum reference representing the left side of the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum SubtractOperation(Datum& left, Datum& right);

		/** ActionExpression LessOperation
		 *	Evaluates the less than operation given the two datums to operate on.
		 *	@exception will fail if the left Datum's DatumType does not respond to the operation.
		 *	@param left a Datum reference representing the left side of the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum LessOperation(Datum& left, Datum& right);

		/** ActionExpression LessEqualOperation
		 *	Evaluates the less than or equal operation given the two datums to operate on.
		 *	@exception will fail if the left Datum's DatumType does not respond to the operation.
		 *	@param left a Datum reference representing the left side of the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum LessEqualOperation(Datum& left, Datum& right);

		/** ActionExpression GreaterOperation
		 *	Evaluates the greater than operation given the two datums to operate on.
		 *	@exception will fail if the left Datum's DatumType does not respond to the operation.
		 *	@param left a Datum reference representing the left side of the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum GreaterOperation(Datum& left, Datum& right);

		/** ActionExpression GreaterEqualOperation
		 *	Evaluates the greater than or equal operation given the two datums to operate on.
		 *	@exception will fail if the left Datum's DatumType does not respond to the operation.
		 *	@param left a Datum reference representing the left side of the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum GreaterEqualOperation(Datum& left, Datum& right);

		/** ActionExpression EqualOperation
		 *	Evaluates the equality operation given the two datums to operate on.
		 *	@exception will fail if the left Datum's DatumType does not respond to the operation.
		 *	@param left a Datum reference representing the left side of the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum EqualOperation(Datum& left, Datum& right);

		/** ActionExpression NotEqualOperation
		 *	Evaluates the inequality operation given the two datums to operate on.
		 *	@exception will fail if the left Datum's DatumType does not respond to the operation.
		 *	@param left a Datum reference representing the left side of the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum NotEqualOperation(Datum& left, Datum& right);

		/** ActionExpression AndOperation
		 *	Evaluates the logical and operation given the two datums to operate on.
		 *	@exception will fail if the left Datum's DatumType does not respond to the operation.
		 *	@param left a Datum reference representing the left side of the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum AndOperation(Datum& left, Datum& right);

		/** ActionExpression OrOperation
		 *	Evaluates the logical or operation given the two datums to operate on.
		 *	@exception will fail if the left Datum's DatumType does not respond to the operation.
		 *	@param left a Datum reference representing the left side of the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum OrOperation(Datum& left, Datum& right);

		/** ActionExpression BracketOperation
		 *	Evaluates the accessor operation given the two datums to operate on.
		 *	@exception will fail if the left Datum's DatumType does not respond to the operation.
		 *	@param left a Datum reference representing the left side of the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum BracketOperation(Datum& left, Datum& right);

		/** ActionExpression NotOperation
		 *	Evaluates the toggle operation given the datum to operate on.
		 *	@exception will fail if the given Datum's DatumType does not respond to the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum NotOperation(Datum& right);

		/** ActionExpression SinOperation
		 *	Evaluates the sine operation given the datum to operate on.
		 *	@exception will fail if the given Datum's DatumType does not respond to the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum SinOperation(Datum& right);

		/** ActionExpression CosOperation
		 *	Evaluates the cosine operation given the datum to operate on.
		 *	@exception will fail if the given Datum's DatumType does not respond to the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum CosOperation(Datum& right);

		/** ActionExpression TanOperation
		 *	Evaluates the tangent operation given the datum to operate on.
		 *	@exception will fail if the given Datum's DatumType does not respond to the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum TanOperation(Datum& right);

		/** ActionExpression AsinOperation
		 *	Evaluates the arcsine operation given the datum to operate on.
		 *	@exception will fail if the given Datum's DatumType does not respond to the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum AsinOperation(Datum& right);

		/** ActionExpression AcosOperation
		 *	Evaluates the arccosine operation given the datum to operate on.
		 *	@exception will fail if the given Datum's DatumType does not respond to the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum AcosOperation(Datum& right);

		/** ActionExpression AtanOperation
		 *	Evaluates the arctangent operation given the datum to operate on.
		 *	@exception will fail if the given Datum's DatumType does not respond to the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum AtanOperation(Datum& right);

		/** ActionExpression Log2Operation
		 *	Evaluates the log base 2 operation given the datum to operate on.
		 *	@exception will fail if the given Datum's DatumType does not respond to the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum Log2Operation(Datum& right);

		/** ActionExpression LogOperation
		 *	Evaluates the natural log operation given the datum to operate on.
		 *	@exception will fail if the given Datum's DatumType does not respond to the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum LogOperation(Datum& right);

		/** ActionExpression Log10Operation
		 *	Evaluates the log base 10 operation given the datum to operate on.
		 *	@exception will fail if the given Datum's DatumType does not respond to the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum Log10Operation(Datum& right);

		/** ActionExpression SqrtOperation
		 *	Evaluates the square root operation given the datum to operate on.
		 *	@exception will fail if the given Datum's DatumType does not respond to the operation.
		 *	@param right a Datum reference representing the right side of the operation.
		 *	@return a Datum representing the result of the operation.
		 */
		Datum SqrtOperation(Datum& right);

		/** ActionExpression mExpression
		 *	A string representing the RPN version of the current expression.
		 */
		std::string mExpression;

		/** ActionExpression mValue
		 *	A Datum storing the result of the expression's evaluation.
		 */
		Datum mValue;

		/** ActionExpression AssociativityType
		 *	An enumeration representing whether an operation is left or right associative.
		 */
		enum class AssociativityType
		{
			Left,
			Right
		};

		/** ActionExpression Operation typedef
		 *	Defines the signature of Operation functions.
		 */
		typedef Datum(ActionExpression::*Operation)(Datum& left, Datum& right);

		/** ActionExpression sOperationFunctionMap
		 *	A HashMap of string to Operation functions that maps symbols to operations with two inputs.
		 */
		static const HashMap<std::string, Operation> sOperationFunctionMap;

		/** ActionExpression SingleInputOperation typedef
		 *	Defines the signature of SingleInputOperation functions.
		 */
		typedef Datum(ActionExpression::*SingleInputOperation)(Datum& right);

		/** ActionExpression sSingleInputOperationFunctionMap
		 *	A HashMap of string to Operation functions that maps symbols to operations with one input.
		 */
		static const HashMap<std::string, SingleInputOperation> sSingleInputOperationFunctionMap;

		/** ActionExpression sOperatorPrecedenceMap
		 *	A HashMap of string to unsigned integer values that maps symbols to precedence values.
		 */
		static const HashMap<std::string, std::uint32_t> sOperatorPrecedenceMap;

		/** ActionExpression sOperatorAssociativityMap
		 *	A HashMap of string to AssociativityType values that maps symbols to their AssociativityType.
		 */
		static const HashMap<std::string, AssociativityType> sOperatorAssociativitiyMap;

		/** ActionExpression sLeftParen
		 *	A constant string that holds the value "(".
		 */
		static const std::string sLeftParen;

		/** ActionExpression sRightParen
		 *	A constant string that holds the value ")".
		 */
		static const std::string sRightParen;

		/** ActionExpression sLeftBracket
		 *	A constant string that holds the value "[".
		 */
		static const std::string sLeftBracket;

		/** ActionExpression sRightBracket
		 *	A constant string that holds the value "]".
		 */
		static const std::string sRightBracket;

		/** ActionExpression sDot
		 *	A constant string that holds the value ".".
		 */
		static const std::string sDot;

		/** ActionExpression sNot
		 *	A constant string that holds the value "!".
		 */
		static const std::string sNot;

		/** ActionExpression sTokenDelineator
		 *	A constant character that holds the value ' '.
		 */
		static const char sTokenDelineator;

		/** ActionExpression sLiteralWrapper
		 *	A constant character that holds the value '"'.
		 */
		static const char sLiteralWrapper;

	public:

		/** ActionExpression sActionExpressionValueKey
		 *	A constant string that holds the key expression values are held in.
		 */
		static const std::string sActionExpressionValueKey;

		/** ActionExpression sExpressionKey
		 *	A constant string that holds the key expressions are held in.
		 */
		static const std::string sExpressionKey;
	};

	/** ActionExpression Factory of Action Macro Call
	 *	Defines the ActionExpressionFactory class that returns Action pointers from its create call.
	 */
	ConcreteActionFactory(ActionExpression);
}