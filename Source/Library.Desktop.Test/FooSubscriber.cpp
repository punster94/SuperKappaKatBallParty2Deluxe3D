#include "pch.h"

namespace TestingUtilities
{
	void FooSubscriber::Notify(const FieaGameEngine::EventPublisher& eventPublisher)
	{
		std::lock_guard<std::mutex> lock(mMutex);

		FieaGameEngine::Event<Foo>* fooEvent = eventPublisher.As<FieaGameEngine::Event<Foo>>();

		if (fooEvent != nullptr)
		{
			mFoo = fooEvent->Message();
		}

		++mNotifyCount;
	}
}