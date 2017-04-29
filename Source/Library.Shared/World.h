#pragma once

#include <string>
#include "Attributed.h"
#include "Datum.h"
#include "Sector.h"
#include "EventQueue.h"

#define BT_NO_SIMD_OPERATOR_OVERLOADS
#include "btBulletDynamicsCommon.h"

namespace FieaGameEngine
{	
	/** Forward Declaration of WorldState
	 *	Prevents cyclical dependencies by forward declaring the WorldState class.
	 */
	class WorldState;

	/** World class
	 *	An Attributed class that represents an area that contains Sector instances.
	 */
	class World final : public Attributed
	{

		/** World RTTI_DECLARATIONS Macro Call
		 *	Declares virtual RTTI methods that World defines.
		 */
		RTTI_DECLARATIONS(World, Attributed)

	public:

		/** World Constructor
		 *	Constructs a World of the given name.
		 *	@param name a constant string reference to name the World with, defaulted to "".
		 */
		explicit World(const std::string& name = "");

		/** World Destructor
		 *	Destroys the World.
		 *	Defaulted.
		 */
		~World() = default;

		/** World Copy Constructor
		 *	Constructs a World as a copy of the given World.
		 *	@param otherWorld the constant World reference to copy into this World.
		 */
		World(const World& otherWorld);

		/** World Assignment Operator
		 *	Copies the contents of the given World into this World.
		 *	@param otherWorld the constant World reference to copy into this World.
		 *	@return a World reference to this entity after assignment.
		 */
		World& operator=(const World& otherWorld);

		/** World Move Constructor
		 *	Constructs a copy of the given World, nullifying the given World.
		 *	@param previousWorld the World double reference to copy into this World.
		 */
		World(World&& previousWorld);

		/** World Move Assignment Operator
		 *	Copies the contents of the given World into this World, nullifying the given World.
		 *	@param previousWorld the World double reference to copy into this World.
		 *	@return a World reference to this World after assignment.
		 */
		World& operator=(World&& previousWorld);

		/** World Name
		 *	Returns the name of this World.
		 *	@return a constant string reference representing the name of this World.
		 */
		const std::string& Name() const;

		/** World SetName
		 *	Sets the name of this World to the given string.
		 *	@param name a constant string reference to set this World instance's name to.
		 */
		void SetName(const std::string& name);

		/** World Sectors
		 *	Returns the Datum within this World that contains the Sector instances owned by this World.
		 *	@return a Datum reference containing the Sector instances.
		 */
		Datum& Sectors();

		/** World Sectors
		 *	Returns the Datum within this World that contains the Sector instances owned by this World.
		 *	@return a constant Datum reference containing the Sector instances.
		 */
		const Datum& Sectors() const;

		/** World CreateSector
		 *	Creates a Sector instance given an instance name.
		 *	@param instanceName a constant string reference representing the name of the Sector.
		 *	@return a pointer to the created Sector.
		 */
		Sector* CreateSector(const std::string& instanceName);

		/** World Actions
		 *	Returns the Datum within this World that contains the Action instances owned by this World.
		 *	@return a Datum reference containing the Action instances.
		 */
		Datum& Actions();

		/** World Actions
		 *	Returns the Datum within this World that contains the Action instances owned by this World.
		 *	@return a constant Datum reference containing the Action instances.
		 */
		const Datum& Actions() const;

		/** World CreateAction
		 *	Creates an Action instance given the c++ class name and the instance name.
		 *	@param className a constant string reference representing the name of the c++ class to create.
		 *	@param instanceName a constant string reference representing the name of the Action.
		 *	@return a pointer to the created Action.
		 */
		Action* CreateAction(const std::string& className, const std::string& instanceName);

		/** World Reactions
		 *	Returns the Datum within this World that contains the Reaction instances owned by this World.
		 *	@return a Datum reference containing the Reaction instances.
		 */
		Datum& Reactions();

		/** World Reactions
		 *	Returns the Datum within this World that contains the Reaction instances owned by this World.
		 *	@return a constant Datum reference containing the Reaction instances.
		 */
		const Datum& Reactions() const;

		/** World Update
		 *	Updates the components of this World and notifies the worldState that this is the current World.
		 *	@param worldState a WorldState reference containing the data of the world at this moment in time.
		 */
		void Update(WorldState& worldState);

		/** World Copy
		 *	Creates a copy of this World as a Scope pointer.
		 *	@return a Scope pointer constructed as a World.
		 */
		virtual Scope* Copy() const override;

		/** World Enqueue
		 *	Enqueues the given EventPublisher into the EventQueue.
		 *	@param eventPublisher an EventPublisher reference representing the event to enqueue.
		 *	@param worldState a reference to the current WorldState.
		 *	@param delay an integer representing the amount of delay to enqueue the event with.
		 */
		void Enqueue(EventPublisher& eventPublisher, WorldState& worldState, std::int32_t delay);

		/** World QueueSize
		 *	Returns the size of this World instances EventQueue.
		 *	@return an unsigned integer representing the number of events in the EventQueue.
		 */
		std::uint32_t QueueSize() const;

		void Initialize();

		void RegisterRigidBody(btCollisionShape& shape, btRigidBody& body);

	private:

		/** World InitializeSignatures
		 *	Initializes the prescribed attributes for instances of this class.
		 */
		void InitializeSignatures();

		/** World CopyPrivateDataMembers
		 *	Copies the data members of the given World that are not within the Scope hash.
		 *	@param otherWorld a constant World reference to copy data members from.
		 */
		void CopyPrivateDataMembers(const World& otherWorld);

		/** World FixExternalAttributes
		 *	Fixes the external attribute pointers within this World after being copied to.
		 */
		void FixExternalAttributes();

		/** World mName
		 *	A string representing the name of this World.
		 */
		std::string mName;

		/** World mEventQueue
		 *	An EventQueue instance containing events to be fired in this world.
		 */
		EventQueue mEventQueue;

		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		btDefaultCollisionConfiguration* mCollisionConfiguration;

		///use the default collision dispatcher. For parallel processing you can use a different dispatcher (see Extras/BulletMultiThreaded)
		btCollisionDispatcher* mDispatcher;

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		btBroadphaseInterface* mOverlappingPairCache;

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		btSequentialImpulseConstraintSolver* mSolver;

		btDiscreteDynamicsWorld* mDynamicsWorld;

		btAlignedObjectArray<btCollisionShape*> mCollisionShapes;


	public:

		/** World sWorldNameKey
		 *	A string representing the key names are held at within a World.
		 */
		static const std::string sWorldNameKey;
		static const std::string sGroundDimensionsKey;

		/** World sWorldSectorsKey
		 *	A string representing the key Sector instance's are held at within a World.
		 */
		static const std::string sWorldSectorsKey;
	};
}