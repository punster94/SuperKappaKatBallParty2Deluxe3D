#pragma once

#include <chrono>
#include "GameTime.h"

namespace FieaGameEngine
{
	/** WorldState class
	 *	A class intended to hold the world state of the game during the run loop.
	 */
	class WorldState final
	{

	public:

		/** WorldState Constructor
		 *	Constructs a WorldState with a null state.
		 */
		WorldState();

		/** WorldState Destructor
		 *	Destroys the WorldState and its data members.
		 *	Defaulted.
		 */
		~WorldState() = default;

		/** WorldState GetGameTime
		 *	Gets the current time of the WorldState in std::chrono::milliseconds.
		 *	@return a constant reference to a std::chrono::milliseconds instance containing the time.
		 */
		const std::chrono::high_resolution_clock::time_point& GetGameTime() const;

		/** Returns the time in seconds since last frame.
		 *	@return Time in seconds since last frame. */
		float DeltaTime() const;

		/** WorldState SetGameTime
		 *	Sets the current time of the WorldState.
		 *	@param gameTime a constant reference to a std::chrono::milliseconds instance to set the time to.
		 */
		void SetGameTime(const std::chrono::high_resolution_clock::time_point& gameTime);

		/** WorldState mWorld
		 *	A World pointer containing the current World being run.
		 */
		class World* mWorld;

		/** WorldState mSector
		 *	A Sector pointer containing the current Sector being run.
		 */
		class Sector* mSector;

		/** WorldState mEntity
		 *	An Entity pointer containing the current Entity being run.
		 */
		class Entity* mEntity;

		/** WorldState mAction
		 *	An Action pointer containing the current Action being run.
		 */
		class Action* mAction;

		/** WorldState mGameTime
		 *	A GameTime instance that handles the time of the game.
		 */
		GameTime mGameTime;
	};
}