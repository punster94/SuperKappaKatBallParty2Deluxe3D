#pragma once
#include <cstdint>

namespace FieaGameEngine
{
	/** Singly-linked list class
	 *	Encapsulates a singly-linked list of template-typed data nodes.
	 *  Exposes several methods for accessing data without knowledge of the list implementation.
	 */
	template <typename T>
	class SList
	{
	private:

		/** Node structure
		 *	Contains a template-typed data value and a next Node pointer.
		 */
		struct Node
		{
		public:

			/** Node Constructor
			 *	Takes in a reference to data and a pointer to the node after this one
			 *	@param data constant reference to the T-typed data to be copied into the node.
			 *	@param next a Node pointer that points to the node after the one being constructed.
			 */
			Node(const T& data, Node* next = nullptr);

			/** Template-typed data member
			 *	Contains the data in a node of the type the SList was created with.
			 */
			T Data;

			/** Node pointer member next
			 *	Contains a pointer to the node after this node in the list.
			 *  Points to nullptr if no nodes after this one.
			 */
			Node* Next;
		};

		/** Helper function that deep-copies a list
		 *	Traverses a given SList and copies the contents of its nodes into this.
		 *	@param list a reference to an existing SList
		 */
		void DeepCopy(const SList& list);

	public:

		/** Iterator class for SList
		*	Contains methods and data members used to optimize traversing the list
		*/
		class Iterator
		{
			friend class SList;

		public:

			/** Default constructor for Iterator
			 *	Constructs a new Iterator with its pointers set to nullptr
			 */
			Iterator();

			/** Functional constructor that takes an SList reference and a Node pointer
			 *	Constructs a new Iterator with its owner and node set to the given values
			 *	@param owner the SList owner of the Iterator to be constructed
			 *	@param node the node the Iterator should currently be on
			 */
			Iterator(const SList& owner, Node* node);

			/** Destructor for Iterator
			 *	Nulls out pointers on the Iterator before the memory is freed
			 */
			~Iterator();

			/** Post-increment operator for Iterator
			 *	Increments the Node of the Iterator after returning the current Iterator
			 *	@return an Iterator that represents the state of this before incrementing the Node
			 */
			Iterator operator++(int);

			/** Pre-increment operator for Iterator
			 *	Increments the Node of the Iterator before returning the current Iterator
			 *	@return an Iterator that represents the state of this after incrementing the Node
			 */
			Iterator& operator++();

			/** Dereference operator for Iterator
			 *	Returns the data in the Node currently in this Iterator
			 *	@return the T-typed data in the current Node of this Iterator.
			 */
			T& operator*();

			/** Dereference operator for Iterator
			 *	Returns the constant data in the Node currently in this Iterator
			 *	@return the constant T-typed data in the current Node of this Iterator.
			 */
			const T& operator*() const;

			/** Equality operator
			 *	Returns true if the passed Iterator is equivalent to this Iterator
			 *	@return whether the passed Iterator is equivalent to this Iterator
			 */
			bool operator==(const Iterator& otherIt) const;

			/** Inequality operator
			 *	Returns true if the passed Iterator is not equivalent to this Iterator
			 *	@return whether the passed Iterator is not equivalent to this Iterator.
			 */
			bool operator!=(const Iterator& otherIt) const;

			/** Assignment operator
			 *	Returns this Iterator after assigning members of the passed iterator to this
			 *	@param otherIt a constant reference to the Iterator to be copied.
			 *	@return a reference to this iterator after assignment is completed.
			 */
			Iterator& operator=(const Iterator& otherIt);

		private:

			/** Current Node pointer
			*	Holds the current Node represented by this Iterator
			*/
			Node* mNode;

			/** Owner SList pointer
			*	Holds a pointer to the SList that owns this Iterator
			*/
			const SList* mOwner;
		};

		/** Default constructor
		 *	Constructs a new SList and sets the mFront and mBack Node pointers to nullptr.
		 *	Sets the mSize of the list to 0.
		 */
		SList();

		/** Copy constructor
		 *	Constructs a new SList by taking in an existing SList and deep-copying it if the passed list is not empty.
		 *	@param list a reference to an existing SList
		 */
		SList(const SList& list);

		/** Assignment operator
		 *	Sets the contents of this to the passed in SList by clearing this and deep-copying if the passed list is not this.
		 *	@param list a reference to an existing SList
		 */
		SList& operator=(const SList& list);

		/** Destructor
		 *	Prevents memory leaks by clearing the allocated nodes contained in the list.
		 */
		~SList();

		/** Method to push new data to the front of the list
		 *	Takes the given template-typed data and puts it at the front of the list.
		 *	Assigns the front node's next pointer to the previous front node.
		 *	@param data a reference to template-typed data to be added to the list.
		 *	@return an Iterator representing the element inserted into the list.
		 */
		Iterator PushFront(const T& data);

		/** Method to remove the front node of the list
		 *	Deletes the front node of the list.
		 *	Sets the front node's next pointer to the previous front node.
		 */
		void PopFront();

		/** Method to push new data to the back of the list
		 *	Takes the given template-typed data and puts it at the back of the list.
		 *	Assigns the previous back node's next pointer to the new node.
		 *	Assigns the new node to be the current back node.
		 *	@param data a reference to template-typed data to be added to the list
		 *	@return an Iterator representing the element inserted into the list.
		 */
		Iterator PushBack(const T& data);

		/** Method that deletes all nodes in the list
		 *	Deletes all nodes in the list by removing the front node until it is considered empty.
		 *	Sets the front and back pointers to nullptr.
		 */
		void Clear();

		/** Method that returns whether or not the list is empty
		 *	Returns true if the size of the list is non-zero.
		 *	@return Whether the list is empty or not
		 */
		bool IsEmpty() const;

		/** Accessor for the size data member
		 *	Returns the size data member of the SList class.
		 *	@return The current unsigned size of the list
		 */
		std::uint32_t Size() const;

		/** Method that returns the data at the front of the list
		 *	Returns the data pointed to by the front node pointer.
		 *	Throws an exception if the list is considered empty.
		 *	@return The template-typed data at the front of the list
		 */
		T& Front();

		/** Method that returns a constant reference to the data at the front of the list
		 *	Returns the data pointed to by the front node pointer.
		 *	Throws an exception if the list is considered empty.
		 *	@return The template-typed data at the front of the list
		 */
		const T& Front() const;

		/** Method that returns the data at the back of the list
		 *	Returns the data pointed to by the back node pointer.
		 *	Throws an exception if the list is considered empty.
		 *	@return The template-typed data at the back of the list
		 */
		T& Back();

		/** Method that returns a constant reference to the data at the back of the list
		 *	Returns the data pointed to by the back node pointer.
		 *	Throws an exception if the list is considered empty.
		 *	@return the template-typed data at the back of the list
		 */
		const T& Back() const;

		/** Method that returns an Iterator representing the beginning of the list
		 *	Returns an Iterator that contains a pointer to the first node in this SList
		 *	@return an Iterator containing a pointer to this SList and a pointer to the front Node.
		 */
		Iterator begin() const;

		/** Method that returns an Iterator representing the node past the end of the list
		*	Returns an Iterator that contains a nullptr
		*	@return an Iterator containing a pointer to this SList and a nullptr.
		*/
		Iterator end() const;

		/** Method that inserts the given T-typed data at a specified position
		 *	Inserts a new Node in the SList containing the given data at the position represented by the given Iterator reference
		 *	@param data the T-typed data to be inserted into the SList.
		 *	@param it the Iterator representing the position in the SList to place the data.
		 *	@return an Iterator representing the element inserted into the list.
		 */
		Iterator InsertAfter(const T& data, const Iterator& it);

		/** Method that finds a given T-typed data in the SList
		 *	Returns an Iterator that holds the Node containing the specified data or returns end() if not found
		 *	@param value the T-typed value to be searched for over the SList
		 *	@return an Iterator that represents the Node containing the specified data or end().
		 */
		Iterator Find(const T& value);

		/** Method that removes a given value from the SList if found
		 *	Deletes the node containing the given value if it is found in the list, properly maintaining the list's mFront, mBack and mSize
		 *	@param value the T-typed value to be potentially removed from the SList
		 */
		void Remove(const T& value);

		SList(SList&& previousSList);

		SList& operator=(SList&& previousSList);

	private:

		/** Unsigned integer member size
		 *	Contains current the size of the list.
		 */
		std::uint32_t mSize;

		/** Node pointer member front
		 *	Points to the first node in the list.
		 */
		Node* mFront;

		/** Node pointer member back
		 *	Points to the last node in the list.
		 */
		Node* mBack;
	};
}

#include "SList.inl"