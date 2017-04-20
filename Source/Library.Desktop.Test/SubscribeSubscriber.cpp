#include "pch.h"

namespace TestingUtilities
{
	SubscribeSubscriber::~SubscribeSubscriber()
	{
		for (FooSubscriber* child : mChildSubscribers)
		{
			delete(child);
		}
	}

	void SubscribeSubscriber::Notify(const FieaGameEngine::EventPublisher&)
	{
		FooSubscriber* child = new FooSubscriber();
		FieaGameEngine::Event<Foo>::Subscribe(*child);

		std::lock_guard<std::mutex> lock(mMutex);
		mChildSubscribers.PushBack(child);
	}
}