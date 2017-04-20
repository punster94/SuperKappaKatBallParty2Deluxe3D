#include "pch.h"

namespace TestingUtilities
{
	void EventThrowSubscriber::Notify(const FieaGameEngine::EventPublisher& eventPublisher)
	{
		using namespace FieaGameEngine;

		Event<Foo>* e = eventPublisher.As<Event<Foo>>();

		Event<Foo>* otherE = new Event<Foo>(*e);
		mWorldState->mWorld->Enqueue(*otherE, *mWorldState, 0);
	}
}