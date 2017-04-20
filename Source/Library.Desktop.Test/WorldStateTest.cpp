#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(WorldStateTest)
	{
	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);

			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}

		TEST_METHOD(WorldStateConstructor)
		{
			WorldState worldState;

			Assert::IsNull(worldState.mWorld);
			Assert::IsNull(worldState.mSector);
			Assert::IsNull(worldState.mEntity);
			Assert::IsNull(worldState.mAction);
		}

		TEST_METHOD(WorldStateDestructor)
		{
			WorldState worldState, otherWorldState;

			worldState.SetGameTime(otherWorldState.GetGameTime());
		}

		TEST_METHOD(WorldStateGetGameTime)
		{
			WorldState worldState;
			std::chrono::high_resolution_clock::time_point time;

			worldState.SetGameTime(time);

			Assert::IsTrue(worldState.GetGameTime() == time);

			time += std::chrono::milliseconds(1);

			worldState.SetGameTime(time);

			Assert::IsTrue(worldState.GetGameTime() == time);
		}

		TEST_METHOD(WorldStateSetGameTime)
		{
			WorldState worldState, otherWorldState;
			std::chrono::high_resolution_clock::time_point time;

			time += std::chrono::milliseconds(1);

			worldState.SetGameTime(time);
			otherWorldState.SetGameTime(worldState.GetGameTime());

			Assert::IsTrue(worldState.GetGameTime() == time);
			Assert::IsTrue(otherWorldState.GetGameTime() == time);
		}
		
	private:

		static _CrtMemState sStartMemState;
	};

	_CrtMemState WorldStateTest::sStartMemState;
}
