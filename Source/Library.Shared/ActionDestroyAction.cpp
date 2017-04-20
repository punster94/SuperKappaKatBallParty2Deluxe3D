#include "pch.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionDestroyAction)

	ActionDestroyAction::ActionDestroyAction(const std::string& name) :
		Action(name)
	{
		InitializeSignatures();
	}

	ActionDestroyAction::ActionDestroyAction(const ActionDestroyAction& otherActionDestroyAction) :
		Action(otherActionDestroyAction), mInstanceName(otherActionDestroyAction.mInstanceName)
	{
		FixExternalAttributes();
	}

	ActionDestroyAction& ActionDestroyAction::operator=(const ActionDestroyAction& otherActionDestroyAction)
	{
		if (this != &otherActionDestroyAction)
		{
			Action::operator=(otherActionDestroyAction);

			CopyPrivateDataMembers(otherActionDestroyAction);
		}

		return *this;
	}

	ActionDestroyAction::ActionDestroyAction(ActionDestroyAction&& previousActionDestroyAction) :
		Action(std::move(previousActionDestroyAction)), mInstanceName(previousActionDestroyAction.mInstanceName)
	{
		FixExternalAttributes();

		previousActionDestroyAction.mInstanceName = std::string();
	}

	ActionDestroyAction& ActionDestroyAction::operator=(ActionDestroyAction&& previousActionDestroyAction)
	{
		if (this != &previousActionDestroyAction)
		{
			Action::operator=(std::move(previousActionDestroyAction));

			CopyPrivateDataMembers(previousActionDestroyAction);

			previousActionDestroyAction.mInstanceName = std::string();
		}

		return *this;
	}

	void ActionDestroyAction::SetInstanceName(const std::string& instanceName)
	{
		mInstanceName = instanceName;
	}

	void ActionDestroyAction::Update(WorldState& worldState)
	{
		worldState.mAction = this;

		if (GetParent() != nullptr)
		{
			Datum* actions = GetParent()->Search(Action::sActionsKey);
			
			if (actions != nullptr)
			{
				for (std::uint32_t i = 0; i < actions->Size(); ++i)
				{
					Action* action = actions->Get<Scope&>(i).As<Action>();

					if (action != nullptr && action->Name() == mInstanceName)
					{
						delete(action);
						break;
					}
				}
			}
		}
	}

	Scope* ActionDestroyAction::Copy() const
	{
		return new ActionDestroyAction(*this);
	}

	void ActionDestroyAction::InitializeSignatures()
	{
		Action::InitializeSignatures();

		AddExternalAttribute(sActionDestroyActionInstanceNameKey, &mInstanceName, 1);
	}

	void ActionDestroyAction::CopyPrivateDataMembers(const ActionDestroyAction& otherActionDestroyAction)
	{
		mInstanceName = otherActionDestroyAction.mInstanceName;

		FixExternalAttributes();
	}

	void ActionDestroyAction::FixExternalAttributes()
	{
		Append(sActionDestroyActionInstanceNameKey).SetStorage(&mInstanceName, 1);
	}

	const std::string ActionDestroyAction::sActionDestroyActionInstanceNameKey = "instance";
}