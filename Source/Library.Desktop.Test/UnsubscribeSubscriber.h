#pragma once

#include "EventSubscriber.h"
#include "Vector.h"

namespace TestingUtilities
{
	class UnsubscribeSubscriber : public FieaGameEngine::EventSubscriber
	{

	public:

		virtual void Notify(const class FieaGameEngine::EventPublisher& eventPublisher) override;

		bool mUnsubscribed;
	};
}