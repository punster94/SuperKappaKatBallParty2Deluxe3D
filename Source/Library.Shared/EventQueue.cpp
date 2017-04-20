#include "pch.h"

namespace FieaGameEngine
{
	EventQueue::~EventQueue()
	{
		Clear();
	}

	void EventQueue::Enqueue(EventPublisher& publisher, const GameTime& gameTime, const std::chrono::milliseconds& delay)
	{
		publisher.SetTime(gameTime.CurrentTime(), delay);

		std::lock_guard<std::mutex> lock(mMutex);
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

		{
			std::lock_guard<std::mutex> lock(mMutex);

			// Partition queue into expired and non-expired
			auto expiredStart = std::partition(mQueue.begin(), mQueue.end(), [&gameTime] (EventPublisher* publisher)
			{
				return !publisher->IsExpired(gameTime.CurrentTime());
			});

			// Copy elements into expiredEvents vector
			expiredEvents.insert(expiredEvents.begin(), expiredStart, mQueue.end());

			// Remove expired events from the queue
			mQueue.erase(expiredStart, mQueue.end());
		}

		std::vector<std::future<void>> futures;

		for (EventPublisher* publisher : expiredEvents)
		{
			futures.emplace_back(std::async([publisher]
			{
				publisher->Deliver();

				if (publisher->DeleteAfterPublishing())
				{
					delete(publisher);
				}
			}));
		}

		for (std::future<void>& result : futures)
		{
			result.get();
		}
	}

	void EventQueue::Clear()
	{
		std::lock_guard<std::mutex> lock(mMutex);

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
		std::lock_guard<std::mutex> lock(mMutex);
		return mQueue.empty();
	}

	const std::uint32_t EventQueue::Size() const
	{
		std::lock_guard<std::mutex> lock(mMutex);
		return static_cast<std::uint32_t>(mQueue.size());
	}
}