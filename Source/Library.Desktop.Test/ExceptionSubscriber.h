#pragma once

#include "EventSubscriber.h"

namespace TestingUtilities
{
	class ExceptionSubscriber : public FieaGameEngine::EventSubscriber
	{

	public:

		virtual void Notify(const FieaGameEngine::EventPublisher& EventPublisher) override;
	};
}