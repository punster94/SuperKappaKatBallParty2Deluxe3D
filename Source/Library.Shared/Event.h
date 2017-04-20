#pragma once

#include <mutex>
#include "EventPublisher.h"
#include "RTTI.h"

namespace FieaGameEngine
{
	/** EventSubscriber class forward declaration
	 *	Enables the passing of references of EventSubscriber instances without including the header.
	 */
	class EventSubscriber;

	/** Event class
	 *	A template EventPublisher class that can contains a message to deliver.
	 */
	template <typename T>
	class Event final : public EventPublisher
	{
		/** RTTI declarations for Event
		 *	Declares RTTI methods that Event will define.
		 */
		RTTI_DECLARATIONS(Event, EventPublisher)

	public:

		/** Event Constructor
		 *	Constructs a new Event given a message and an optional boolean to delete after publishing.
		 *	@param message a constant reference to a T typed message.
		 *	@param deleteAfterPublishing an optional boolean that represents whether to delete this event when published, defaulted to true.
		 */
		explicit Event(const T& message, bool deleteAfterPublishing = true);

		/** Event Destructor
		 *	A destructor for Events.
		 *	Implementation defaulted.
		 */
		~Event() = default;

		/** Event Copy Constructor
		 *	Constructs a new Event as a copy of the given Event.
		 *	@param otherEvent a constant reference to an Event that will be copied.
		 */
		Event(const Event& otherEvent);

		/** Event Assignment Operator
		 *	Copies the given Event into this Event.
		 *	@param otherEvent a constant reference to an Event that will be copied.
		 *	@return a reference to this Event after copying.
		 */
		Event& operator=(const Event& otherEvent);

		/** Event Move Constructor
		 *	Constructs a new Event by moving the data of the given Event.
		 *	@param previousEvent a double reference to an Event that will be moved.
		 */
		Event(Event&& previousEvent);

		/** Event Move Assignment Operator
		 *	Moves the given Event into this Event.
		 *	@param previousEvent a double reference to an Event that will be moved.
		 *	@return a reference to this Event after copying.
		 */
		Event& operator=(Event&& previousEvent);

		/** Event Subscribe
		 *	A static method used to subscribe the given subscriber to the Event type.
		 *	@param subscriber a reference to the EventSubscriber to subscribe to this Event type.
		 */
		static void Subscribe(EventSubscriber& subscriber);

		/** Event Unsubscribe
		 *	A static method used to remove subscription of the given subscriber from the Event type.
		 *	@param subscriber a reference to the EventSubscriber to remove subscription from this Event type.
		 */
		static void Unsubscribe(EventSubscriber& subscriber);

		/** Event UnsubscribeAll
		 *	A static method used to remove subscription of all subscribers from the Event type.
		 */
		static void UnsubscribeAll();

		/** Event Message
		 *	Returns the message of this Event.
		 *	@return a constant reference to the T typed message within this event.
		 */
		const T& Message() const;

	private:

		/** Event mMessage
		 *	A T typed instance that contains the message of this Event.
		 */
		T mMessage;

		/** Event sSubscribers
		 *	A static Vector of EventSubscriber pointers that contains the subscribers to this Event type.
		 */
		static Vector<EventSubscriber*> sSubscribers;

		static std::mutex sMutex;
	};
}

#include "Event.inl"