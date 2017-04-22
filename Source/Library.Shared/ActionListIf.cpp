#include "pch.h"
#include "ActionListIf.h"
#include "WorldState.h"
#include "ActionExpression.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionListIf)

	ActionListIf::ActionListIf(const std::string& name) :
		ActionList(name)
	{
		InitializeSignatures();
	}

	ActionListIf::ActionListIf(const ActionListIf& otherActionListIf) :
		ActionList(otherActionListIf)
	{
		FixExternalAttributes();
	}

	ActionListIf& ActionListIf::operator=(const ActionListIf& otherActionListIf)
	{
		if (this != &otherActionListIf)
		{
			ActionList::operator=(otherActionListIf);

			CopyPrivateDataMembers(otherActionListIf);
		}

		return *this;
	}

	ActionListIf::ActionListIf(ActionListIf&& previousActionListIf) :
		ActionList(std::move(previousActionListIf))
	{
		FixExternalAttributes();
	}

	ActionListIf& ActionListIf::operator=(ActionListIf&& previousActionListIf)
	{
		if (this != &previousActionListIf)
		{
			ActionList::operator=(std::move(previousActionListIf));

			CopyPrivateDataMembers(previousActionListIf);
		}

		return *this;
	}

	Action* ActionListIf::ThenAction() const
	{
		return FindAction(sActionListIfThenKey);
	}

	Action* ActionListIf::ElseAction() const
	{
		return FindAction(sActionListIfElseKey);
	}

	void ActionListIf::Update(WorldState& worldState)
	{
		worldState.mAction = this;

		Action* expression = FindAction(ActionExpression::sExpressionKey);

		if (expression != nullptr && expression->Is(ActionExpression::TypeIdClass()))
		{
			const Datum& value = expression->As<ActionExpression>()->Evaluate();

			if (value.Type() == Datum::DatumType::Integer && value.Size() > 0U)
			{
				Action* action = FindAction(value.Get<const std::int32_t&>() ? sActionListIfThenKey : sActionListIfElseKey);

				if (action != nullptr)
				{
					action->Update(worldState);
				}
			}
		}
	}

	Scope* ActionListIf::Copy() const
	{
		return new ActionListIf(*this);
	}

	void ActionListIf::InitializeSignatures()
	{
		ActionList::InitializeSignatures();
	}

	void ActionListIf::CopyPrivateDataMembers(const ActionListIf&)
	{
		FixExternalAttributes();
	}

	void ActionListIf::FixExternalAttributes()
	{

	}

	const std::string ActionListIf::sActionListIfThenKey = "then";

	const std::string ActionListIf::sActionListIfElseKey = "else";
}