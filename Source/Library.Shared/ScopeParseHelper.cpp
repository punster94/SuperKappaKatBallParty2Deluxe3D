#include "pch.h"
#include "ScopeParseHelper.h"
#include "World.h"
#include "ActionListIf.h"
#include "ActionExpression.h"
#include "Reaction.h"

namespace FieaGameEngine
{
#pragma region

	RTTI_DEFINITIONS(ScopeParseHelper::ScopeSharedData);

	ScopeParseHelper::ScopeSharedData::ScopeSharedData() :
		XmlParseMaster::SharedData(), mScope(nullptr)
	{
		PushState(State::NotParsing);
	}

	ScopeParseHelper::ScopeSharedData::~ScopeSharedData()
	{
		if (mScope != nullptr)
		{
			delete(mScope);
		}
	}

	void ScopeParseHelper::ScopeSharedData::Initialize()
	{
		if (mScope != nullptr)
		{
			delete(mScope);

			mScope = nullptr;
		}
	}

	XmlParseMaster::SharedData* ScopeParseHelper::ScopeSharedData::Clone() const
	{
		return new ScopeSharedData();
	}

	void ScopeParseHelper::ScopeSharedData::PushState(State state)
	{
		mStateStack.PushFront(state);
	}

	ScopeParseHelper::State ScopeParseHelper::ScopeSharedData::PeekState()
	{
		return mStateStack.Front();
	}

	void ScopeParseHelper::ScopeSharedData::PopState()
	{
		mStateStack.PopFront();
	}

#pragma endregion ScopeParseHelper::ScopeSharedData Implementation

#pragma region

	ScopeParseHelper::ScopeParseHelper()
	{

	}

	ScopeParseHelper::~ScopeParseHelper()
	{
		IXmlParseHelper::~IXmlParseHelper();
	}

	void ScopeParseHelper::Initialize()
	{
		IXmlParseHelper::Initialize();
	}

	bool ScopeParseHelper::StartElementHandler(XmlParseMaster::SharedData* sharedData, const std::string& name, HashMap<std::string, std::string>& attributes) const
	{
		if (sharedData == nullptr)
		{
			return false;
		}

		ScopeSharedData* scopeSharedData = sharedData->As<ScopeSharedData>();

		if (scopeSharedData == nullptr)
		{
			return false;
		}

		if (!sStartElementActions.ContainsKey(name))
		{
			return false;
		}

		(const_cast<ScopeParseHelper*>(this)->*sStartElementActions[name])(name, scopeSharedData, attributes);

		return true;
	}

	bool ScopeParseHelper::EndElementHandler(XmlParseMaster::SharedData* sharedData, const std::string& name) const
	{
		if (sharedData == nullptr)
		{
			return false;
		}

		ScopeSharedData* scopeSharedData = sharedData->As<ScopeSharedData>();

		if (scopeSharedData == nullptr)
		{
			return false;
		}

		if (!sEndElementActions.ContainsKey(name))
		{
			return false;
		}

		(const_cast<ScopeParseHelper*>(this)->*sEndElementActions[name])(name, scopeSharedData);

		return true;
	}

	bool ScopeParseHelper::CharDataHandler(XmlParseMaster::SharedData* sharedData, const std::string&, const std::uint32_t) const
	{
		if (sharedData == nullptr)
		{
			return false;
		}

		ScopeSharedData* scopeSharedData = sharedData->As<ScopeSharedData>();

		if (scopeSharedData == nullptr)
		{
			return false;
		}

		return true;
	}

	IXmlParseHelper* ScopeParseHelper::Clone() const
	{
		return new ScopeParseHelper();
	}

#pragma endregion ScopeParseHelper Implementation

#pragma region

	void ScopeParseHelper::PrimitiveStartElementAction(const std::string& element, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes)
	{
		if (!AllowsForDataMembers(sharedData) || sharedData->mScope == nullptr)
		{
			throw std::exception("Invalid state detected.");
		}

		Datum& datum = sharedData->mScope->Append(attributes[sNameAttribute]);

		std::pair<Datum::DatumType, State> pair = sPrimitiveTypeMap[element];

		if (datum.Size() == 0 && datum.Type() == Datum::DatumType::Unknown)
		{
			datum.SetType(pair.first);
		}

		if (datum.IsExternal())
		{
			datum.SetFromString(attributes[sValueAttribute]);
		}
		else
		{
			datum.PushBackFromString(attributes[sValueAttribute]);
		}

		sharedData->PushState(pair.second);
	}

	void ScopeParseHelper::ScopeStartElementAction(const std::string&, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes)
	{
		if (sharedData->PeekState() != State::NotParsing && !AllowsForDataMembers(sharedData))
		{
			throw std::exception("Invalid state detected.");
		}

		if (sharedData->mScope == nullptr)
		{
			sharedData->mScope = new Scope();
		}
		else
		{
			sharedData->mScope = &sharedData->mScope->AppendScope(attributes[sNameAttribute]);
		}

		sharedData->PushState(State::ParsingScope);
	}

	void ScopeParseHelper::WorldStartElementAction(const std::string&, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes)
	{
		if (sharedData->PeekState() != State::NotParsing)
		{
			throw std::exception("Invalid state detected.");
		}

		sharedData->mScope = new World(attributes[sNameAttribute]);

		sharedData->PushState(State::ParsingWorld);
	}

	void ScopeParseHelper::SectorStartElementAction(const std::string&, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes)
	{
		if (sharedData->PeekState() != State::ParsingWorld || sharedData->mScope == nullptr)
		{
			throw std::exception("Invalid state detected.");
		}

		World* world = sharedData->mScope->As<World>();

		sharedData->mScope = world->CreateSector(attributes[sNameAttribute]);
		
		sharedData->PushState(State::ParsingSector);
	}

	void ScopeParseHelper::EntityStartElementAction(const std::string&, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes)
	{
		if (sharedData->PeekState() != State::ParsingSector || sharedData->mScope == nullptr)
		{
			throw std::exception("Invalid state detected.");
		}
		
		Sector* sector = sharedData->mScope->As<Sector>();

		sharedData->mScope = sector->CreateEntity(attributes[sClassAttribute], attributes[sNameAttribute]);

		sharedData->PushState(State::ParsingEntity);
	}

	void ScopeParseHelper::ActionStartElementAction(const std::string&, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes)
	{
		if (!AllowsForDataMembers(sharedData) || sharedData->mScope == nullptr)
		{
			throw std::exception("Invalid state detected.");
		}

		Action* action = Factory<Action>::Create(attributes[sClassAttribute]);
		action->SetName(attributes[sNameAttribute]);
		sharedData->mScope->Adopt(*action, Action::sActionsKey);
		sharedData->mScope = action;

		sharedData->PushState(State::ParsingAction);
	}

	void ScopeParseHelper::IfStartElementAction(const std::string&, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes)
	{
		if (!AllowsForDataMembers(sharedData) || sharedData->mScope == nullptr)
		{
			throw std::exception("Invalid state detected.");
		}

		ActionListIf* action = new ActionListIf(attributes[sNameAttribute]);
		sharedData->mScope->Adopt(*action, Action::sActionsKey);
		sharedData->mScope = action;

		sharedData->PushState(State::ParsingIf);
	}

	void ScopeParseHelper::ThenStartElementAction(const std::string&, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes)
	{
		if (sharedData->PeekState() != State::ParsingIf || sharedData->mScope == nullptr)
		{
			throw std::exception("Invalid state detected.");
		}

		Action* action = Factory<Action>::Create(attributes[sClassAttribute]);
		action->SetName(ActionListIf::sActionListIfThenKey);
		sharedData->mScope->Adopt(*action, Action::sActionsKey);
		sharedData->mScope = action;

		sharedData->PushState(State::ParsingAction);
	}

	void ScopeParseHelper::ElseStartElementAction(const std::string&, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes)
	{
		if (sharedData->PeekState() != State::ParsingIf || sharedData->mScope == nullptr)
		{
			throw std::exception("Invalid state detected.");
		}

		Action* action = Factory<Action>::Create(attributes[sClassAttribute]);
		action->SetName(ActionListIf::sActionListIfElseKey);
		sharedData->mScope->Adopt(*action, Action::sActionsKey);
		sharedData->mScope = action;

		sharedData->PushState(State::ParsingAction);
	}

	void ScopeParseHelper::ExpressionStartElementAction(const std::string&, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes)
	{
		if (!AllowsForDataMembers(sharedData) || sharedData->mScope == nullptr)
		{
			throw std::exception("Invalid state detected");
		}

		ActionExpression* expression = new ActionExpression(ActionExpression::sExpressionKey);
		expression->SetExpression(attributes[sValueAttribute]);
		sharedData->mScope->Adopt(*expression, Action::sActionsKey);
		sharedData->mScope = expression;

		sharedData->PushState(State::ParsingExpression);
	}

	void ScopeParseHelper::ReactionStartElementAction(const std::string&, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes)
	{
		if (!AllowsForDataMembers(sharedData) || sharedData->mScope == nullptr)
		{
			throw std::exception("Invalid state detected");
		}

		Reaction* reaction = Factory<Reaction>::Create(attributes[sClassAttribute]);
		reaction->SetName(attributes[sNameAttribute]);
		sharedData->mScope->Adopt(*reaction, Reaction::sReactionsKey);
		sharedData->mScope = reaction;

		sharedData->PushState(State::ParsingReaction);
	}

	void ScopeParseHelper::SubfileStartElementAction(const std::string&, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes)
	{
		UNREFERENCED_PARAMETER(sharedData);
		UNREFERENCED_PARAMETER(attributes);
		// TODO
	}

#pragma endregion ScopeParseHelper StartElementAction Functions

#pragma region

	void ScopeParseHelper::PrimitiveEndElementAction(const std::string& element, ScopeSharedData* sharedData)
	{
		if (sharedData->PeekState() != sPrimitiveTypeMap[element].second)
		{
			throw std::exception("Invalid state detected.");
		}

		sharedData->PopState();
	}

	void ScopeParseHelper::ScopeEndElementAction(const std::string&, ScopeSharedData* sharedData)
	{
		if (sharedData->PeekState() != State::ParsingScope)
		{
			throw std::exception("Invalid state detected.");
		}

		if (sharedData->mScope->GetParent() != nullptr)
		{
			sharedData->mScope = sharedData->mScope->GetParent();
		}

		sharedData->PopState();
	}

	void ScopeParseHelper::WorldEndElementAction(const std::string&, ScopeSharedData* sharedData)
	{
		if (sharedData->PeekState() != State::ParsingWorld)
		{
			throw std::exception("Invalid state detected.");
		}

		sharedData->PopState();
	}

	void ScopeParseHelper::SectorEndElementAction(const std::string&, ScopeSharedData* sharedData)
	{
		if (sharedData->PeekState() != State::ParsingSector)
		{
			throw std::exception("Invalid state detected.");
		}

		sharedData->mScope = sharedData->mScope->GetParent();

		sharedData->PopState();
	}

	void ScopeParseHelper::EntityEndElementAction(const std::string&, ScopeSharedData* sharedData)
	{
		if (sharedData->PeekState() != State::ParsingEntity)
		{
			throw std::exception("Invalid state detected.");
		}

		sharedData->mScope = sharedData->mScope->GetParent();

		sharedData->PopState();
	}

	void ScopeParseHelper::ActionEndElementAction(const std::string&, ScopeSharedData* sharedData)
	{
		if (sharedData->PeekState() != State::ParsingAction)
		{
			throw std::exception("Invalid state detected.");
		}

		sharedData->mScope = sharedData->mScope->GetParent();

		sharedData->PopState();
	}

	void ScopeParseHelper::IfEndElementAction(const std::string&, ScopeSharedData* sharedData)
	{
		if (sharedData->PeekState() != State::ParsingIf)
		{
			throw std::exception("Invalid state detected.");
		}

		sharedData->mScope = sharedData->mScope->GetParent();

		sharedData->PopState();
	}

	void ScopeParseHelper::ExpressionEndElementAction(const std::string&, ScopeSharedData* sharedData)
	{
		if (sharedData->PeekState() != State::ParsingExpression)
		{
			throw std::exception("Invalid state detected.");
		}

		sharedData->mScope = sharedData->mScope->GetParent();

		sharedData->PopState();
	}

	void ScopeParseHelper::ReactionEndElementAction(const std::string&, ScopeSharedData* sharedData)
	{
		if (sharedData->PeekState() != State::ParsingReaction)
		{
			throw std::exception("Invalid state detected.");
		}

		sharedData->mScope = sharedData->mScope->GetParent();

		sharedData->PopState();
	}

	void ScopeParseHelper::SubfileEndElementAction(const std::string&, ScopeSharedData* sharedData)
	{
		UNREFERENCED_PARAMETER(sharedData);
		// TODO
	}

#pragma endregion ScopeParseHelper EndElementAction Functions

	bool ScopeParseHelper::AllowsForDataMembers(ScopeSharedData* sharedData)
	{
		State state = sharedData->PeekState();

		return state == State::ParsingScope || state == State::ParsingWorld
			|| state == State::ParsingSector || state == State::ParsingEntity
			|| state == State::ParsingAction || state == State::ParsingIf
			|| state == State::ParsingExpression || state == State::ParsingReaction;
	}

#pragma region

	const HashMap<std::string, ScopeParseHelper::StartElementAction> ScopeParseHelper::sStartElementActions =
	{
		{ "integer", &ScopeParseHelper::PrimitiveStartElementAction },
		{ "float", &ScopeParseHelper::PrimitiveStartElementAction },
		{ "vector", &ScopeParseHelper::PrimitiveStartElementAction },
		{ "matrix", &ScopeParseHelper::PrimitiveStartElementAction },
		{ "string", &ScopeParseHelper::PrimitiveStartElementAction },
		{ "scope", &ScopeParseHelper::ScopeStartElementAction },
		{ "world", &ScopeParseHelper::WorldStartElementAction },
		{ "sector", &ScopeParseHelper::SectorStartElementAction },
		{ "entity", &ScopeParseHelper::EntityStartElementAction },
		{ "action", &ScopeParseHelper::ActionStartElementAction },
		{ "if", &ScopeParseHelper::IfStartElementAction },
		{ "then", &ScopeParseHelper::ThenStartElementAction },
		{ "else", &ScopeParseHelper::ElseStartElementAction },
		{ "expression", &ScopeParseHelper::ExpressionStartElementAction },
		{ "reaction", &ScopeParseHelper::ReactionStartElementAction },
		{ "subfile", &ScopeParseHelper::SubfileStartElementAction }
	};

	const HashMap<std::string, ScopeParseHelper::EndElementAction> ScopeParseHelper::sEndElementActions =
	{
		{ "integer", &ScopeParseHelper::PrimitiveEndElementAction },
		{ "float", &ScopeParseHelper::PrimitiveEndElementAction },
		{ "vector", &ScopeParseHelper::PrimitiveEndElementAction },
		{ "matrix", &ScopeParseHelper::PrimitiveEndElementAction },
		{ "string", &ScopeParseHelper::PrimitiveEndElementAction },
		{ "scope", &ScopeParseHelper::ScopeEndElementAction },
		{ "world", &ScopeParseHelper::WorldEndElementAction },
		{ "sector", &ScopeParseHelper::SectorEndElementAction },
		{ "entity", &ScopeParseHelper::EntityEndElementAction },
		{ "action", &ScopeParseHelper::ActionEndElementAction },
		{ "if", &ScopeParseHelper::IfEndElementAction },
		{ "then", &ScopeParseHelper::ActionEndElementAction },
		{ "else", &ScopeParseHelper::ActionEndElementAction },
		{ "expression", &ScopeParseHelper::ExpressionEndElementAction },
		{ "reaction", &ScopeParseHelper::ReactionEndElementAction },
		{ "subfile", &ScopeParseHelper::SubfileEndElementAction }
	};

	const HashMap<std::string, std::pair<Datum::DatumType, ScopeParseHelper::State>> ScopeParseHelper::sPrimitiveTypeMap =
	{
		{ "integer", { Datum::DatumType::Integer, ScopeParseHelper::State::ParsingInteger } },
		{ "float", { Datum::DatumType::Float, ScopeParseHelper::State::ParsingFloat } },
		{ "vector", { Datum::DatumType::Vec4, ScopeParseHelper::State::ParsingVector } },
		{ "matrix", { Datum::DatumType::Mat4, ScopeParseHelper::State::ParsingMatrix } },
		{ "string", { Datum::DatumType::String, ScopeParseHelper::State::ParsingString } },
	};

	const std::string ScopeParseHelper::sNameAttribute = "name";

	const std::string ScopeParseHelper::sValueAttribute = "value";

	const std::string ScopeParseHelper::sClassAttribute = "class";

	const std::string ScopeParseHelper::sInstanceAttribute = "instance";

	const std::string ScopeParseHelper::sPathAttribute = "path";

#pragma endregion ScopeParseHelper Static Data
}