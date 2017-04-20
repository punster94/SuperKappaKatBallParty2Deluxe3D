#pragma once

#include "ActionList.h"

namespace FieaGameEngine
{
	/** ActionListIf class
	 *	An Action class that executes actions based on a condition.
	 */
	class ActionListIf final : public ActionList
	{

		/** ActionListIf RTTI_DECLARATIONS Macro Call
		 *	Declares virtual RTTI methods that ActionListIf defines.
		 */
		RTTI_DECLARATIONS(ActionListIf, ActionList)

	public:

		/** ActionListIf Constructor
		 *	Constructs an ActionListIf of the given name.
		 *	@param name a constant string reference to name the ActionListIf with, defaulted to "".
		 */
		explicit ActionListIf(const std::string& name = "");

		/** ActionListIf Destructor
		 *	Destroys the ActionListIf.
		 *	Defaulted.
		 */
		virtual ~ActionListIf() = default;

		/** ActionListIf Copy Constructor
		 *	Constructs an ActionListIf as a copy of the given ActionListIf.
		 *	@param otherActionListIf the constant ActionListIf reference to copy into this ActionListIf.
		 */
		ActionListIf(const ActionListIf& otherActionListIf);

		/** ActionListIf Assignment Operator
		 *	Copies the contents of the given ActionListIf into this ActionListIf.
		 *	@param otherActionListIf the constant ActionListIf reference to copy into this ActionListIf.
		 *	@return an ActionListIf reference to this ActionListIf after assignment.
		 */
		ActionListIf& operator=(const ActionListIf& otherActionListIf);

		/** ActionListIf Move Constructor
		 *	Constructs a copy of the given ActionListIf, nullifying the given ActionListIf.
		 *	@param previousActionListIf the ActionListIf double reference to copy into this ActionListIf.
		 */
		ActionListIf(ActionListIf&& previousActionListIf);

		/** ActionListIf Move Assignment Operator
		 *	Copies the contents of the given ActionListIf into this ActionListIf, nullifying the given ActionListIf.
		 *	@param previousActionListIf the ActionListIf double reference to copy into this ActionListIf.
		 *	@return an ActionListIf reference to this ActionListIf after assignment.
		 */
		ActionListIf& operator=(ActionListIf&& previousActionListIf);

		/** ActionListIf ThenAction
		 *	Gets the then block from this if.
		 *	@return an Action pointer representing the then block, or nullptr.
		 */
		Action* ThenAction() const;

		/** ActionListIf ElseAction
		 *	Gets the else block from this if.
		 *	@return an Action pointer representing the else block, or nullptr.
		 */
		Action* ElseAction() const;

		/** ActionListIf Update
		 *	Updates the components of this ActionListIf and notifies the worldState that this is the current Action.
		 *	@param worldState a WorldState reference containing the data of the world at this moment in time.
		 */
		virtual void Update(class WorldState& worldState) override;

		/** ActionListIf Copy
		 *	Creates a copy of this ActionListIf as a Scope pointer.
		 *	@return a Scope pointer constructed as an ActionListIf.
		 */
		virtual Scope* Copy() const override;

	protected:

		/** ActionListIf InitializeSignatures
		 *	Initializes the prescribed attributes for instances of this class.
		 */
		void InitializeSignatures();

	private:

		/** ActionListIf CopyPrivateDataMembers
		 *	Copies the data members of the given ActionListIf that are not within the Scope hash.
		 *	@param otherActionListIf a constant ActionListIf reference to copy data members from.
		 */
		void CopyPrivateDataMembers(const ActionListIf& otherActionListIf);

		/** ActionListIf FixExternalAttributes
		 *	Fixes the external attribute pointers within this ActionListIf after being copied to.
		 */
		void FixExternalAttributes();

	public:

		/** ActionListIf sActionListIfThenKey
		 *	A string representing the key then Actions have within an ActionListIf.
		 */
		static const std::string sActionListIfThenKey;

		/** ActionListIf sActionListIfElseKey
		 *	A string representing the name else Actions have within an ActionListIf.
		 */
		static const std::string sActionListIfElseKey;
	};

	/** ConcreteActionFactory of ActionListIf Macro Call
	 *	Defines the ActionListIfFactory class that returns Action pointers from its create call.
	 */
	ConcreteActionFactory(ActionListIf);
}