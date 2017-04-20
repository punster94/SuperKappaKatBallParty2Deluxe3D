#include "pch.h"

namespace TestingUtilities
{
	void ExceptionSubscriber::Notify(const FieaGameEngine::EventPublisher& eventPublisher)
	{
		delete(&eventPublisher);

		throw std::exception("Exception thrown.");
	}
}