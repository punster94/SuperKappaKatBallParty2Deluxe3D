#pragma once

#include <string>
#include "Action.h"

namespace FieaGameEngine
{
	/** ActionList class
	 *	An Action class that contains a list of Actions.
	 */
	class ActionList : public Action
	{

		/** ActionList RTTI_DECLARATIONS Macro Call
		 *	Declares virtual RTTI methods that ActionList defines.
		 */
		RTTI_DECLARATIONS(ActionList, Action)

	public:

		/** ActionList Constructor
		 *	Constructs an ActionList of the given name.
		 *	@param name a constant string reference to name the ActionList with, defaulted to "".
		 */
		explicit ActionList(const std::string& name = "");

		/** ActionList Destructor
		 *	Destroys the ActionList.
		 *	Defaulted.
		 */
		virtual ~ActionList() = default;

		/** ActionList Copy Constructor
		 *	Constructs an ActionList as a copy of the given ActionList.
		 *	@param otherActionList the constant ActionList reference to copy into this ActionList.
		 */
		ActionList(const ActionList& otherActionList);

		/** ActionList Assignment Operator
		 *	Copies the contents of the given ActionList into this ActionList.
		 *	@param otherActionList the constant ActionList reference to copy into this ActionList.
		 *	@return an ActionList reference to this ActionList after assignment.
		 */
		ActionList& operator=(const ActionList& otherActionList);

		/** ActionList Move Constructor
		 *	Constructs a copy of the given ActionList, nullifying the given ActionList.
		 *	@param previousActionList the ActionList double reference to copy into this ActionList.
		 */
		ActionList(ActionList&& previousActionList);

		/** ActionList Move Assignment Operator
		 *	Copies the contents of the given ActionList into this ActionList, nullifying the given ActionList.
		 *	@param previousActionList the ActionList double reference to copy into this ActionList.
		 *	@return an ActionList reference to this ActionList after assignment.
		 */
		ActionList& operator=(ActionList&& previousActionList);

		/** ActionList Actions
		 *	Returns the Datum within this ActionList that contains the Action instances owned by this ActionList.
		 *	@return a Datum reference containing the Action instances.
		 */
		Datum& Actions();

		/** ActionList Actions
		 *	Returns the Datum within this ActionList that contains the Action instances owned by this ActionList.
		 *	@return a constant Datum reference containing the Action instances.
		 */
		const Datum& Actions() const;
		
		/** ActionList CreateAction
		 *	Creates an Action instance given the c++ class name and the instance name.
		 *	@param className a constant string reference representing the name of the c++ class to create.
		 *	@param instanceName a constant string reference representing the name of the Action.
		 *	@return a pointer to the created Action.
		 */
		Action* CreateAction(const std::string& className, const std::string& instanceName);

		/** ActionList CreateAction
		 *	Finds the Action within this ActionList with the given name.
		 *	@param name a constant string reference representing the name of the Action to find.
		 *	@return a pointer to the found Action, or nullptr.
		 */
		Action* FindAction(const std::string& name) const;

		/** ActionList Update
		 *	Updates the components of this ActionList and notifies the worldState that this is the current Action.
		 *	@param worldState a WorldState reference containing the data of the world at this moment in time.
		 */
		virtual void Update(class WorldState& worldState) override;

		/** ActionList Copy
		 *	Creates a copy of this ActionList as a Scope pointer.
		 *	@return a Scope pointer constructed as an ActionList.
		 */
		virtual Scope* Copy() const override;

	protected:

		/** ActionList InitializeSignatures
		 *	Initializes the prescribed attributes for instances of this class.
		 */
		void InitializeSignatures();

	};

	/** ConcreteActionFactory of ActionList Macro Call
	 *	Defines the ActionListFactory class that returns Action pointers from its create call.
	 */
	ConcreteActionFactory(ActionList);
}