#include "pch.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionEvent)

	ActionEvent::ActionEvent(const std::string& name) :
		Action(name), mDelay(0)
	{
		InitializeSignatures();
	}

	ActionEvent::ActionEvent(const ActionEvent& otherActionEvent) :
		Action(otherActionEvent), mSubtype(otherActionEvent.mSubtype), mDelay(otherActionEvent.mDelay)
	{
		FixExternalAttributes();
	}

	ActionEvent& ActionEvent::operator=(const ActionEvent& otherActionEvent)
	{
		if (this != &otherActionEvent)
		{
			Action::operator=(otherActionEvent);

			CopyPrivateDataMembers(otherActionEvent);
		}

		return *this;
	}

	ActionEvent::ActionEvent(ActionEvent&& previousActionEvent) :
		Action(std::move(previousActionEvent)), mSubtype(std::move(previousActionEvent.mSubtype)),
		mDelay(std::move(previousActionEvent.mDelay))
	{
		FixExternalAttributes();

		previousActionEvent.mDelay = std::int32_t();
	}

	ActionEvent& ActionEvent::operator=(ActionEvent&& previousActionEvent)
	{
		if (this != &previousActionEvent)
		{
			Action::operator=(std::move(previousActionEvent));

			CopyPrivateDataMembers(previousActionEvent);

			previousActionEvent.mSubtype = std::string();
			previousActionEvent.mDelay = std::int32_t();
		}
		
		return *this;
	}

	void ActionEvent::SetSubtype(const std::string& subtype)
	{
		mSubtype = subtype;
	}

	const std::string& ActionEvent::GetSubtype() const
	{
		return mSubtype;
	}

	void ActionEvent::SetDelay(const std::int32_t delay)
	{
		mDelay = delay;
	}

	std::int32_t ActionEvent::GetDelay() const
	{
		return mDelay;
	}

	void ActionEvent::Update(WorldState& worldState)
	{
		EventMessageAttributed message(mSubtype, &worldState);

		CopyAuxiliaryAttributesInto(message);

		worldState.mAction = this;

		if (worldState.mWorld != nullptr)
		{
			Event<EventMessageAttributed>* e = new Event<EventMessageAttributed>(message);

			worldState.mWorld->Enqueue(*e, worldState, mDelay);
		}
	}

	Scope* ActionEvent::Copy() const
	{
		return new ActionEvent(*this);
	}

	void ActionEvent::InitializeSignatures()
	{
		Action::InitializeSignatures();

		AddExternalAttribute(sActionEventSubtypeKey, &mSubtype, 1);
		AddExternalAttribute(sActionEventDelayKey, &mDelay, 1);
	}

	void ActionEvent::CopyPrivateDataMembers(const ActionEvent& otherActionEvent)
	{
		mSubtype = otherActionEvent.mSubtype;
		mDelay = otherActionEvent.mDelay;

		FixExternalAttributes();
	}

	void ActionEvent::FixExternalAttributes()
	{
		Append(sActionEventSubtypeKey).SetStorage(&mSubtype, 1);
		Append(sActionEventDelayKey).SetStorage(&mDelay, 1);
	}

	const std::string ActionEvent::sActionEventSubtypeKey = "subtype";

	const std::string ActionEvent::sActionEventDelayKey = "delay";
}