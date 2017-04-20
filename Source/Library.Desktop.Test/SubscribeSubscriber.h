#pragma once

#include <mutex>
#include "EventSubscriber.h"
#include "Vector.h"
#include "FooSubscriber.h"

namespace TestingUtilities
{
	class SubscribeSubscriber : public FieaGameEngine::EventSubscriber
	{

	public:

		virtual ~SubscribeSubscriber();

		virtual void Notify(const class FieaGameEngine::EventPublisher& eventPublisher) override;

		FieaGameEngine::Vector<FooSubscriber*> mChildSubscribers;

		std::mutex mMutex;
	};
}