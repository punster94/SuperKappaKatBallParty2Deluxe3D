#include "pch.h"
#include "InputSubscriber.h"
#include "Gamepad.h"

using namespace std;
using namespace FieaGameEngine;

namespace KatBall
{
	InputSubscriber::InputSubscriber()
	{
		Event<Gamepad>::Subscribe(*this);
	}

	InputSubscriber::~InputSubscriber()
	{
		Event<Gamepad>::Unsubscribe(*this);
	}

	void InputSubscriber::Notify(const EventPublisher& eventPublisher)
	{
		assert(eventPublisher.Is(Event<Gamepad>::TypeIdClass()));
		const Gamepad& received = static_cast<const Event<Gamepad>&>(eventPublisher).Message();

		int port = received.GetPort();
		//NOTE: wButtons can contain multiple button presses at a time.
		WORD buttons = received.GetState()->wButtons;
		//TODO:
		//GetPlayer(port).DoAction(button) //could have a word -> action hashmap (ex. word == XINPUT_GAMEPAD_A function == ActionPunch)
	}
}
