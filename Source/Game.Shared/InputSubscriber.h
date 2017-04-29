#pragma once

#include "Event.h"
#include "EventSubscriber.h"

namespace KatBall
{
	class InputSubscriber : public FieaGameEngine::EventSubscriber
	{
	public:
		InputSubscriber();
		~InputSubscriber();

		virtual void Notify(const FieaGameEngine::EventPublisher& eventPublisher) override;
	};
}