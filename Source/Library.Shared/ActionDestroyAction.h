#pragma once

#include "Action.h"

namespace FieaGameEngine
{
	/** ActionDestroyAction class
	 *	An Action class that destroys Action instances within its parent.
	 */
	class ActionDestroyAction final : public Action
	{
		
		/** ActionDestroyAction RTTI_DECLARATIONS Macro Call
		 *	Declares virtual RTTI methods that ActionDestroyAction defines.
		 */
		RTTI_DECLARATIONS(ActionDestroyAction, Action)

	public:

		/** ActionDestroyAction Constructor
		 *	Constructs an ActionDestroyAction of the given name.
		 *	@param name a constant string reference to name the ActionDestroyAction with, defaulted to "".
		 */
		explicit ActionDestroyAction(const std::string& name = "");

		/** ActionDestroyAction Destructor
		 *	Destroys the ActionDestroyAction.
		 *	Defaulted.
		 */
		virtual ~ActionDestroyAction() = default;

		/** ActionDestroyAction Copy Constructor
		 *	Constructs an ActionDestroyAction as a copy of the given ActionDestroyAction.
		 *	@param otherActionDestroyAction the constant ActionDestroyAction reference to copy into this ActionDestroyAction.
		 */
		ActionDestroyAction(const ActionDestroyAction& otherActionDestroyAction);

		/** ActionDestroyAction Assignment Operator
		 *	Copies the contents of the given ActionDestroyAction into this ActionDestroyAction.
		 *	@param otherActionDestroyAction the constant ActionDestroyAction reference to copy into this ActionDestroyAction.
		 *	@return an ActionDestroyAction reference to this ActionDestroyAction after assignment.
		 */
		ActionDestroyAction& operator=(const ActionDestroyAction& otherActionDestroyAction);

		/** ActionDestroyAction Move Constructor
		 *	Constructs a copy of the given ActionDestroyAction, nullifying the given ActionDestroyAction.
		 *	@param previousActionDestroyAction the ActionDestroyAction double reference to copy into this ActionDestroyAction.
		 */
		ActionDestroyAction(ActionDestroyAction&& otherActionDestroyAction);

		/** ActionDestroyAction Move Assignment Operator
		 *	Copies the contents of the given ActionDestroyAction into this ActionDestroyAction, nullifying the given ActionDestroyAction.
		 *	@param previousActionDestroyAction the ActionDestroyAction double reference to copy into this ActionDestroyAction.
		 *	@return an ActionDestroyAction reference to this ActionDestroyAction after assignment.
		 */
		ActionDestroyAction& operator=(ActionDestroyAction&& otherActionDestroyAction);

		/** ActionDestroyAction SetInstanceName
		*	Sets the name of the Action to destroy.
		*	@param instanceName a constant string reference to set this Entity instance's name to.
		*/
		void SetInstanceName(const std::string& instanceName);

		/** ActionDestroyAction Update
		 *	Updates the components of this ActionDestroyAction and notifies the worldState that this is the current ActionDestroyAction.
		 *	@param worldState a WorldState reference containing the data of the world at this moment in time.
		 */
		virtual void Update(class WorldState& worldState) override;

		/** ActionDestroyAction Copy
		 *	Creates a copy of this ActionDestroyAction as a Scope pointer.
		 *	@return a Scope pointer constructed as an ActionDestroyAction.
		 */
		virtual Scope* Copy() const override;

	protected:

		/** ActionDestroyAction InitializeSignatures
		 *	Initializes the prescribed attributes for instances of this class.
		 */
		void InitializeSignatures();

	private:

		/** ActionDestroyAction CopyPrivateDataMembers
		 *	Copies the data members of the given ActionDestroyAction that are not within the Scope hash.
		 *	@param otherEntity a constant ActionDestroyAction reference to copy data members from.
		 */
		void CopyPrivateDataMembers(const ActionDestroyAction& otherActionDestroyAction);

		/** ActionDestroyAction FixExternalAttributes
		 *	Fixes the external attribute pointers within this ActionDestroyAction after being copied to.
		 */
		void FixExternalAttributes();

		/** ActionDestroyAction mName
		 *	A string representing the name of the Action to destroy.
		 */
		std::string mInstanceName;

	public:

		/** ActionDestroyAction sActionDestroyActionInstanceNameKey
		 *	A string representing the key instance names are held at within an ActionDestroyAction.
		 */
		static const std::string sActionDestroyActionInstanceNameKey;
	};

	/** ActionDestroyAction of Entity Macro Call
	 *	Defines the ActionDestroyActionFactory class that returns Action pointers from its create call.
	 */
	ConcreteActionFactory(ActionDestroyAction);
}