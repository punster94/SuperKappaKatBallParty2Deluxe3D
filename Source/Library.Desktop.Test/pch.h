// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"

// Standard Libraries
#include <cstdint>
#include <string>
#include <typeinfo>
#include <utility>
#include <cmath>
#include <thread>
#include <chrono>
#include <future>
#include <algorithm>

// Local Headers
#include "Foo.h"
#include "SList.h"
#include "Vector.h"
#include "HashMap.h"
#include "FooHashFunctor.h"
#include "Datum.h"
#include "RTTI.h"
#include "Scope.h"
#include "Attributed.h"
#include "AttributedFoo.h"
#include "XmlParseMaster.h"
#include "IXmlParseHelper.h"
#include "FooParseHelper.h"
#include "FooSharedData.h"
#include "ScopeParseHelper.h"
#include "Factory.h"
#include "GameClock.h"
#include "GameTime.h"
#include "WorldState.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "Action.h"
#include "Reaction.h"

// Action Headers
#include "ActionList.h"
#include "ActionListIf.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "ActionExpression.h"
#include "ReactionAttributed.h"
#include "ActionEvent.h"

// Event libraries
#include "EventSubscriber.h"
#include "EventPublisher.h"
#include "Event.h"
#include "EventQueue.h"
#include "FooSubscriber.h"
#include "EventMessageAttributed.h"
#include "ExceptionSubscriber.h"
#include "EventThrowSubscriber.h"
#include "ClearSubscriber.h"
#include "SubscribeSubscriber.h"
#include "UnsubscribeSubscriber.h"

// External libraries
#include "expat.h"