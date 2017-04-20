#include "pch.h"

namespace TestingUtilities
{
	void ClearSubscriber::Notify(const FieaGameEngine::EventPublisher&)
	{
		mQueue->Clear();
	}
}