#pragma once

#include "Action.h"

namespace FieaGameEngine
{
	/** Forward Declaration of WorldState
	 *	Declares the WorldState class.
	 */
	class WorldState;

	/** ActionEvent class
	 *	An Action class that fires an event.
	 */
	class ActionEvent : public Action
	{

		/** ActionEvent RTTI_DECLARATIONS Macro Call
		 *	Declares virtual RTTI methods that ActionEvent defines.
		 */
		RTTI_DECLARATIONS(ActionEvent, Action)

	public:

		/** ActionEvent Constructor
		 *	Constructs a ActionEvent of the given name.
		 *	@param name a constant string reference representing the name of the action.
		 */
		ActionEvent(const std::string& name = "");

		/** ActionEvent Destructor
		 *	Destroys the ActionEvent.
		 *	Defaulted.
		 */
		virtual ~ActionEvent() = default;

		/** ActionEvent Copy Constructor
		 *	Constructs a ActionEvent as a copy of the given ActionEvent.
		 *	@param otherActionEvent the constant ActionEvent reference to copy into this ActionEvent.
		 */
		ActionEvent(const ActionEvent& otherActionEvent);

		/** ActionEvent Assignment Operator
		 *	Copies the contents of the given ActionEvent into this ActionEvent.
		 *	@param otherActionEvent the constant ActionEvent reference to copy into this ActionEvent.
		 *	@return a ActionEvent reference to this ActionEvent after assignment.
		 */
		ActionEvent& operator=(const ActionEvent& otherActionEvent);

		/** ActionEvent Move Constructor
		 *	Constructs a copy of the given ActionEvent, nullifying the given ActionEvent.
		 *	@param previousActionEvent the ActionEvent double reference to copy into this ActionEvent.
		 */
		ActionEvent(ActionEvent&& previousActionEvent);

		/** ActionEvent Move Assignment Operator
		 *	Copies the contents of the given ActionEvent into this ActionEvent, nullifying the given ActionEvent.
		 *	@param previousActionEvent the ActionEvent double reference to copy into this ActionEvent.
		 *	@return a ActionEvent reference to this ActionEvent after assignment.
		 */
		ActionEvent& operator=(ActionEvent&& previousActionEvent);

		/** ActionEvent SetSubtype
		 *	Sets the subtype of this ActionEvent to the given string.
		 *	@param subtype a constant string reference to set this ActionEvent instance's subtype to.
		 */
		void SetSubtype(const std::string& subtype);

		/** ActionEvent GetSubtype
		 *	Returns the subtype of this ActionEvent.
		 *	@return a constant string reference representing the subtype of this ActionEvent.
		 */
		const std::string& GetSubtype() const;

		/** ActionEvent SetDelay
		 *	Sets the delay of this ActionEvent to the given integer.
		 *	@param delay a constant integer to set this ActionEvent instance's delay to.
		 */
		void SetDelay(const std::int32_t delay);

		/** ActionEvent GetDelay
		 *	Returns the delay of this ActionEvent.
		 *	@return an integer representing the delay of this ActionEvent.
		 */
		std::int32_t GetDelay() const;

		/** ActionEvent Update
		*
		*/
		virtual void Update(WorldState& worldState);

		/** ActionEvent Copy
		 *	Creates a copy of this ActionEvent as a Scope pointer.
		 *	@return a Scope pointer constructed as an ActionEvent.
		 */
		virtual Scope* Copy() const override;

	protected:

		/** ActionEvent InitializeSignatures
		 *	Initializes the prescribed attributes for instances of this class.
		 */
		void InitializeSignatures();

	private:

		/** ActionEvent CopyPrivateDataMembers
		 *	Copies the data members of the given ActionEvent that are not within the Scope hash.
		 *	@param otherActionEvent a constant ActionEvent reference to copy data members from.
		 */
		void CopyPrivateDataMembers(const ActionEvent& otherActionEvent);

		/** ActionEvent FixExternalAttributes
		 *	Fixes the external attribute pointers within this ActionEvent after being copied to.
		 */
		void FixExternalAttributes();

		/** ActionEvent mSubtype
		 *	A string representing the subtype of this ActionEvent.
		 */
		std::string mSubtype;

		/** ActionEvent mDelay
		 *	An integer representing the delay of this ActionEvent.
		 */
		std::int32_t mDelay;

	public:

		/** ActionEvent sActionEventSubtypeKey
		 *	A string representing the key subtypes are held at within an ActionEvent.
		 */
		static const std::string sActionEventSubtypeKey;

		/** ActionEvent sActionEventDelayKey
		 *	A string representing the key delays are held at within an ActionEvent.
		 */
		static const std::string sActionEventDelayKey;
	};

	/** ConcreteActionFactory of ActionEvent Macro Call
	 *	Defines the ActionEventFactory class that returns Action pointers from its create call.
	 */
	ConcreteActionFactory(ActionEvent);
}