#pragma once

// Windows libraries
#include <SDKDDKVer.h>

// Standard libraries
#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <cmath>
#include <thread>
#include <chrono>
#include <future>
#include <algorithm>

// Local libraries
#include "SList.h"
#include "Vector.h"
#include "ResizeStrategy.h"
#include "HashMap.h"
#include "RTTI.h"
#include "Datum.h"
#include "Scope.h"
#include "Attributed.h"
#include "IXmlParseHelper.h"
#include "XmlParseMaster.h"
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

// Action libraries
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
#include "EventMessageAttributed.h"

// External libraries
#include "expat.h"