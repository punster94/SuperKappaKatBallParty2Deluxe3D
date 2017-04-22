#include "pch.h"
#include "EventMessageAttributed.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventMessageAttributed)

	EventMessageAttributed::EventMessageAttributed(const std::string& subType, WorldState* worldState) :
		mSubtype(subType), mWorldState(worldState)
	{
		InitializeSignatures();
	}

	EventMessageAttributed::EventMessageAttributed(const EventMessageAttributed& otherEventMessageAttributed) :
		Attributed(otherEventMessageAttributed),mSubtype(otherEventMessageAttributed.mSubtype),
		mWorldState(otherEventMessageAttributed.mWorldState)
	{
		FixExternalAttributes();
	}

	EventMessageAttributed& EventMessageAttributed::operator=(const EventMessageAttributed& otherEventMessageAttributed)
	{
		if (this != &otherEventMessageAttributed)
		{
			Attributed::operator=(otherEventMessageAttributed);

			CopyPrivateDataMembers(otherEventMessageAttributed);
		}
		
		return *this;
	}

	EventMessageAttributed::EventMessageAttributed(EventMessageAttributed&& previousEventMessageAttributed) :
		Attributed(std::move(previousEventMessageAttributed)),
		mSubtype(std::move(previousEventMessageAttributed.mSubtype)),
		mWorldState(previousEventMessageAttributed.mWorldState)
	{
		FixExternalAttributes();

		previousEventMessageAttributed.mWorldState = nullptr;
	}

	EventMessageAttributed& EventMessageAttributed::operator=(EventMessageAttributed&& previousEventMessageAttributed)
	{
		if (this != &previousEventMessageAttributed)
		{
			Attributed::operator=(std::move(previousEventMessageAttributed));

			CopyPrivateDataMembers(previousEventMessageAttributed);

			previousEventMessageAttributed.mSubtype = std::string();
			previousEventMessageAttributed.mWorldState = nullptr;
		}

		return *this;
	}

	void EventMessageAttributed::SetSubtype(const std::string& subtype)
	{
		mSubtype = subtype;
	}

	const std::string& EventMessageAttributed::GetSubtype() const
	{
		return mSubtype;
	}

	void EventMessageAttributed::SetWorldState(WorldState& worldState)
	{
		mWorldState = &worldState;
	}

	WorldState* EventMessageAttributed::GetWorldState() const
	{
		return mWorldState;
	}

	Scope* EventMessageAttributed::Copy() const
	{
		return new EventMessageAttributed(*this);
	}

	void EventMessageAttributed::InitializeSignatures()
	{
		Attributed::InitializeSignatures();

		AddExternalAttribute(sEventMessageAttributedSubtypeKey, &mSubtype, 1);
	}

	void EventMessageAttributed::CopyPrivateDataMembers(const EventMessageAttributed& otherEventMessageAttributed)
	{
		mSubtype = otherEventMessageAttributed.mSubtype;
		mWorldState = otherEventMessageAttributed.mWorldState;

		FixExternalAttributes();
	}

	void EventMessageAttributed::FixExternalAttributes()
	{
		Append(sEventMessageAttributedSubtypeKey).SetStorage(&mSubtype, 1);
	}

	const std::string EventMessageAttributed::sEventMessageAttributedSubtypeKey = "subtype";
}