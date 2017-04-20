#include "pch.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionCreateAction)

	ActionCreateAction::ActionCreateAction(const std::string& name) :
		Action(name)
	{
		InitializeSignatures();
	}

	ActionCreateAction::ActionCreateAction(const ActionCreateAction& otherActionCreateAction) :
		Action(otherActionCreateAction), mInstanceName(otherActionCreateAction.mInstanceName), 
		mClassName(otherActionCreateAction.mClassName)
	{
		FixExternalAttributes();
	}

	ActionCreateAction& ActionCreateAction::operator=(const ActionCreateAction& otherActionCreateAction)
	{
		if (this != &otherActionCreateAction)
		{
			Action::operator=(otherActionCreateAction);

			CopyPrivateDataMembers(otherActionCreateAction);
		}

		return *this;
	}

	ActionCreateAction::ActionCreateAction(ActionCreateAction&& previousActionCreateAction) :
		Action(std::move(previousActionCreateAction)), mInstanceName(previousActionCreateAction.mInstanceName),
		mClassName(previousActionCreateAction.mClassName)
	{
		FixExternalAttributes();

		previousActionCreateAction.mInstanceName = std::string();
		previousActionCreateAction.mClassName = std::string();
	}

	ActionCreateAction& ActionCreateAction::operator=(ActionCreateAction&& previousAction)
	{
		if (this != &previousAction)
		{
			Action::operator=(std::move(previousAction));

			CopyPrivateDataMembers(previousAction);

			previousAction.mInstanceName = std::string();
			previousAction.mClassName = std::string();
		}

		return *this;
	}

	void ActionCreateAction::SetInstanceName(const std::string& instanceName)
	{
		mInstanceName = instanceName;
	}

	void ActionCreateAction::SetClassName(const std::string& className)
	{
		mClassName = className;
	}

	void ActionCreateAction::Update(WorldState& worldState)
	{
		worldState.mAction = this;

		if (GetParent() != nullptr)
		{
			Action* action = Factory<Action>::Create(mClassName);

			if (action != nullptr)
			{
				action->SetName(mInstanceName);
				GetParent()->Adopt(*action, Action::sActionsKey);
			}
		}
	}

	Scope* ActionCreateAction::Copy() const
	{
		return new ActionCreateAction(*this);
	}

	void ActionCreateAction::InitializeSignatures()
	{
		Action::InitializeSignatures();

		AddExternalAttribute(sActionCreateActionInstanceNameKey, &mInstanceName, 1);
		AddExternalAttribute(sActionCreateActionClassNameKey, &mClassName, 1);
	}

	void ActionCreateAction::CopyPrivateDataMembers(const ActionCreateAction& otherActionCreateAction)
	{
		mInstanceName = otherActionCreateAction.mInstanceName;
		mClassName = otherActionCreateAction.mClassName;

		FixExternalAttributes();
	}

	void ActionCreateAction::FixExternalAttributes()
	{
		Append(sActionCreateActionInstanceNameKey).SetStorage(&mInstanceName, 1);
		Append(sActionCreateActionClassNameKey).SetStorage(&mClassName, 1);
	}

	const std::string ActionCreateAction::sActionCreateActionInstanceNameKey = "instance";

	const std::string ActionCreateAction::sActionCreateActionClassNameKey = "class";
}