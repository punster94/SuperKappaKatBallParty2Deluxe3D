#pragma once

#include "ActionList.h"
#include "EventSubscriber.h"

namespace FieaGameEngine
{
	/** Reaction class
	 *	An ActionList EventSubscriber class that can be contained in a Reactions list of a Scope.
	 */
	class Reaction : public ActionList, public EventSubscriber
	{
		/** Reaction RTTI_DECLARATIONS Macro Call
		 *	Declares virtual RTTI methods that Reaction defines.
		 */
		RTTI_DECLARATIONS(Reaction, ActionList)

	public:

		/** Reaction Constructor
		 *	Constructs a Reaction of the given name.
		 *	@param name a constant string reference to name the Reaction with, defaulted to "".
		 */
		Reaction(const std::string& name = "");

		/** Reaction Destructor
		 *	Destroys the Reaction.
		 *	Defaulted.
		 */
		virtual ~Reaction() = default;

		/** Reaction Copy Constructor
		 *	Constructs a Reaction as a copy of the given Reaction.
		 *	@param otherReaction the constant Reaction reference to copy into this Reaction.
		 */
		Reaction(const Reaction& otherReaction);

		/** Reaction Move Constructor
		 *	Constructs a copy of the given Reaction, nullifying the given Reaction.
		 *	@param previousReaction the Reaction double reference to copy into this Reaction.
		 */
		Reaction(Reaction&& previousReaction);

	public:

		/** Reaction sReactionsKey
		 *	A string representing the key Reaction instance's are held at within a Scope.
		 */
		static const std::string sReactionsKey;
	};

	/** ConcreteReactionFactory Macro Definition
	 *	Defines the macro that creates the classes for Reaction Factories.
	 */
#define ConcreteReactionFactory(ConcreteProductT) ConcreteFactory(Reaction, ConcreteProductT)
}