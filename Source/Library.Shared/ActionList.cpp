#include "pch.h"
#include "ActionList.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionList)

	ActionList::ActionList(const std::string& name) :
		Action(name)
	{
		InitializeSignatures();
	}

	ActionList::ActionList(const ActionList& otherActionList) :
		Action(otherActionList)
	{

	}

	ActionList& ActionList::operator=(const ActionList& otherActionList)
	{
		if (this != &otherActionList)
		{
			Action::operator=(otherActionList);
		}

		return *this;
	}

	ActionList::ActionList(ActionList&& previousActionList) :
		Action(std::move(previousActionList))
	{

	}

	ActionList& ActionList::operator=(ActionList&& previousActionList)
	{
		if (this != &previousActionList)
		{
			Action::operator=(std::move(previousActionList));
		}

		return *this;
	}

	Datum& ActionList::Actions()
	{
		return const_cast<Datum&>(const_cast<const ActionList*>(this)->Actions());
	}

	const Datum& ActionList::Actions() const
	{
		const Datum* datum = Find(Action::sActionsKey);
		assert(datum != nullptr);
		return *datum;
	}

	Action* ActionList::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Action* action = Factory<Action>::Create(className);
		action->SetName(instanceName);
		Adopt(*action, Action::sActionsKey);

		return action;
	}

	Action* ActionList::FindAction(const std::string& name) const
	{
		Action* found = nullptr;

		const Datum& actions = Actions();

		for (std::uint32_t i = 0; i < actions.Size(); ++i)
		{
			Action* action = actions.Get<const Scope&>(i).As<Action>();

			if (action != nullptr && action->Name() == name)
			{
				found = action;
				break;
			}
		}

		return found;
	}

	void ActionList::Update(class WorldState& worldState)
	{
		worldState.mAction = this;

		Datum& actions = *Find(Action::sActionsKey);

		for (std::uint32_t i = 0; i < actions.Size(); ++i)
		{
			static_cast<Action*>(&actions.Get<Scope&>(i))->Update(worldState);
		}
	}

	Scope* ActionList::Copy() const
	{
		return new ActionList(*this);
	}

	void ActionList::InitializeSignatures()
	{
		Action::InitializeSignatures();

		AddEmptyTable(Action::sActionsKey);
	}
}