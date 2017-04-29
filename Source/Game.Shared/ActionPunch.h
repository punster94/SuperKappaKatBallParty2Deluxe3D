#pragma once

#include "Action.h"

namespace FieaGameEngine
{
	class ActionPunch : public Action
	{
		RTTI_DECLARATIONS(ActionPunch, Action)

	public:
		explicit ActionPunch(const std::string& name = "");
		virtual ~ActionPunch() = default;
		ActionPunch(const ActionPunch& other);
		ActionPunch& operator=(const ActionPunch& other);
		ActionPunch(ActionPunch&& other);
		ActionPunch& operator=(ActionPunch&& other);

		void SetInstanceName(const std::string& instanceName);
		void SetClassName(const std::string& className);
		virtual void Update(class FieaGameEngine::WorldState& worldState) override;
		virtual Scope* Copy() const override;

	protected:
		void InitializeSignatures();

	private:
		void CopyPrivateDataMembers(const ActionPunch& other);
		void FixExternalAttributes();

		std::string mInstanceName;
		std::string mClassName;
		std::int32_t mPunching;
		std::int32_t mLength;
		std::int32_t mForce;

	public:
		static const std::string sActionPunchInstanceNameKey;
		static const std::string sActionPunchClassNameKey;
		static const std::string sActionPunchPunchingNameKey;
		static const std::string sActionPunchLengthNameKey;
		static const std::string sActionPunchForceNameKey;
	};

	ConcreteActionFactory(ActionPunch);
}