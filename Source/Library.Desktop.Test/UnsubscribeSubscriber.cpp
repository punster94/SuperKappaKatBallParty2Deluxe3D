#include "pch.h"

namespace TestingUtilities
{
	void UnsubscribeSubscriber::Notify(const FieaGameEngine::EventPublisher&)
	{
		FieaGameEngine::Event<Foo>::Subscribe(*this);
		mUnsubscribed = true;
	}
}