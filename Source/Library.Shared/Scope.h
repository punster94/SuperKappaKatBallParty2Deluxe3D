#pragma once

#include "Datum.h"
#include "RTTI.h"
#include "Factory.h"

namespace FieaGameEngine
{
	/** Scope class
	 *	An RTTI class composed of a HashMap of std::string-Datum pairs.
	 */
	class Scope : public RTTI
	{

		/** RTTI method declarations macro
		 *	Declares the methods inherited by RTTI.
		 */
		RTTI_DECLARATIONS(Scope, RTTI);

	protected:

		/** PairType typedef for Scope
		 *	Represents a std::pair<std::string, Datum>
		 */
		typedef std::pair<std::string, Datum> PairType;

		/** Iterator typedef for Scope
		 *	Represents a HashMap<std::string, Datum> Iterator
		 */
		typedef HashMap<std::string, Datum>::Iterator Iterator;

	public:

		/** Scope Default Constructor
		 *	Sets the new Scope's size to a passed unsigned integer.
		 *	Initializes the created Scope's parent to nullptr.
		 *	@param initialSize the unsigned integer size to initialize the structure to.
		 */
		explicit Scope(std::uint32_t initialSize = 13);

		/** Scope Destructor
		 *	Orphans the scope from its parent before clearing any memory the scope is responsible for.
		 */
		virtual ~Scope();

		/** Scope Copy Constructor
		 *	Copies the given Scope into this Scope, allocating any Scopes the given Scope may contain.
		 *	@param otherScope the constant scope reference to copy from.
		 */
		Scope(const Scope& otherScope);

		/** Scope Assignment Operator
		 *	Copies the given Scope into this Scope after clearing the contents of this Scope.
		 *	Any Scope's this Scope was responsible for will be deleted.
		 *	Does not modify the parent of this Scope.
		 *	@param otherScope the constant Scope reference to assign this Scope to.
		 *	@return a reference to this Scope after assignment.
		 */
		Scope& operator=(const Scope& otherScope);

		/** Scope Find
		 *	Finds the Datum within this Scope at the given key.
		 *	Returns nullptr if no Datum is found with the given key.
		 *	@param key the constant std::string reference to find on this Scope.
		 *	@return a pointer to the Datum at this key, or nullptr.
		 */
		Datum* Find(const std::string& key);

		/** Scope Find
		 *	Finds the Datum within this Scope at the given key.
		 *	Returns nullptr if no Datum is found with the given key.
		 *	@param key the constant std::string reference to find on this Scope.
		 *	@return a constant pointer to the Datum at this key, or nullptr.
		 */
		const Datum* Find(const std::string& key) const;

		/** Scope Search
		 *	Searches for the Datum corresponding to the given key.
		 *	If the Datum is not found, this Scope's parent will be searched.
		 *	Returns nullptr if no Datum is found with the given key in the Scope or its lineage.
		 *	Does not modify the parent of this Scope.
		 *	@param key the constant std::string reference to search for.
		 *	@param containingScope a pointer to a Scope pointer that will be used to store the Scope the key is found in.
		 *	@return a pointer to the Datum of the corresponding key, or nullptr.
		 */
		Datum* Search(const std::string& key, Scope** containingScope = nullptr);

		/** Scope Search
		 *	Searches for the Datum corresponding to the given key.
		 *	If the Datum is not found, this Scope's parent will be searched.
		 *	Returns nullptr if no Datum is found with the given key in the Scope or its lineage.
		 *	Does not modify the parent of this Scope.
		 *	@param key the constant std::string reference to search for.
		 *	@param containingScope a pointer to a Scope pointer that will be used to store the Scope the key is found in.
		 *	@return a constant pointer to the Datum of the corresponding key, or nullptr.
		 */
		const Datum* Search(const std::string& key, Scope** containingScope = nullptr) const;

		/** Scope Append
		 *	Appends a new Datum with the given key, returning a reference to it.
		 *	If the key exists in the Scope, returns a reference to the existing Datum.
		 *	@param key the constant std::string reference key of the new or existing Datum.
		 *	@return a reference to the Datum of the corresponding key.
		 */
		Datum& Append(const std::string& key);

		/** Scope AppendScope
		 *	Appends a new Scope in this Scope with the given key, returning a reference to it.
		 *	If the key does not exist, will append a new Datum to the Scope to contain the created Scope.
		 *	@exception will fail if the given key exists for a populated Datum whose type is non-Table.
		 *	@param key the constant std::string reference key corresponding to the Datum containing the new Scope.
		 *	@return a reference to the new Scope.
		 */
		Scope& AppendScope(const std::string& key);

		/** Scope Adopt
		 *	Adopts the given Scope as a child at the given key.
		 *	The child will change its parent to this Scope after orphaning itself from its current parent Scope.
		 *	@exception will fail if this Scope attempts to adopt itself.
		 *	@exception will fail if a populated, non-Table Datum exists in this Scope at the given key.
		 *	@param child a reference to the Scope to adopt.
		 *	@param key the constant std::string reference key corresponding to the Datum containing the new Scope.
		 */
		void Adopt(Scope& child, const std::string& key);


		/** Scope GetParent
		 *	Returns this Scope's current parent.
		 *	@return a Scope pointer representing this Scope's parent.
		 */
		Scope* GetParent();

		/** Scope GetParent
		 *	Returns this Scope's current parent.
		 *	@return a constant Scope pointer representing this Scope's parent.
		 */
		const Scope* GetParent() const;

		/** Scope Key Accessor
		 *	Returns the Datum corresponding to the given key.
		 *	@param key a constant std::string reference representing the key to access.
		 *	@return a Datum reference corresponding to the given key.
		 */
		Datum& operator[](const std::string& key);

		/** Scope Key Accessor
		 *	Returns the Datum corresponding to the given key.
		 *	@exception will fail if the given key does not exist in the Scope.
		 *	@param key a constant std::string reference representing the key to access.
		 *	@return a constant Datum reference corresponding to the given key.
		 */
		const Datum& operator[](const std::string& key) const;

		/** Scope Order Accessor
		 *	Returns the Datum corresponding to the given index.
		 *	@param key a constant unsigned integer representing the index to access.
		 *	@return a Datum reference corresponding to the given index.
		 */
		Datum& operator[](const std::uint32_t index);

		/** Scope Order Accessor
		 *	Returns the Datum corresponding to the given index.
		 *	@exception will fail if the given index is out of bounds.
		 *	@param key a constant unsigned integer representing the index to access.
		 *	@return a constant Datum reference corresponding to the given index.
		 */
		const Datum& operator[](const std::uint32_t index) const;

		/** Scope Equality Operator
		 *	Returns whether the given Scope is equivalent to this Scope.
		 *	@param otherScope a constant reference to the Scope to compare with.
		 *	@return a boolean representing whether the given Scope is equivalent to this Scope.
		 */
		bool operator==(const Scope& otherScope) const;

		/** Scope Inequality Operator
		 *	Returns whether the given Scope is not equivalent to this Scope.
		 *	@param otherScope a constant reference to the Scope to compare with.
		 *	@return a boolean representing whether the given Scope is not equivalent to this Scope.
		 */
		bool operator!=(const Scope& otherScope) const;

		/** Scope Equals override
		 *	Returns whether the given RTTI pointer is equivalent to this Scope.
		 *	@param otherScope a constant RTTI pointer to compare with.
		 *	@return a boolean representing whether the given RTTI is equivalent to this Scope.
		 */
		bool Equals(const RTTI* otherScope) const override;

		/** Scope Clear
		 *	Clears this Scope by deleting any contained Scopes or copied data.
		 */
		void Clear();

		/** Scope Orphan
		 *	Removes this Scope from its parent's references, nulling this Scope's parent pointer.
		 */
		void Orphan();

		void Orphan(std::string& selfKey);

		Scope(Scope&& previousScope);

		Scope& operator=(Scope&& previousScope);

		Datum* FindContainedScope(Scope& scopeToFind, std::uint32_t& index) const;

		/** Scope Copy
		 *	Creates a copy of this Scope as a Scope pointer.
		 *	@return a Scope pointer constructed as a Scope.
		 */
		virtual Scope* Copy() const;

	protected:

		/** Scope CopyEntryTo
		 *	Copies the entry in the scope at the given index to the new scope.
		 *	@param index a constant unsigned integer to use as an index into this scope.
		 *	@param otherScope a Scope reference to copy the entry into.
		 */
		void CopyEntryTo(const std::uint32_t index, Scope& otherScope) const;

		/** HashMap<std::string, Datum> mMap for Scope
		 *	Stores the Datum contained by this Scope by mapping them to their names.
		 */
		HashMap<std::string, Datum> mMap;

	private:

		void Reparent(Scope& previousScope);

		/** Scope* mParent for Scope
		 *	Stores a pointer to this Scope's current parent.
		 */
		Scope* mParent;

		/** Vector<PairType*> mOrderVector for Scope
		 *	Stores pointers to the pairs in this Scope's map in  the order they were added.
		 */
		Vector<PairType*> mOrderVector;
	};
}