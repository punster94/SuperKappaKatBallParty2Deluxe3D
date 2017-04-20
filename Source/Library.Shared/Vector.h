#pragma once
#include <cstdint>
#include "ResizeStrategy.h"

namespace FieaGameEngine
{
	template <typename T>
	class Vector
	{

	public:

		/** Iterator class for Vector
		 *	Contains methods and data members used to optimize traversing the vector
		 */
		class Iterator
		{
			friend class Vector;

		public:

			/** Default constructor for Iterator
			 *	Initializes the pointers contained in this Iterator to null
			 */
			Iterator();

			/** Copy constructor for Iterator
			 *	Copies the values being pointed at by the iterator passed in
			 *	@param otherIt the other iterator to be copied.
			 */
			Iterator(const Iterator& otherIt);

			/** Destructor for Iterator
			 *	Nulls out pointers on the Iterator before the memory is freed
			 */
			~Iterator();

			/** Post-increment operator for Iterator
			 *	Increments the Node of the Iterator after returning the current Iterator
			 *	@return an Iterator that represents the state of this before incrementing the Node.
			 */
			Iterator operator++(int);

			/** Pre-increment operator for Iterator
			 *	Increments the Node of the Iterator before returning the current Iterator
			 *	@return an Iterator that represents the state of this after incrementing the Node.
			 */
			Iterator& operator++();

			/** Dereference operator for Iterator
			 *	Dereferences and returns the value at the pointer mData
			 *	@return a reference to the value being pointed at by the Iterators mData member.
			 */
			T& operator*();

			/** Dereference operator for Iterator
			 *	Dereferences and returns the value at the pointer mData
			 *	@return a constant reference to the value being pointed at by the Iterators mData member.
			 */
			const T& operator*() const;

			/** Equality operator for Iterator
			 *	Returns whether the given Iterator is equivalent to this Iterator
			 *	@param otherIt the Iterator for this to be compared with.
			 *	@return a boolean representing whether the given Iterator is equivalent to this.
			 */
			bool operator==(const Iterator& otherIt) const;

			/** Inequality operator for Iterator
			 *	Returns whether the given Iterator is different from this Iterator
			 *	@param otherIt the Iterator for this to be compared with.
			 *	@return a boolean representing whether the given Iterator is different from this.
			 */
			bool operator!=(const Iterator& otherIt) const;

			/** Assignment operator for Iterator
			 *	Sets the pointer values of this Iterator to the values of the Iterator passed in, then returns a reference to this
			 *	@param otherIt the Iterator for this to be compared with.
			 *	@return a reference to this Iterator once pointer assignments are completed.
			 */
			Iterator& operator=(const Iterator& otherIt);

		private:

			/** Private functional constructor for Iterator
			 *	Constructs a new Iterator given an owner Vector and a pointer to T data for this Iterator to wrap
			 *	@param owner a reference to the Vector that owns this Iterator.
			 *	@param data a pointer to T-typed data that can be referenced through the Iterator.
			 */
			Iterator(const Vector& owner, T* data);

			/** Owner Vector pointer
			 *	Data member that holds a pointer to the owner of this Iterator
			 */
			const Vector* mOwner;

			/** Data T pointer
			 *	Data member that points to the data to be dereferenced when called for on the Iterator
			 */
			T* mData;
		};

		/** Default constructor for Vector
		 *	Constructs a Vector with an array of default capacity and zero size
		 */
		Vector(std::uint32_t initialCapacity = defaultCapacity);

		Vector(std::initializer_list<std::string> list);

		/** Copy constructor for Vector
		 *	Deep-copies the data in the given Vector
		 *	@param otherVector the Vector to be copied into this Vector.
		 */
		Vector(const Vector& otherVector);

		/** Destructor for Vector
		 *	Clears the data in this Vector and frees the dynamically allocated buffer
		 */
		~Vector();

		/** Accessor operator for Vector
		 *	Returns the T-typed data the given index from the Vector if it is within range
		 *	@param index the index to access data from.
		 *	@return T-typed reference to the data at the given index.
		 */
		T& operator[](std::uint32_t index);

		/** Accessor operator for Vector
		 *	Returns the constant T-typed data the given index from the Vector if it is within range
		 *	@param index the index to access data from.
		 *	@return constant T-typed reference to the data at the given index.
		 */
		const T& operator[](std::uint32_t index) const;

		/** PopBack for Vector
		 *	Removes the last item from the Vector, reducing it's size by one
		 */
		void PopBack();

		/** IsEmpty for Vector
		 *	Returns whether the Vector has a size of zero 
		 *	@return a boolean 
		 */
		bool IsEmpty() const;

		/** Front accessor for Vector
		 *	Returns the T-typed data at the front of the Vector
		 *	@return T-typed reference to the data at the front of the Vector.
		 */
		T& Front();

		/** Front accessor for Vector
		 *	Returns constant T-typed data at the front of the Vector
		 *	@return constant T-typed reference to the data at the front of the Vector.
		 */
		const T& Front() const;

		/** Back accessor for Vector
		 *	Returns the T-typed data at the end of the Vector
		 *	@return T-typed reference to the data at the end of the Vector.
		 */
		T& Back();

		/** Back accessor for Vector
		 *	Returns constant T-typed data at the end of the Vector
		 *	@return constant T-typed reference to the data at the end of the Vector.
		 */
		const T& Back() const;

		/** Size accessor for Vector
		 *	Returns the size of the Vector
		 *	@return unsigned integer representing the number of elements in the Vector.
		 */
		std::uint32_t Size() const;

		/** Capacity accessor for Vector
		 *	Returns the capacity of the Vector
		 *	@return unsigned integer representing the capacity of the Vector.
		 */
		std::uint32_t Capacity() const;

		/** Beginning Iterator for Vector
		 *	Returns an Iterator pointing to the first element in the Vector
		 *	@return an Iterator that points to the first element in the Vector.
		 */
		Iterator begin() const;

		/** End Iterator for Vector
		 *	Returns an Iterator pointing to the element after the last in the Vector
		 *	@return a const Iterator that points to the element after the last in the Vector.
		 */
		const Iterator end() const;

		/** PushBack for Vector
		 *	Inserts the given data into the Vector at the end of the Vector
		 *	@param value a constant reference to the T-typed data to insert.
		 *	@return an Iterator representing the element inserted into the Vector.
		 */
		Iterator PushBack(const T& value);

		/** Assignment operator for Vector
		 *	Deep-copies a given Vector after clearing the contents of this Vector
		 *	@param otherVector a reference to the Vector to be deep-copied.
		 *	@return a reference to this Vector after deep-copying the given Vector.
		 */
		Vector& operator=(const Vector& otherVector);

		/** At for Vector
		 *	Returns the T-typed data at the given index
		 *	@param index an unsigned integer representing the index to access data from.
		 *	@return a reference to the T-typed data at the given index in this Vector.
		 */
		T& At(std::uint32_t index);

		/** At for Vector
		 *	Returns the constant T-typed data at the given index
		 *	@param index an unsigned integer representing the index to access data from.
		 *	@return a constant reference to the T-typed data at the given index in this Vector.
		 */
		const T& At(std::uint32_t index) const;

		/** ShrinkToFit for Vector
		 *	Reduces the capacity of the Vector to the current size of the Vector
		 */
		void ShrinkToFit();

		/** Reserve for Vector
		 *	Increases the capacity of this Vector by the given amount, reallocating the Vectors contiguous memory
		 *	Only reallocates and resizes if the given amount is greater than the current capacity
		 *	@param amount the number of elements the new capacity this Vector should have.
		 */
		void Reserve(std::uint32_t amount);

		/** Find for Vector
		 *	Returns an Iterator representing the position in the Vector the given value can be found
		 *	Returns the end of the Vector if the value is not found in the Vector
		 *	@param value a constant reference to the T-typed value to find in the Vector.
		 *	@return an Iterator representing the position the value exists at in the Vector.
		 */
		Iterator Find(const T& value) const;

		/** Clear for Vector
		 *	Destructs all elements in the Vector, reducing the size to zero
		 */
		void Clear();

		/** Remove value for Vector
		 *	Removes the given value from the vector if it is found
		 *	Adjusts the contents to remain contiguous if a value is removed
		 *	@param value a constant reference to the T-typed value to be removed from the Vector.
		 */
		void Remove(const T& value);

		/** Remove range for Vector
		 *	Removes the range represented by the two Iterators passed in, if the range is valid
		 *	Adjusts the contents to remain contiguous if values are removed
		 *	@param first a reference to the beginning Iterator of the range to be removed.
		 *	@param last a reference to the ending Iterator of the range to be removed.
		 */
		void Remove(Iterator first, Iterator last);

		/** SetResizeStrategy for Vector
		 *	Sets the current resize strategy to the constant reference passed in
		 *	@param strategy a constant pointer to set the Vector's strategy to.
		 */
		void SetResizeStrategy(const ResizeStrategy* strategy);

		/** Default capacity of Vector
		 *	Stores the capacity that default-constructed Vectors will be initialized with
		 */
		static const std::uint32_t defaultCapacity = 3;

		Vector(Vector&& previousVector);

		Vector& operator=(Vector&& previousVector);

	private:

		/** Deep copy helper function for Vector
		 *	Copies the given Vector's contents into this vector
		 *	@param otherVector a constant reference to the Vector that will be copied into this Vector.
		 */
		void DeepCopy(const Vector& otherVector);

		/** Size data member for Vector
		 *	Contains the current unsigned size of the Vector
		 */
		std::uint32_t mSize;

		/** Capacity data member for Vector
		 *	Contains the current unsigned capacity of the Vector
		 */
		std::uint32_t mCapacity;

		/** Array data member for Vector
		 *	Points to the beginning of the array where the Vector stores it's data contiguously
		 */
		T* mArray;

		/** Pointer to ResizeStrategy for Vector
		 *	Points to the current ResizeStrategy the Vector will use when auto-expanding
		 */
		ResizeStrategy* mResizeStrategy;
	};
}

#include "Vector.inl"