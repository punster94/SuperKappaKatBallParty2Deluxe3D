#pragma once

#include <mutex>
#include "EventSubscriber.h"
#include "Foo.h"

namespace TestingUtilities
{
	class FooSubscriber : public FieaGameEngine::EventSubscriber
	{

	public:

		virtual void Notify(const FieaGameEngine::EventPublisher& eventPublisher) override;

		Foo mFoo;

		std::uint32_t mNotifyCount;

		std::mutex mMutex;
	};
}