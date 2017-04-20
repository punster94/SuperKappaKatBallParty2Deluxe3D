#pragma once

namespace FieaGameEngine
{
	/** EventPublisher class forward declaration
	 *	Enables the passing of references of EventPublisher instances without including the header.
	 */
	class EventPublisher;

	/** EventSubscriber class
	 *	A pure virtual class that interfaces the Notify method.
	 */
	class EventSubscriber
	{

	public:

		/** EventSubscriber Notify
		 *	Given an event instance, acts upon it with polymorphic behavior.
		 *	Explicitly deleted.
		 *	@param eventPublisher an constant EventPublisher reference that is the event being thrown.
		 */
		virtual void Notify(const EventPublisher& eventPublisher) = 0;
	};
}