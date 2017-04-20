#pragma once

#include "EventSubscriber.h"

namespace TestingUtilities
{
	class EventThrowSubscriber : public FieaGameEngine::EventSubscriber
	{
	public:

		virtual void Notify(const class FieaGameEngine::EventPublisher& eventPublisher) override;

		class FieaGameEngine::WorldState* mWorldState;
	};
}