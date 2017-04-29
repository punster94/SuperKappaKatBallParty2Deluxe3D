#include "pch.h"
#include "EventPublisher.h"
#include "EventSubscriber.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(const Vector<EventSubscriber*>& subscriberList, bool deleteAfterPublishing) :
		mSubscriberList(&subscriberList), mDelay(std::chrono::milliseconds::zero()),
		mDeleteAfterPublishing(deleteAfterPublishing)
	{

	}

	EventPublisher::EventPublisher(const EventPublisher& otherEventPublisher) :
		mSubscriberList(otherEventPublisher.mSubscriberList), mTimeEnqueued(otherEventPublisher.mTimeEnqueued),
		mDelay(otherEventPublisher.mDelay), mDeleteAfterPublishing(otherEventPublisher.mDeleteAfterPublishing)
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
		}

		return *this;
	}

	EventPublisher::EventPublisher(EventPublisher&& previousEventPublisher) :
		mSubscriberList(previousEventPublisher.mSubscriberList), mTimeEnqueued(std::move(previousEventPublisher.mTimeEnqueued)),
		mDelay(std::move(previousEventPublisher.mDelay)), mDeleteAfterPublishing(previousEventPublisher.mDeleteAfterPublishing)
	{
		previousEventPublisher.mSubscriberList = nullptr;
		previousEventPublisher.mTimeEnqueued = std::chrono::high_resolution_clock::time_point();
		previousEventPublisher.mDelay = std::chrono::milliseconds::zero();
		previousEventPublisher.mDeleteAfterPublishing = true;
	}

	EventPublisher& EventPublisher::operator=(EventPublisher&& previousEventPublisher)
	{
		if (this != &previousEventPublisher)
		{
			mTimeEnqueued = std::move(previousEventPublisher.mTimeEnqueued);
			mDelay = std::move(previousEventPublisher.mDelay);

			mSubscriberList = previousEventPublisher.mSubscriberList;
			mDeleteAfterPublishing = previousEventPublisher.mDeleteAfterPublishing;

			previousEventPublisher.mSubscriberList = nullptr;
			previousEventPublisher.mTimeEnqueued = std::chrono::high_resolution_clock::time_point();
			previousEventPublisher.mDelay = std::chrono::milliseconds::zero();
			previousEventPublisher.mDeleteAfterPublishing = true;
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
		for (EventSubscriber* subscriber : *mSubscriberList)
		{
			subscriber->Notify(*this);
		}
	}

	bool EventPublisher::DeleteAfterPublishing() const
	{
		return mDeleteAfterPublishing;
	}
}