#include "pch.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Action);

	Action::Action(const std::string& name) :
		Attributed(), mName(name)
	{
		InitializeSignatures();
	}

	Action::Action(const Action& otherAction) :
		Attributed(otherAction), mName(otherAction.mName)
	{

	}

	Action& Action::operator=(const Action& otherAction)
	{
		if (this != &otherAction)
		{
			Attributed::operator=(otherAction);

			CopyPrivateDataMembers(otherAction);
		}

		return *this;
	}

	Action::Action(Action&& previousAction) :
		Attributed(std::move(previousAction)), mName(std::move(previousAction.mName))
	{
		
	}

	Action& Action::operator=(Action&& previousAction)
	{
		if (this != &previousAction)
		{
			Attributed::operator=(std::move(previousAction));

			CopyPrivateDataMembers(previousAction);

			previousAction.mName = std::string();
		}

		return *this;
	}

	const std::string& Action::Name() const
	{
		return mName;
	}

	void Action::SetName(const std::string& name)
	{
		mName = name;
	}

	Datum& Action::Reactions()
	{
		return const_cast<Datum&>(const_cast<const Action*>(this)->Reactions());
	}

	const Datum& Action::Reactions() const
	{
		const Datum* datum = Find(Reaction::sReactionsKey);
		assert(datum != nullptr);
		return *datum;
	}

	void Action::InitializeSignatures()
	{
		Attributed::InitializeSignatures();

		AddExternalAttribute(sActionNameKey, &mName, 1);
		AddEmptyTable(Reaction::sReactionsKey);
	}

	void Action::CopyPrivateDataMembers(const Action& otherAction)
	{
		mName = otherAction.mName;

		FixExternalAttributes();
	}

	void Action::FixExternalAttributes()
	{
		Append(sActionNameKey).SetStorage(&mName, 1);
	}

	const std::string Action::sActionNameKey = "name";

	const std::string Action::sActionsKey = "actions";
}