#pragma once

#include <chrono>
#include <mutex>
#include "Vector.h"

namespace FieaGameEngine
{
	/** EventPublisher class forward declaration
	 *	Enables the passing of references of EventPublisher instances without including the header.
	 */
	class EventPublisher;

	/** EventQueue class
	 *	A class to hold Event instances and deliver them when necessary.
	 */
	class EventQueue final
	{

	public:

		/** EventQueue Constructor
		 *	Constructs a new EventQueue.
		 *	Implementation defaulted.
		 */
		EventQueue() = default;

		/** EventQueue Destructor
		 *	A destructor for EventQueues.
		 *	Implementation defaulted.
		 */
		~EventQueue();

		/** EventQueue Copy Constructor
		 *	Constructs a new EventQueue as a copy of the given EventQueue.
		 *	@param otherEventQueue a constant reference to an EventQueue that will be copied.
		 */
		EventQueue(const EventQueue& otherEventQueue) = delete;

		/** EventQueue Assignment Operator
		 *	Copies the given EventQueue into this EventQueue.
		 *	@param otherEventQueue a constant reference to an EventQueue that will be copied.
		 *	@return a reference to this EventQueue after copying.
		 */
		EventQueue& operator=(const EventQueue& otherEventQueue) = delete;

		/** EventQueue Move Constructor
		 *	Constructs a new EventQueue by moving the data of the given EventQueue.
		 *	@param previousEventQueue a double reference to an EventQueue that will be moved.
		 */
		EventQueue(EventQueue&& previousEventQueue) = delete;

		/** EventQueue Move Assignment Operator
		 *	Moves the given EventQueue into this EventQueue.
		 *	@param previousEventQueue a double reference to an EventQueue that will be moved.
		 *	@return a reference to this EventQueue after copying.
		 */
		EventQueue& operator=(EventQueue&& previousEventQueue) = delete;

		/** EventQueue Enqueue
		 *	Enqueues a given EventPublisher with a given time and optional delay.
		 *	@param publisher an EventPublisher reference representing the Event to enqueue.
		 *	@param gameTime a constant reference to a GameTime instance that represents the current time.
		 *	@param delay a constant reference to a chrono milliseconds value representing the delay to wait before sending the event, defaulted to zero.
		 */
		void Enqueue(EventPublisher& publisher, const GameTime& gameTime, const std::chrono::milliseconds& delay = std::chrono::milliseconds::zero());

		/** EventQueue Send
		 *	Sends the given EventPublisher immediately, deleting it if necessary.
		 *	@param publisher a constant EventPublisher reference representing the Event to publish.
		 */
		void Send(const EventPublisher& publisher);

		/** EventQueue Update
		 *	Updates the EventQueue by iterating through the Event list and delivering expired events.
		 *	Deletes expired events if necessary.
		 *	@param gameTime a constant reference to a GameTime instance that represents the current time.
		 */
		void Update(const GameTime& gameTime);

		/** EventQueue Clear
		 *	Clears the Event list of this EventQueue.
		 *	Deletes events if necessary.
		 */
		void Clear();

		/** EventQueue IsEmpty
		 *	Returns whether this EventQueue has no more Event instances in its list.
		 *	@return a boolean representing whether this EventQueue is empty.
		 */
		bool IsEmpty() const;

		/** EventQueue Size
		 *	Returns the current size of this EventQueue's Event list.
		 *	Return a constant unsigned integer representing the size of this EventQueue's Event list.
		 */
		const std::uint32_t Size() const;

	private:

		/** EventQueue mQueue
		 *	A Vector of EventPublisher pointers that holds the Events enqueued in the EventQueue.
		 */
		std::vector<EventPublisher*> mQueue;

		mutable std::mutex mMutex;
	};
}