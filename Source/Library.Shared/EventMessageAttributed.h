#pragma once

#include "Attributed.h"

namespace FieaGameEngine
{
	/** Forward Declaration of WorldState
	 *	Declares the WorldState class.
	 */
	class WorldState;

	/** EventMessageAttributed class
	 *	An Attributed class that represents a message passed in an event.
	 */
	class EventMessageAttributed : public Attributed
	{

		/** EventMessageAttributed RTTI_DECLARATIONS Macro Call
		 *	Declares virtual RTTI methods that EventMessageAttributed defines.
		 */
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed)

	public:

		/** EventMessageAttributed Constructor
		 *	Constructs a EventMessageAttributed of the given name.
		 *	@param subType a constant string reference representing the type of event.
		 *	@param worldState a WorldState pointer associated with this message.
		 */
		EventMessageAttributed(const std::string& subType, WorldState* worldState = nullptr);

		/** EventMessageAttributed Destructor
		 *	Destroys the EventMessageAttributed.
		 *	Defaulted.
		 */
		virtual ~EventMessageAttributed() = default;

		/** EventMessageAttributed Copy Constructor
		 *	Constructs a EventMessageAttributed as a copy of the given EventMessageAttributed.
		 *	@param otherEventMessageAttributed the constant EventMessageAttributed reference to copy into this EventMessageAttributed.
		 */
		EventMessageAttributed(const EventMessageAttributed& otherEventMessageAttributed);

		/** EventMessageAttributed Assignment Operator
		 *	Copies the contents of the given EventMessageAttributed into this EventMessageAttributed.
		 *	@param otherEventMessageAttributed the constant EventMessageAttributed reference to copy into this EventMessageAttributed.
		 *	@return a EventMessageAttributed reference to this EventMessageAttributed after assignment.
		 */
		EventMessageAttributed& operator=(const EventMessageAttributed& otherEventMessageAttributed);

		/** EventMessageAttributed Move Constructor
		 *	Constructs a copy of the given EventMessageAttributed, nullifying the given EventMessageAttributed.
		 *	@param previousEventMessageAttributed the EventMessageAttributed double reference to copy into this EventMessageAttributed.
		 */
		EventMessageAttributed(EventMessageAttributed&& previousEventMessageAttributed);

		/** EventMessageAttributed Move Assignment Operator
		 *	Copies the contents of the given EventMessageAttributed into this EventMessageAttributed, nullifying the given EventMessageAttributed.
		 *	@param previousEventMessageAttributed the EventMessageAttributed double reference to copy into this EventMessageAttributed.
		 *	@return a EventMessageAttributed reference to this EventMessageAttributed after assignment.
		 */
		EventMessageAttributed& operator=(EventMessageAttributed&& previousEventMessageAttributed);

		/** EventMessageAttributed SetSubtype
		 *	Sets the subtype of this EventMessageAttributed to the given string.
		 *	@param subtype a constant string reference to set this EventMessageAttributed instance's subtype to.
		 */
		void SetSubtype(const std::string& subtype);

		/** EventMessageAttributed GetSubtype
		 *	Returns the subtype of this EventMessageAttributed.
		 *	@return a constant string reference representing the subtype of this EventMessageAttributed.
		 */
		const std::string& GetSubtype() const;

		/** Sector SetWorld
		*	Sets the parent World of this Sector to the given World.
		*	@param sector a World reference to set this Sector instance's World to.
		*/
		void SetWorldState(WorldState& worldState);

		/** EventMessageAttributed GetWorldState
		 *	Returns the WorldState that contains this EventMessageAttributed.
		 *	@return a WorldState pointer pointing to the WorldState that contains this EventMessageAttributed.
		 */
		WorldState* GetWorldState() const;

		/** EventMessageAttributed Copy
		 *	Creates a copy of this EventMessageAttributed as a Scope pointer.
		 *	@return a Scope pointer constructed as a EventMessageAttributed.
		 */
		virtual class Scope* Copy() const override;

	protected:

		/** EventMessageAttributed InitializeSignatures
		 *	Initializes the prescribed attributes for instances of this class.
		 */
		void InitializeSignatures();

	private:

		/** EventMessageAttributed CopyPrivateDataMembers
		 *	Copies the data members of the given EventMessageAttributed that are not within the Scope hash.
		 *	@param otherEventMessageAttributed a constant EventMessageAttributed reference to copy data members from.
		 */
		void CopyPrivateDataMembers(const EventMessageAttributed& otherEventMessageAttributed);

		/** EventMessageAttributed FixExternalAttributes
		 *	Fixes the external attribute pointers within this EventMessageAttributed after being copied to.
		 */
		void FixExternalAttributes();

		/** EventMessageAttributed mSubtype
		 *	A string representing the subtype of this EventMessageAttributed.
		 */
		std::string mSubtype;

		/** EventMessageAttributed mWorldState
		 *	A WorldState pointer that caches off the WorldState of this EventMessageAttributed.
		 */
		WorldState* mWorldState;

	public:

		/** EventMessageAttributed sEventMessageAttributedSubtypeKey
		 *	A string representing the key subtypes are held at within a EventMessageAttributed.
		 */
		static const std::string sEventMessageAttributedSubtypeKey;
	};
}