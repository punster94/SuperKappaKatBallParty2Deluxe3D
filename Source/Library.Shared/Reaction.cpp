#include "pch.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Reaction)

	Reaction::Reaction(const std::string& name) :
		ActionList(name)
	{

	}

	Reaction::Reaction(const Reaction& otherReaction) :
		ActionList(otherReaction)
	{

	}

	Reaction::Reaction(Reaction&& previousReaction) :
		ActionList(std::move(previousReaction))
	{

	}

	const std::string Reaction::sReactionsKey = "reactions";
}