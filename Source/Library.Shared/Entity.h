#pragma once

#include <string>
#include "Attributed.h"
#include "Datum.h"
#include "ActionList.h"

namespace FieaGameEngine
{
	/** Forward Declaration of Sector
	 *	Prevents cyclical dependencies by forward declaring the Sector class.
	 */
	class Sector;

	/** Forward Declaration of WorldState
	 *	Prevents cyclical dependencies by forward declaring the WorldState class.
	 */
	class WorldState;
	
	/** Forward Declaration of Renderer
	*	Prevents cyclical dependencies by forward declaring the Renderer class.
	*/
	class Renderer;

	/** Forward Declaration of Renderable
	*	Prevents cyclical dependencies by forward declaring the Renderable class.
	*/
	class Renderable;

	/** Entity class
	 *	An Attributed class that represents an entity in the game world.
	 */
	class Entity : public Attributed
	{

		/** Entity RTTI_DECLARATIONS Macro Call
		 *	Declares virtual RTTI methods that Entity defines.
		 */
		RTTI_DECLARATIONS(Entity, Attributed)

	public:

		/** Entity Constructor
		 *	Constructs an Entity of the given name.
		 *	@param name a constant string reference to name the Entity with, defaulted to "".
		 */
		explicit Entity(const std::string& name = "");

		/** Entity Destructor
		 *	Destroys the Entity.
		 *	Defaulted.
		 */
		virtual ~Entity() = default;

		/** Entity Copy Constructor
		 *	Constructs an Entity as a copy of the given Entity.
		 *	@param otherEntity the constant Entity reference to copy into this Entity.
		 */
		Entity(const Entity& otherEntity);

		/** Entity Assignment Operator
		 *	Copies the contents of the given Entity into this Entity.
		 *	@param otherEntity the constant Entity reference to copy into this Entity.
		 *	@return an Entity reference to this entity after assignment.
		 */
		Entity& operator=(const Entity& otherEntity);

		/** Entity Move Constructor
		 *	Constructs a copy of the given Entity, nullifying the given Entity.
		 *	@param previousEntity the Entity double reference to copy into this Entity.
		 */
		Entity(Entity&& previousEntity);

		/** Entity Move Assignment Operator
		 *	Copies the contents of the given Entity into this Entity, nullifying the given Entity.
		 *	@param previousEntity the Entity double reference to copy into this Entity.
		 *	@return an Entity reference to this Entity after assignment.
		 */
		Entity& operator=(Entity&& previousEntity);

		/** Entity Name
		 *	Returns the name of this Entity.
		 *	@return a constant string reference representing the name of this Entity.
		 */
		const std::string& Name() const;

		/** Entity SetName
		 *	Sets the name of this Entity to the given string.
		 *	@param name a constant string reference to set this Entity instance's name to.
		 */
		void SetName(const std::string& name);

		/** Entity GetSector
		 *	Returns the Sector that contains this Entity.
		 *	@return a Sector pointer pointing to the Sector that contains this Entity.
		 */
		Sector* GetSector() const;

		/** Entity SetSector
		 *	Sets the parent Sector of this Entity to the given Sector.
		 *	@param sector a Sector reference to set this Entity instance's Sector to.
		 */
		void SetSector(Sector& sector);

		/** Entity Actions
		 *	Returns the Datum within this Entity that contains the Action instances owned by this Entity.
		 *	@return a Datum reference containing the Action instances.
		 */
		Datum& Actions();

		/** Entity Actions
		 *	Returns the Datum within this Entity that contains the Action instances owned by this Entity.
		 *	@return a constant Datum reference containing the Action instances.
		 */
		const Datum& Actions() const;

		/** Entity Reactions
		 *	Returns the Datum within this Entity that contains the Reaction instances owned by this Entity.
		 *	@return a Datum reference containing the Reaction instances.
		 */
		Datum& Reactions();

		/** Entity Reactions
		 *	Returns the Datum within this Entity that contains the Reaction instances owned by this Entity.
		 *	@return a constant Datum reference containing the Reaction instances.
		 */
		const Datum& Reactions() const;

		/** Entity CreateAction
		 *	Creates an Action instance given the c++ class name and the instance name.
		 *	@param className a constant string reference representing the name of the c++ class to create.
		 *	@param instanceName a constant string reference representing the name of the Action.
		 *	@return a pointer to the created Action.
		 */
		Action* CreateAction(const std::string& className, const std::string& instanceName);

		Datum& Entities();

		const Datum& Entities() const;

		/** Entity Update
		 *	Updates the components of this Entity and notifies the worldState that this is the current Entity.
		 *	@param worldState a WorldState reference containing the data of the world at this moment in time.
		 */
		virtual void Update(WorldState& worldState);

		/** Entity Render
		*	Renders the Renderable objects of this Entity.
		*	@param renderer The Renderer used to render the Renderable objects.
		*/
		virtual void Render(Renderer* renderer);

		/** Entity Copy
		 *	Creates a copy of this Entity as a Scope pointer.
		 *	@return a Scope pointer constructed as an Entity.
		 */
		virtual Scope* Copy() const override;

		/** Transform properties set methods
		 *	Change position, rotation, or scale.
		 */
		void SetRelativePosition(glm::vec3 position);
		void SetRelativeRotation(glm::vec3 rotation);
		void SetRelativeScale(glm::vec3 scale);

		void SetWorldPosition(glm::vec3 position);
		void SetWorldRotation(glm::vec3 rotation);
		void SetWorldScale(glm::vec3 scale);

		/** Transform properties get methods
		 *	Get position, rotation, or scale.
		 */
		glm::vec3 GetWorldPosition() const;
		glm::vec3 GetWorldRotation() const;
		glm::vec3 GetWorldScale() const;

		glm::vec3 GetRelativePosition() const;
		glm::vec3 GetRelativeRotation() const;
		glm::vec3 GetRelativeScale() const;

		/** Adjust transform properties
		 *	Adjust position, rotation, or scale.
		 */
		void Translate(glm::vec3 delta);
		void Rotate(glm::vec3 delta);
		void Scale(glm::vec3 delta);

		virtual void Initialize(WorldState& worldState);
		virtual void Reset(WorldState& worldState);

	protected:

		/** Entity InitializeSignatures
		 *	Initializes the prescribed attributes for instances of this class.
		 */
		void InitializeSignatures();

		void AddRenderable(Renderable& renderable);

		void RemoveRenderable(Renderable& renderable);

		void RemoveAllRenderables();

		Entity* FindChildEntityByName(const std::string& name);

		/** Entity mRenderables
		*	List of renderable objects for this Entity.
		*/
		Vector<Renderable*> mRenderables;

		glm::vec3 mPosition;
		glm::vec3 mRotation;
		glm::vec3 mScale;

		glm::vec4 mPositionVec4;
		glm::vec4 mRotationVec4;
		glm::vec4 mScaleVec4;

	private:

		/** Entity CopyPrivateDataMembers
		 *	Copies the data members of the given Entity that are not within the Scope hash.
		 *	@param otherEntity a constant Entity reference to copy data members from.
		 */
		void CopyPrivateDataMembers(const Entity& otherEntity);

		/** Entity FixExternalAttributes
		 *	Fixes the external attribute pointers within this Entity after being copied to.
		 */
		void FixExternalAttributes();

		/** Entity mName
		 *	A string representing the name of this Entity.
		 */
		std::string mName;

		/** Entity mSector
		 *	A Sector pointer that caches off the parent of this Entity.
		 */
		Sector* mSector;

	public:

		/** Entity sEntityNameKey
		 *	A string representing the key names are held at within an Entity.
		 */
		static const std::string sEntityNameKey;

		static const std::string sTransformPositionKey;

		static const std::string sTransformRotationKey;

		static const std::string sTransformScaleKey;

		static const std::string sMeshesKey;


	};

	/** ConcreteEntityFactory Macro Definition
	 *	Defines the macro that creates the classes for Entity Factories.
	 */
#define ConcreteEntityFactory(ConcreteProductT) ConcreteFactory(FieaGameEngine::Entity, ConcreteProductT)

	/** ConcreteEntityFactory of Entity Macro Call
	 *	Defines the EntityFactory class that returns Entity pointers from its create call.
	 */
	ConcreteEntityFactory(Entity);
}