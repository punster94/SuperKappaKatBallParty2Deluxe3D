#pragma once

#include "World.h"
#include "GameTime.h"
#include "GameClock.h"
#include "WorldState.h"
#include "Renderer.h"
#define BT_NO_SIMD_OPERATOR_OVERLOADS
#include "btBulletDynamicsCommon.h"

#include "GameStateManager.h"

namespace KatBall
{
	class Game final
	{
	public:

		explicit Game(FieaGameEngine::Renderer& renderer);

		void Run();
		FieaGameEngine::WorldState& GetWorldState();

		static Game* GetInstance();

		btDiscreteDynamicsWorld* mDynamicsWorld;

	private:
		void Init();
		void Update();
		void Shutdown();

		void DebugUpdate();

		void LoadAssets();

		void RegisterRigidBody(btCollisionShape& shape, btRigidBody& body);


		FieaGameEngine::Renderer* mRenderer;
		FieaGameEngine::World mWorld;
		FieaGameEngine::Sector* mMenuSector;
		FieaGameEngine::Sector* mGameSector;
		FieaGameEngine::WorldState mWorldState;
		FieaGameEngine::GameTime mGameTime;
		FieaGameEngine::GameClock mGameClock;


		GameStateManager mStateManager;

		btDefaultCollisionConfiguration* mCollisionConfiguration;

		///use the default collision dispatcher. For parallel processing you can use a different dispatcher (see Extras/BulletMultiThreaded)
		btCollisionDispatcher* mDispatcher;

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		btBroadphaseInterface* mOverlappingPairCache;

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		btSequentialImpulseConstraintSolver* mSolver;

		btAlignedObjectArray<btCollisionShape*> mCollisionShapes;

		static Game* sInstance;
	};
}
