#pragma once

#include <chrono>
#include <mutex>
#include "RTTI.h"
#include "Vector.h"

namespace FieaGameEngine
{
	/** EventSubscriber class forward declaration
	 *	Enables the passing of references of EventSubscriber instances without including the header.
	 */
	class EventSubscriber;

	/** EventPublisher class
	 *	An RTTI class that can deliver themselves to a list of subscribers.
	 */
	class EventPublisher : public RTTI
	{
		/** RTTI declarations for EventPublisher
		 *	Declares RTTI methods that EventPublisher will define.
		 */
		RTTI_DECLARATIONS(EventPublisher, RTTI)

	public:

		/** EventPublisher Constructor
		 *	Constructs a new EventPublisher given a subscriber list and an optional boolean to delete after publishing.
		 *	@param subscriberList a constant reference to a Vector of EventSubscriber pointers that represents a list of subscribers.
		 *	@param mutex a recursive_mutex reference that locks the given subscriber list.
		 *	@param deleteAfterPublishing an optional boolean that represents whether to delete this event when published, defaulted to true.
		 */
		EventPublisher(const Vector<EventSubscriber*>& subscriberList, std::mutex& mutex, bool deleteAfterPublishing = true);

		/** EventPublisher Destructor
		 *	A virtual destructor for EventPublishers.
		 *	Implementation defaulted.
		 */
		virtual ~EventPublisher() = default;

		/** EventPublisher Copy Constructor
		 *	Constructs a new EventPublisher as a copy of the given EventPublisher.
		 *	@param otherEventPublisher a constant reference to an EventPublisher that will be copied.
		 */
		EventPublisher(const EventPublisher& otherEventPublisher);

		/** EventPublisher Assignment Operator
		 *	Copies the given EventPublisher into this EventPublisher.
		 *	@param otherEventPublisher a constant reference to an EventPublisher that will be copied.
		 *	@return a reference to this EventPublisher after copying.
		 */
		EventPublisher& operator=(const EventPublisher& otherEventPublisher);

		/** EventPublisher Move Constructor
		 *	Constructs a new EventPublisher by moving the data of the given EventPublisher.
		 *	@param previousEventPublisher a double reference to an EventPublisher that will be moved.
		 */
		EventPublisher(EventPublisher&& previousEventPublisher);

		/** EventPublisher Move Assignment Operator
		 *	Moves the given EventPublisher into this EventPublisher.
		 *	@param previousEventPublisher a double reference to an EventPublisher that will be moved.
		 *	@return a reference to this EventPublisher after copying.
		 */
		EventPublisher& operator=(EventPublisher&& previousEventPublisher);

		/** EventPublisher SetTime
		 *	Sets the enqueued time and delay of the EventPublisher.
		 *	@param timeEnqueued a constant reference to a chrono time_point representing the point in time the event was enqueued.
		 *	@param delay a constant reference to a chrono milliseconds value representing the time after being enqueued that the event should be delivered.
		 */
		void SetTime(const std::chrono::high_resolution_clock::time_point& timeEnqueued, const std::chrono::milliseconds& delay = std::chrono::milliseconds::zero());

		/** EventPublisher TimeEnqueued
		 *	Returns the time the EventPublisher was enqueued.
		 *	@return a constant reference to a chrono time_point that represents the time enqueued.
		 */
		const std::chrono::high_resolution_clock::time_point& TimeEnqueued() const;

		/** EventPublisher Delay
		 *	Returns the time from when the EventPublisher was enqueued that it should be delivered.
		 *	@return a constant reference to a chrono milliseconds value representing the delay.
		 */
		const std::chrono::milliseconds& Delay() const;

		/** EventPublisher IsExpired
		 *	Returns whether the EventPublisher has expired given the current time.
		 *	@param currentTime a constant reference to a chrono time_point representing the current point in time.
		 *	@return a boolean representing whether the EventPublisher has expired.
		 */
		bool IsExpired(const std::chrono::high_resolution_clock::time_point& currentTime) const;

		/** EventPublisher Deliver
		 *	Delivers this EventPublisher to every one of its subscribers.
		 */
		void Deliver() const;

		/** EventPublisher DeleteAfterPublishing
		 *	Returns whether the EventPublisher should be deleted by its container after being published.
		 *	@return a boolean representing whether the EventPublisher should be deleted after being published.
		 */
		bool DeleteAfterPublishing() const;

	private:

		/** EventPublisher mSubscriberList
		 *	A constant pointer to a Vector of EventSubscriber pointers that represents this EventPublisher's list of subscribers.
		 */
		const Vector<EventSubscriber*>* mSubscriberList;

		/** EventPublisher mTimeEnqueued
		 *	A chrono time_point representing this EventPublisher's current time enqueued.
		 */
		std::chrono::high_resolution_clock::time_point mTimeEnqueued;

		/** EventPublisher mDelay
		 *	A chrono milliseconds value representing this EventPublisher's delay from the time it was enqueued before it is expired.
		 */
		std::chrono::milliseconds mDelay;

		/** EventPublisher mDeleteAfterPublishing
		 *	A boolean representing whether this EventPublisher should be deleted after being published.
		 */
		bool mDeleteAfterPublishing;

		std::mutex* mMutex;
	};
}