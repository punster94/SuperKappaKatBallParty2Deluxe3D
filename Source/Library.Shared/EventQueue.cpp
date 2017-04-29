#include "pch.h"
#include "EventQueue.h"
#include "EventPublisher.h"

namespace FieaGameEngine
{
	EventQueue::~EventQueue()
	{
		Clear();
	}

	void EventQueue::Enqueue(EventPublisher& publisher, const GameTime& gameTime, const std::chrono::milliseconds& delay)
	{
		publisher.SetTime(gameTime.CurrentTime(), delay);

		mQueue.emplace_back(&publisher);
	}

	void EventQueue::Send(const EventPublisher& publisher)
	{
		publisher.Deliver();

		if (publisher.DeleteAfterPublishing())
		{
			delete(&publisher);
		}
	}

	void EventQueue::Update(const GameTime& gameTime)
	{
		std::vector<EventPublisher*> expiredEvents;

		// Partition queue into expired and non-expired
		auto expiredStart = std::partition(mQueue.begin(), mQueue.end(), [&gameTime] (EventPublisher* publisher)
		{
			return !publisher->IsExpired(gameTime.CurrentTime());
		});

		// Copy elements into expiredEvents vector
		expiredEvents.insert(expiredEvents.begin(), expiredStart, mQueue.end());

		// Remove expired events from the queue
		mQueue.erase(expiredStart, mQueue.end());

		for (EventPublisher* publisher : expiredEvents)
		{
			publisher->Deliver();

			if (publisher->DeleteAfterPublishing())
			{
				delete(publisher);
			}
		}
	}

	void EventQueue::Clear()
	{
		for (EventPublisher* publisher : mQueue)
		{
			if (publisher->DeleteAfterPublishing())
			{
				delete(publisher);
			}
		}

		mQueue.clear();
	}

	bool EventQueue::IsEmpty() const
	{
		return mQueue.empty();
	}

	const std::uint32_t EventQueue::Size() const
	{
		return static_cast<std::uint32_t>(mQueue.size());
	}
}