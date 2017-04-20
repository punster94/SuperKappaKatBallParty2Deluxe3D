#pragma once

#include "Reaction.h"
#include "Datum.h"

namespace FieaGameEngine
{
	/** ReactionAttributed class
	 *	A Reaction class that represents a reaction with auxiliary attribute parameters.
	 */
	class ReactionAttributed final : public Reaction
	{

		/** ReactionAttributed RTTI_DECLARATIONS Macro Call
		 *	Declares virtual RTTI methods that ReactionAttributed defines.
		 */
		RTTI_DECLARATIONS(ReactionAttributed, Reaction)

	public:

		/** ReactionAttributed Constructor
		*	Constructs a ReactionAttributed of the given name.
		*	@param name a constant string reference representing the name of the Reaction.
		*/
		ReactionAttributed(const std::string& name = "");

		/** ReactionAttributed Destructor
		 *	Destroys the ReactionAttributed.
		 *	Defaulted.
		 */
		virtual ~ReactionAttributed();

		/** ReactionAttributed Copy Constructor
		 *	Constructs a ReactionAttributed as a copy of the given ReactionAttributed.
		 *	@param otherReactionAttributed the constant ReactionAttributed reference to copy into this ReactionAttributed.
		 */
		ReactionAttributed(const ReactionAttributed& otherReactionAttributed);

		/** ReactionAttributed Assignment Operator
		 *	Copies the contents of the given ReactionAttributed into this ReactionAttributed.
		 *	@param otherReactionAttributed the constant ReactionAttributed reference to copy into this ReactionAttributed.
		 *	@return a ReactionAttributed reference to this ReactionAttributed after assignment.
		 */
		ReactionAttributed& operator=(const ReactionAttributed& otherReactionAttributed);

		/** ReactionAttributed Move Constructor
		 *	Constructs a copy of the given ReactionAttributed, nullifying the given ReactionAttributed.
		 *	@param previousReactionAttributed the ReactionAttributed double reference to copy into this ReactionAttributed.
		 */
		ReactionAttributed(ReactionAttributed&& previousReactionAttributed);

		/** ReactionAttributed Move Assignment Operator
		 *	Copies the contents of the given ReactionAttributed into this ReactionAttributed, nullifying the given ReactionAttributed.
		 *	@param previousReactionAttributed the ReactionAttributed double reference to copy into this ReactionAttributed.
		 *	@return a ReactionAttributed reference to this ReactionAttributed after assignment.
		 */
		ReactionAttributed& operator=(ReactionAttributed&& previousReactionAttributed);

		/** ReactionAttributed Notify
		 *	Updates this reaction when an event is thrown that matches one of this ReactionAttributed instances subtypes.
		 *	@param eventPublisher a constant reference to an EventPublisher that represents the thrown event.
		 */
		virtual void Notify(const EventPublisher& eventPublisher) override;

		/** ReactionAttributed Copy
		 *	Creates a copy of this ReactionAttributed as a Scope pointer.
		 *	@return a Scope pointer constructed as a ReactionAttributed.
		 */
		virtual class Scope* Copy() const override;

	protected:

		/** ReactionAttributed InitializeSignatures
		 *	Initializes the prescribed attributes for instances of this class.
		 */
		void InitializeSignatures();

	public:

		/** ReactionAttributed sReactionAttributedSubtypesKey
		 *	A string representing the key subtypes are held at within a ReactionAttributed.
		 */
		static const std::string sReactionAttributedSubtypesKey;
	};

	/** ConcreteReactionFactory of ReactionAttributed Macro Call
	 *	Defines the ReactionAttributedFactory class that returns Reaction pointers from its create call.
	 */
	ConcreteReactionFactory(ReactionAttributed);
}