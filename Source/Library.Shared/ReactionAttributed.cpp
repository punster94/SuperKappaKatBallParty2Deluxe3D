#include "pch.h"
#include "ReactionAttributed.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "EventSubscriber.h"
#include "EventPublisher.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ReactionAttributed)

	ReactionAttributed::ReactionAttributed(const std::string& name) :
		Reaction(name)
	{
		InitializeSignatures();

		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessageAttributed>::Unsubscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(const ReactionAttributed& otherReactionAttributed) :
		Reaction(otherReactionAttributed)
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed& ReactionAttributed::operator=(const ReactionAttributed& otherReactionAttributed)
	{
		if (this != &otherReactionAttributed)
		{
			ActionList::operator=(otherReactionAttributed);
		}

		return *this;
	}

	ReactionAttributed::ReactionAttributed(ReactionAttributed&& previousReactionAttributed) :
		Reaction(std::move(previousReactionAttributed))
	{
		Event<EventMessageAttributed>::Subscribe(*this);
		Event<EventMessageAttributed>::Unsubscribe(previousReactionAttributed);
	}

	ReactionAttributed& ReactionAttributed::operator=(ReactionAttributed&& previousReactionAttributed)
	{
		if (this != &previousReactionAttributed)
		{
			ActionList::operator=(std::move(previousReactionAttributed));

			Event<EventMessageAttributed>::Unsubscribe(previousReactionAttributed);
		}

		return *this;
	}

	void ReactionAttributed::Notify(const EventPublisher& eventPublisher)
	{
		Event<EventMessageAttributed>* e = eventPublisher.As<Event<EventMessageAttributed>>();

		if (e != nullptr)
		{
			Datum* subtypes = Find(sReactionAttributedSubtypesKey);
			const EventMessageAttributed& message = e->Message();

			for (std::uint32_t i = 0; i < subtypes->Size(); ++i)
			{
				if (message.GetSubtype() == subtypes->Get<std::string&>(i))
				{
					message.CopyAuxiliaryAttributesInto(*this);

					if (message.GetWorldState() != nullptr)
					{
						ActionList::Update(*message.GetWorldState());
					}
				}
			}
		}
	}

	Scope* ReactionAttributed::Copy() const
	{
		return new ReactionAttributed(*this);
	}

	void ReactionAttributed::InitializeSignatures()
	{
		Reaction::InitializeSignatures();

		AddEmptyString(sReactionAttributedSubtypesKey);
	}

	const std::string ReactionAttributed::sReactionAttributedSubtypesKey = "subtypes";
}