#pragma once

#include "Action.h"

namespace FieaGameEngine
{
	/** ActionCreateAction class
	 *	An Action class that creates Action objects using the factory pattern.
	 */
	class ActionCreateAction final : public Action
	{

		/** ActionCreateAction RTTI_DECLARATIONS Macro Call
		 *	Declares virtual RTTI methods that ActionCreateAction defines.
		 */
		RTTI_DECLARATIONS(ActionCreateAction, Action)

	public:

		/** ActionCreateAction Constructor
		 *	Constructs an ActionCreateAction of the given name.
		 *	@param name a constant string reference to name the ActionCreateAction with, defaulted to "".
		 */
		explicit ActionCreateAction(const std::string& name = "");

		/** ActionCreateAction Destructor
		 *	Destroys the ActionCreateAction.
		 *	Defaulted.
		 */
		virtual ~ActionCreateAction() = default;

		/** ActionCreateAction Copy Constructor
		 *	Constructs an ActionCreateAction as a copy of the given ActionCreateAction.
		 *	@param otherActionCreateAction the constant ActionCreateAction reference to copy into this ActionCreateAction.
		 */
		ActionCreateAction(const ActionCreateAction& otherActionCreateAction);

		/** ActionCreateAction Assignment Operator
		 *	Copies the contents of the given ActionCreateAction into this ActionCreateAction.
		 *	@param otherEntity the constant ActionCreateAction reference to copy into this ActionCreateAction.
		 *	@return an ActionCreateAction reference to this ActionCreateAction after assignment.
		 */
		ActionCreateAction& operator=(const ActionCreateAction& otherActionCreateAction);

		/** ActionCreateAction Move Constructor
		 *	Constructs a copy of the given ActionCreateAction, nullifying the given ActionCreateAction.
		 *	@param previousActionCreateAction the ActionCreateAction double reference to copy into this ActionCreateAction.
		 */
		ActionCreateAction(ActionCreateAction&& previousActionCreateAction);

		/** ActionCreateAction Move Assignment Operator
		*	Copies the contents of the given ActionCreateAction into this ActionCreateAction, nullifying the given ActionCreateAction.
		*	@param previousActionCreateAction the ActionCreateAction double reference to copy into this ActionCreateAction.
		*	@return an ActionCreateAction reference to this ActionCreateAction after assignment.
		*/
		ActionCreateAction& operator=(ActionCreateAction&& previousActionCreateAction);

		/** ActionCreateAction SetInstanceName
		 *	Sets the instance name for the Action to be created.
		 *	@param instanceName a constant string reference to set the instance name to.
		 */
		void SetInstanceName(const std::string& instanceName);

		/** ActionCreateAction SetClassName
		 *	Sets the class name for the Action to be created.
		 *	@param className a constant string reference to set the class name to.
		 */
		void SetClassName(const std::string& className);

		/** ActionCreateAction Update
		 *	Updates the components of this ActionCreateAction and notifies the worldState that this is the current Action.
		 *	@param worldState a WorldState reference containing the data of the world at this moment in time.
		 */
		virtual void Update(class WorldState& worldState) override;

		/** ActionCreateAction Copy
		 *	Creates a copy of this ActionCreateAction as a Scope pointer.
		 *	@return a Scope pointer constructed as an ActionCreateAction.
		 */
		virtual Scope* Copy() const override;

	protected:

		/** ActionCreateAction InitializeSignatures
		 *	Initializes the prescribed attributes for instances of this class.
		 */
		void InitializeSignatures();

	private:

		/** ActionCreateAction CopyPrivateDataMembers
		 *	Copies the data members of the given ActionCreateAction that are not within the Scope hash.
		 *	@param otherActionCreateAction a constant ActionCreateAction reference to copy data members from.
		 */
		void CopyPrivateDataMembers(const ActionCreateAction& otherActionCreateAction);

		/** ActionCreateAction FixExternalAttributes
		 *	Fixes the external attribute pointers within this ActionCreateAction after being copied to.
		 */
		void FixExternalAttributes();

		/** ActionCreateAction mInstanceName
		 *	A string representing the instance name of this ActionCreateAction.
		 */
		std::string mInstanceName;

		/** ActionCreateAction mClassName
		 *	A string representing the class name of this ActionCreateAction.
		 */
		std::string mClassName;

	public:

		/** ActionCreateAction sActionCreateActionInstanceNameKey
		 *	A string representing the key the instance name prescribed attribute resides in.
		 */
		static const std::string sActionCreateActionInstanceNameKey;

		/** ActionCreateAction sActionCreateActionClassNameKey
		 *	A string representing the key the class name prescribed attribute resides in.
		 */
		static const std::string sActionCreateActionClassNameKey;
	};

	/** ConcreteActionFactory of ActionCreateAction Macro Call
	 *	Defines the ActionCreateActionFactory class that returns Action pointers from its create call.
	 */
	ConcreteActionFactory(ActionCreateAction);
}