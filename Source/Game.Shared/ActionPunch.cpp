#include "pch.h"
#include "ActionPunch.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionPunch)

		ActionPunch::ActionPunch(const std::string& name) :
		Action(name)
	{
		InitializeSignatures();
	}

	ActionPunch::ActionPunch(const ActionPunch& other) :
		Action(other), mInstanceName(other.mInstanceName),
		mClassName(other.mClassName), mPunching(other.mPunching),
		mLength(other.mLength), mForce(other.mForce)
	{
		FixExternalAttributes();
	}

	ActionPunch& ActionPunch::operator=(const ActionPunch& other)
	{
		if (this != &other)
		{
			Action::operator=(other);

			CopyPrivateDataMembers(other);
		}

		return *this;
	}

	ActionPunch::ActionPunch(ActionPunch&& other) :
		Action(std::move(other)), mInstanceName(other.mInstanceName),
		mClassName(other.mClassName), mPunching(other.mPunching),
		mLength(other.mLength), mForce(other.mForce)
	{
		FixExternalAttributes();

		other.mInstanceName = std::string();
		other.mClassName = std::string();
	}

	ActionPunch& ActionPunch::operator=(ActionPunch&& other)
	{
		if (this != &other)
		{
			Action::operator=(std::move(other));

			CopyPrivateDataMembers(other);

			other.mInstanceName = std::string();
			other.mClassName = std::string();
			other.mPunching = std::int32_t();
			other.mLength = std::int32_t();
			other.mForce = std::int32_t();
		}

		return *this;
	}

	void ActionPunch::SetInstanceName(const std::string& instanceName)
	{
		mInstanceName = instanceName;
	}

	void ActionPunch::SetClassName(const std::string& className)
	{
		mClassName = className;
	}

	void ActionPunch::Update(WorldState& worldState)
	{
		worldState.mAction = this;

		if (mPunching != 0)
		{
			//TODO: Logic for punching
		}
	}

	Scope* ActionPunch::Copy() const
	{
		return new ActionPunch(*this);
	}

	void ActionPunch::InitializeSignatures()
	{
		Action::InitializeSignatures();

		AddExternalAttribute(sActionPunchInstanceNameKey, &mInstanceName, 1);
		AddExternalAttribute(sActionPunchClassNameKey, &mClassName, 1);
		AddExternalAttribute(sActionPunchPunchingNameKey, &mPunching, 1);
		AddExternalAttribute(sActionPunchLengthNameKey, &mLength, 1);
		AddExternalAttribute(sActionPunchForceNameKey, &mForce, 1);
	}

	void ActionPunch::CopyPrivateDataMembers(const ActionPunch& other)
	{
		mInstanceName = other.mInstanceName;
		mClassName = other.mClassName;
		mPunching = other.mPunching;
		mLength = other.mLength;
		mForce = other.mForce;

		FixExternalAttributes();
	}

	void ActionPunch::FixExternalAttributes()
	{
		Append(sActionPunchInstanceNameKey).SetStorage(&mInstanceName, 1);
		Append(sActionPunchClassNameKey).SetStorage(&mClassName, 1);
		Append(sActionPunchForceNameKey).SetStorage(&mForce, 1);
	}

	const std::string ActionPunch::sActionPunchInstanceNameKey = "instance";
	const std::string ActionPunch::sActionPunchClassNameKey = "class";
	const std::string ActionPunch::sActionPunchPunchingNameKey = "punching";
	const std::string ActionPunch::sActionPunchLengthNameKey = "length";
	const std::string ActionPunch::sActionPunchForceNameKey = "force";
}