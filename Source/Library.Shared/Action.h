#pragma once

#include <string>
#include "Attributed.h"

namespace FieaGameEngine
{
	/** Action class
	 *	An Attributed class that represents an Action in the game world.
	 */
	class Action : public Attributed
	{

		/** Action RTTI_DECLARATIONS Macro Call
		 *	Declares virtual RTTI methods that Action defines.
		 */
		RTTI_DECLARATIONS(Action, Attributed)

	public:

		/** Action Constructor
		 *	Constructs an Action of the given name.
		 *	@param name a constant string reference to name the Action with, defaulted to "".
		 */
		explicit Action(const std::string& name = "");

		/** Action Destructor
		 *	Destroys the Action.
		 *	Defaulted.
		 */
		virtual ~Action() = default;

		/** Action Copy Constructor
		 *	Constructs an Action as a copy of the given Action.
		 *	@param otherAction the constant Action reference to copy into this Action.
		 */
		Action(const Action& otherAction);

		/** Action Assignment Operator
		 *	Copies the contents of the given Action into this Action.
		 *	@param otherAction the constant Action reference to copy into this Action.
		 *	@return an Action reference to this Action after assignment.
		 */
		Action& operator=(const Action& otherAction);

		/** Action Move Constructor
		 *	Constructs a copy of the given Action, nullifying the given Action.
		 *	@param previousAction the Action double reference to copy into this Action.
		 */
		Action(Action&& previousAction);

		/** Action Move Assignment Operator
		 *	Copies the contents of the given Action into this Action, nullifying the given Action.
		 *	@param previousAction the Action double reference to copy into this Action.
		 *	@return an Action reference to this Action after assignment.
		 */
		Action& operator=(Action&& previousAction);

		/** Entity Name
		*	Returns the name of this Entity.
		*	@return a constant string reference representing the name of this Entity.
		*/
		const std::string& Name() const;

		/** Action SetName
		 *	Sets the name of this Action to the given string.
		 *	@param name a constant string reference to set this Action instance's name to.
		 */
		void SetName(const std::string& name);

		/** Action Reactions
		 *	Returns the Datum within this Action that contains the Reaction instances owned by this Action.
		 *	@return a Datum reference containing the Reaction instances.
		 */
		Datum& Reactions();

		/** Action Reactions
		 *	Returns the Datum within this Action that contains the Reaction instances owned by this Action.
		 *	@return a constant Datum reference containing the Reaction instances.
		 */
		const Datum& Reactions() const;

		/** Action Update
		 *	Updates the components of this Action and notifies the worldState that this is the current Action.
		 *	@param worldState a WorldState reference containing the data of the world at this moment in time.
		 */
		virtual void Update(class WorldState& worldState) = 0;

	protected:

		/** Action InitializeSignatures
		 *	Initializes the prescribed attributes for instances of this class.
		 */
		void InitializeSignatures();

	private:

		/** Action CopyPrivateDataMembers
		 *	Copies the data members of the given Action that are not within the Scope hash.
		 *	@param otherAction a constant Action reference to copy data members from.
		 */
		void CopyPrivateDataMembers(const Action& otherAction);

		/** Action FixExternalAttributes
		 *	Fixes the external attribute pointers within this Action after being copied to.
		 */
		void FixExternalAttributes();

		/** Action mName
		 *	A string representing the name of this Action.
		 */
		std::string mName;

	public:

		/** Action sActionNameKey
		 *	A string representing the key names are held at within an Action.
		 */
		static const std::string sActionNameKey;

		/** Action sActionsKey
		 *	A string representing the key Action are held at within Attributed classes that contain Action instances.
		 */
		static const std::string sActionsKey;
	};

	/** ConcreteActionFactory Macro Definition
	 *	Defines the macro that creates the classes for Action Factories.
	 */
#define ConcreteActionFactory(ConcreteProductT) ConcreteFactory(Action, ConcreteProductT)
}