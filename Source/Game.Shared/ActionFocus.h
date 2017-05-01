#pragma once

#include "Action.h"

namespace KatBall
{
	class ActionFocus : public FieaGameEngine::Action
	{
		RTTI_DECLARATIONS(ActionFocus, FieaGameEngine::Action)

	public:

		explicit ActionFocus(const std::string& name = "");
		virtual ~ActionFocus() = default;

		ActionFocus(const ActionFocus& other);

		virtual void Update(class FieaGameEngine::WorldState& worldState);
		virtual FieaGameEngine::Scope* Copy() const override;

	protected:

		void InitializeSignatures();

	private:

		void Initialize(class FieaGameEngine::WorldState& worldState);

		void FixExternalAttributes();

		float mMinDistance;
		float mMaxDistance;
		float mFocusScale;
		glm::vec4 mBounds;

		bool mInitialized;
		FieaGameEngine::Vector<FieaGameEngine::Entity*> mTargets;

	public:

		static const std::string sTargetsKey;
		static const std::string sMinDistanceKey;
		static const std::string sMaxDistanceKey;
		static const std::string sFocusScaleKey;
		static const std::string sBoundsKey;

		static const float sDefaultMinDistance;
		static const float sDefaultMaxDistance;
		static const float sDefaultFocusScale;
	};

	ConcreteActionFactory(ActionFocus);
}