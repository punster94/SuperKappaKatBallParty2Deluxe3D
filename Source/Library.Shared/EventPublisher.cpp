#include "pch.h"
#include "EventPublisher.h"
#include "EventSubscriber.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(const Vector<EventSubscriber*>& subscriberList, std::mutex& mutex, bool deleteAfterPublishing) :
		mSubscriberList(&subscriberList), mDelay(std::chrono::milliseconds::zero()),
		mDeleteAfterPublishing(deleteAfterPublishing), mMutex(&mutex)
	{

	}

	EventPublisher::EventPublisher(const EventPublisher& otherEventPublisher) :
		mSubscriberList(otherEventPublisher.mSubscriberList), mTimeEnqueued(otherEventPublisher.mTimeEnqueued),
		mDelay(otherEventPublisher.mDelay), mDeleteAfterPublishing(otherEventPublisher.mDeleteAfterPublishing),
		mMutex(otherEventPublisher.mMutex)
	{
		
	}

	EventPublisher& EventPublisher::operator=(const EventPublisher& otherEventPublisher)
	{
		if (this != &otherEventPublisher)
		{
			mSubscriberList = otherEventPublisher.mSubscriberList;
			mTimeEnqueued = otherEventPublisher.mTimeEnqueued;
			mDelay = otherEventPublisher.mDelay;
			mDeleteAfterPublishing = otherEventPublisher.mDeleteAfterPublishing;
			mMutex = otherEventPublisher.mMutex;
		}

		return *this;
	}

	EventPublisher::EventPublisher(EventPublisher&& previousEventPublisher) :
		mSubscriberList(previousEventPublisher.mSubscriberList), mTimeEnqueued(std::move(previousEventPublisher.mTimeEnqueued)),
		mDelay(std::move(previousEventPublisher.mDelay)), mDeleteAfterPublishing(previousEventPublisher.mDeleteAfterPublishing),
		mMutex(previousEventPublisher.mMutex)
	{
		previousEventPublisher.mSubscriberList = nullptr;
		previousEventPublisher.mTimeEnqueued = std::chrono::high_resolution_clock::time_point();
		previousEventPublisher.mDelay = std::chrono::milliseconds::zero();
		previousEventPublisher.mDeleteAfterPublishing = true;
		previousEventPublisher.mMutex = nullptr;
	}

	EventPublisher& EventPublisher::operator=(EventPublisher&& previousEventPublisher)
	{
		if (this != &previousEventPublisher)
		{
			mTimeEnqueued = std::move(previousEventPublisher.mTimeEnqueued);
			mDelay = std::move(previousEventPublisher.mDelay);

			mSubscriberList = previousEventPublisher.mSubscriberList;
			mDeleteAfterPublishing = previousEventPublisher.mDeleteAfterPublishing;
			mMutex = previousEventPublisher.mMutex;

			previousEventPublisher.mSubscriberList = nullptr;
			previousEventPublisher.mTimeEnqueued = std::chrono::high_resolution_clock::time_point();
			previousEventPublisher.mDelay = std::chrono::milliseconds::zero();
			previousEventPublisher.mDeleteAfterPublishing = true;
			previousEventPublisher.mMutex = nullptr;
		}

		return *this;
	}

	void EventPublisher::SetTime(const std::chrono::high_resolution_clock::time_point& timeEnqueued, const std::chrono::milliseconds& delay)
	{
		mTimeEnqueued = timeEnqueued;
		mDelay = delay;
	}

	const std::chrono::high_resolution_clock::time_point& EventPublisher::TimeEnqueued() const
	{
		return mTimeEnqueued;
	}

	const std::chrono::milliseconds& EventPublisher::Delay() const
	{
		return mDelay;
	}

	bool EventPublisher::IsExpired(const std::chrono::high_resolution_clock::time_point& currentTime) const
	{
		return currentTime >= (mTimeEnqueued + mDelay);
	}

	void EventPublisher::Deliver() const
	{
		Vector<EventSubscriber*> buffer;

		{
			std::lock_guard<std::mutex> lock(*mMutex);
			buffer = *mSubscriberList;
		}

		std::vector<std::future<void>> futures;

		for (EventSubscriber* subscriber : buffer)
		{
			futures.emplace_back(std::async([this, subscriber]
			{
				subscriber->Notify(*this);
			}));
		}

		for (std::future<void>& result : futures)
		{
			result.get();
		}
	}

	bool EventPublisher::DeleteAfterPublishing() const
	{
		return mDeleteAfterPublishing;
	}
}