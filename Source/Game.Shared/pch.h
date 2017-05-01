#pragma once

// Standard libraries
#include <windows.h>
#include <string>
#include <exception>
#include <cassert>
#include <vector>

// External libraries
#include <SFML/Audio.hpp>

//Local libraries
#include "Kat.h"
#include "Constants.h"
#include "Asset.h"
#include "Gamepad.h"
#include "Game.h"
#include "TestDummy.h"
#include "Entity.h"
#include "Camera.h"
#include "RigidBody.h"
#include "XmlParseMaster.h"
#include "ScopeParseHelper.h"
#include "WorldState.h"
#include "Quad.h"
#include "MeshEntity.h"
#include "QuadEntity.h"
#include "Player.h"
#include "MeshGeometry.h"

#define GLM_FORCE_CXX98