#pragma once

#include <string>
#include "Attributed.h"
#include "Datum.h"
#include "Entity.h"
#include "Action.h"

namespace FieaGameEngine
{
	/** Forward Declaration of World
	 *	Prevents cyclical dependencies by forward declaring the World class.
	 */
	class World;

	/** Forward Declaration of WorldState
	 *	Prevents cyclical dependencies by forward declaring the WorldState class.
	 */
	class WorldState;

	/** Sector class
	 *	An Attributed class that represents an area within a World that contains Entity instances.
	 */
	class Sector final : public Attributed
	{

		/** Sector RTTI_DECLARATIONS Macro Call
		 *	Declares virtual RTTI methods that Sector defines.
		 */
		RTTI_DECLARATIONS(Sector, Attributed)

	public:

		/** Sector Constructor
		 *	Constructs a Sector of the given name.
		 *	@param name a constant string reference to name the Sector with, defaulted to "".
		 */
		explicit Sector(const std::string& name = "");

		/** Sector Destructor
		 *	Destroys the Sector.
		 *	Defaulted.
		 */
		~Sector() = default;

		/** Sector Copy Constructor
		 *	Constructs a Sector as a copy of the given Sector.
		 *	@param otherSector the constant Sector reference to copy into this Sector.
		 */
		Sector(const Sector& otherSector);

		/** Sector Assignment Operator
		 *	Copies the contents of the given Sector into this Sector.
		 *	@param otherSector the constant Sector reference to copy into this Sector.
		 *	@return a Sector reference to this entity after assignment.
		 */
		Sector& operator=(const Sector& otherSector);

		/** Sector Move Constructor
		 *	Constructs a copy of the given Sector, nullifying the given Sector.
		 *	@param previousSector the Sector double reference to copy into this Sector.
		 */
		Sector(Sector&& previousSector);

		/** Sector Move Assignment Operator
		 *	Copies the contents of the given Sector into this Sector, nullifying the given Sector.
		 *	@param previousSector the Sector double reference to copy into this Sector.
		 *	@return a Sector reference to this Sector after assignment.
		 */
		Sector& operator=(Sector&& previousSector);

		/** Sector Name
		 *	Returns the name of this Sector.
		 *	@return a constant string reference representing the name of this Sector.
		 */
		const std::string& Name() const;

		/** Sector SetName
		 *	Sets the name of this Sector to the given string.
		 *	@param name a constant string reference to set this Sector instance's name to.
		 */
		void SetName(const std::string& name);

		/** Sector GetWorld
		 *	Returns the World that contains this Sector.
		 *	@return a World pointer pointing to the World that contains this Sector.
		 */
		World* GetWorld() const;

		/** Sector SetWorld
		 *	Sets the parent World of this Sector to the given World.
		 *	@param sector a World reference to set this Sector instance's World to.
		 */
		void SetWorld(World& world);

		/** Sector Entities
		 *	Returns the Datum within this Sector that contains the Entity instances owned by this Sector.
		 *	@return a Datum reference containing the Entity instances.
		 */
		Datum& Entities();

		/** Sector Entities
		 *	Returns the Datum within this Sector that contains the Entity instances owned by this Sector.
		 *	@return a constant Datum reference containing the Entity instances.
		 */
		const Datum& Entities() const;

		/** Sector CreateEntity
		 *	Creates an Entity instance given the c++ class name and the instance name.
		 *	@param className a constant string reference representing the name of the c++ class to create.
		 *	@param instanceName a constant string reference representing the name of the Entity.
		 *	@return a pointer to the created Entity.
		 */
		Entity* CreateEntity(const std::string& className, const std::string& instanceName);

		/** Sector Actions
		 *	Returns the Datum within this Sector that contains the Action instances owned by this Sector.
		 *	@return a Datum reference containing the Action instances.
		 */
		Datum& Actions();

		/** Sector Actions
		 *	Returns the Datum within this Sector that contains the Action instances owned by this Sector.
		 *	@return a constant Datum reference containing the Action instances.
		 */
		const Datum& Actions() const;

		/** Sector CreateAction
		 *	Creates an Action instance given the c++ class name and the instance name.
		 *	@param className a constant string reference representing the name of the c++ class to create.
		 *	@param instanceName a constant string reference representing the name of the Action.
		 *	@return a pointer to the created Action.
		 */
		Action* CreateAction(const std::string& className, const std::string& instanceName);

		/** Sector Reactions
		 *	Returns the Datum within this Sector that contains the Reaction instances owned by this Sector.
		 *	@return a Datum reference containing the Reaction instances.
		 */
		Datum& Reactions();

		/** Sector Reactions
		 *	Returns the Datum within this Sector that contains the Reaction instances owned by this Sector.
		 *	@return a constant Datum reference containing the Reaction instances.
		 */
		const Datum& Reactions() const;

		/** Sector Update
		 *	Updates the components of this Sector and notifies the worldState that this is the current Sector.
		 *	@param worldState a WorldState reference containing the data of the world at this moment in time.
		 */
		void Update(WorldState& worldState);

		/** Sector Copy
		 *	Creates a copy of this Sector as a Scope pointer.
		 *	@return a Scope pointer constructed as a Sector.
		 */
		virtual Scope* Copy() const override;

		void Initialize(WorldState& worldState);
		void Reset(WorldState& worldState);

	private:

		/** Sector InitializeSignatures
		 *	Initializes the prescribed attributes for instances of this class.
		 */
		void InitializeSignatures();

		/** Sector CopyPrivateDataMembers
		 *	Copies the data members of the given Sector that are not within the Scope hash.
		 *	@param otherSector a constant Sector reference to copy data members from.
		 */
		void CopyPrivateDataMembers(const Sector& otherSector);

		/** Sector FixExternalAttributes
		 *	Fixes the external attribute pointers within this Sector after being copied to.
		 */
		void FixExternalAttributes();

		/** Sector mName
		 *	A string representing the name of this Sector.
		 */
		std::string mName;

		/** Sector mWorld
		 *	A World pointer that caches off the parent of this Sector.
		 */
		World* mWorld;

	public:

		/** Sector sSectorNameKey
		 *	A string representing the key names are held at within a Sector.
		 */
		static const std::string sSectorNameKey;

		/** Sector sSectorEntitiesKey
		 *	A string representing the key Entity instance's are held at within a Sector.
		 */
		static const std::string sSectorEntitiesKey;
	};
}