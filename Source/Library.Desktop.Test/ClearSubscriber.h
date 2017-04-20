#pragma once

#include "EventSubscriber.h"

namespace TestingUtilities
{
	class ClearSubscriber : public FieaGameEngine::EventSubscriber
	{

	public:

		virtual void Notify(const FieaGameEngine::EventPublisher& eventPublisher) override;

		class FieaGameEngine::EventQueue* mQueue;
	};
}