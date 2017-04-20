#pragma once
#include <cstdint>
#include <utility>
#include "Vector.h"
#include "SList.h"
#include "DefaultHash.h"

namespace FieaGameEngine
{
	template <typename TKey, typename TValue, typename THash = DefaultHash<TKey>>
	class HashMap
	{

	public:

		/** PairType typedef for HashMap
		 *	Represents a std::pair<TKey, TValue>
		 */
		typedef std::pair<TKey, TValue> PairType;

		/** ChainType typedef for HashMap
		 *	Represents an SList of PairTypes
		 */
		typedef SList<PairType> ChainType;

		/** BucketType typedef for HashMap
		 *	Represents a Vector of ChainTypes
		 */
		typedef Vector<ChainType> BucketType;

		/** Iterator class for HashMap
		 *	Contains methods and data members used to optimize traversing the HashMap
		 */
		class Iterator
		{
			friend class HashMap;

		public:

			/** Default constructor for Iterator
			 *	Initializes default values on members
			 */
			Iterator();

			/** Copy constructor for Iterator
			 *	Copies the values being pointed at by the passed iterator
			 *	Implementation defaulted
			 *	@param otherIt the other Iterator to be copied.
			 */
			Iterator(const Iterator& otherIt) = default;

			/** Destructor for Iterator
			 *	Nulls out pointers on the Iterator before memory is freed
			 *	Implementation defaulted
			 */
			~Iterator() = default;

			/** Post-increment operator for Iterator
			 *	Points the ChainType::Iterator member to the next valid PairType
			 *	Throws exception when owner is not set
			 *	@return an Iterator that represents the state before incrementing this Iterator.
			 */
			Iterator operator++(int);

			/** Pre-increment operator for Iterator
			 *	Points the ChainType::Iterator member to the next valud PairType
			 *	Throws exception when owner is not set
			 *	@return an Iterator reference that represents the incremented Iterator.
			 */
			Iterator& operator++();

			/** Dereference operator for Iterator
			 *	Dereferences the Iterator and returns the PairType it points to
			 *	Throws exception when owner is not set or Iterator represents end
			 *	@return the PairType that is being pointed to by this Iterator.
			 */
			PairType& operator*();

			/** Dereference operator for Iterator
			 *	Dereferences the Iterator and returns the PairType it points to
			 *	Throws exception when owner is not set or Iterator represents end
			 *	@return a constant reference to the PairType that is being pointed to by this Iterator.
			 */
			const PairType& operator*() const;

			/** Arrow operator for Iterator
			 *	Dereferences the Iterator and returns a pointer to the associated PairType
			 *	Throws exception when owner is not set or Iterator represents end
			 *	@return a pointer to the associated PairType.
			 */
			PairType* operator->();

			/** Arrow operator for Iterator
			 *	Dereferences the Iterator and returns a pointer to the associated PairType
			 *	Throws exception when owner is not set or Iterator represents end
			 *	@ return a constant pointer to the associated PairType.
			 */
			const PairType* operator->() const;

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
			 *	Assigns the data members of this Iterator to those of the one passed in
			 *	@param otherIt the Iterator to copy members from.
			 *	@return this Iterator with the passed iterators members copied.
			 */
			Iterator& operator=(const Iterator& otherIt) = default;

		private:

			/** Private constructor for Iterator
			 *	Constructs an Iterator with specific data members
			 *	@param owner the constant HashMap reference that owns this Iterator.
			 *	@param index the vector index this iterator would be contained in.
			 *	@param position the ChainType::Iterator that represents where exactly this Iterator exists in a chain.
			 *	@return an Iterator constructed with data initialized to these values.
			 */
			Iterator(const HashMap& owner, std::uint32_t index, typename ChainType::Iterator position);

			/** HashMap pointer mOwner for Iterator
			 *	Represents the HashMap owner this Iterator belongs to
			 */
			const HashMap* mOwner;

			/** std::uint32_t mChainIndex for Iterator
			 *	Represents which chain in the Iterator's owner's Vector the Iterator should associate with
			 */
			std::uint32_t mChainIndex;
			
			/** ChainType::Iterator mPositionInChain for Iterator
			 *	Represents the exact position in the owner's Vector the Iterator should associate with
			 */
			typename ChainType::Iterator mPositionInChain;
		};

		/** Constructor for HashMap
		 *	Constructs a HashMap with the given bucket count, or a default size
		 *	@param bucketCount the unsigned amount of buckets to create the HashMap with.
		 */
		explicit HashMap(std::uint32_t bucketCount = 13);

		HashMap(std::initializer_list<PairType> list);

		/** Copy constructor for HashMap
		 *	Constructs a HashMap by copying the data members of the passed in HashMap
		 *	Implementation defaulted
		 *	@param otherHash the constant HashMap reference to copy data members from.
		 */
		HashMap(const HashMap& otherHash) = default;

		/** Destructor for HashMap
		 *	Clears heap-allocated data members of this HashMap before freeing memory
		 *	Implementation defaulted
		 */
		~HashMap() = default;

		/** Assignment operator for HashMap
		 *	Assigns the data members of this HashMap to the values of the passed HashMap
		 *	Implementation defaulted
		 *	@param otherHash the constant HashMap reference to copy the data members of.
		 *	@return a reference to this HashMap after the given HashMap's data members are copied.
		 */
		HashMap& operator=(const HashMap& otherHash) = default;

		bool operator==(const HashMap& otherHash) const;

		bool operator!=(const HashMap& otherHash) const;

		/** Find for HashMap
		 *	Finds the PairType in the HashMap associated with the given key, returning an Iterator pointing to the PairType
		 *	Returns end() if the key was not found
		 *	@param key the constant TKey reference to be searched for in the HashMap.
		 *	@return an Iterator representing the found PairType, or end().
		 */
		Iterator Find(const TKey& key) const;

		Iterator Find(const TKey& key, std::uint32_t& index) const;

		/** Insert for HashMap
		 *	Inserts a given PairType into the HashMap, returning a reference to the location inserted
		 *	Returns a reference to an existing element if the key was found
		 *	@param pair the constant PairType reference to be inserted into the HashMap.
		 *	@return an Iterator representing the location of the inserted / existing element.
		 */
		Iterator Insert(const PairType& pair);

		Iterator Insert(const PairType& pair, bool& inserted);

		/** Accessor operator for HashMap
		 *	Finds the TValue associated with the given key
		 *	If the given key is not found, returns a defaulted constructed TValue reference that is inserted into the HashMap
		 *	@param key the constant TKey reference to be accessed from the HashMap.
		 *	@return a reference to the TValue associated with the given key.
		 */
		TValue& operator[](const TKey& key);

		/** Accessor operator for HashMap
		 *	Find the TValue associated with the give key
		 *	If the given key is not found, an exception is thrown
		 *	@param key the constant TKey reference to be accessed from the HashMap.
		 *	@return a constant reference to the TValue associated with the given key.
		 */
		const TValue& operator[](const TKey& key) const;

		/** Remove for HashMap
		 *	Removes the PairType mapping to the given key from the HashMap if found
		 *	@param key the constant TKey reference to be removed from the HashMap.
		 */
		void Remove(const TKey& key);

		/** Clear for HashMap
		 *	Clears all entries from the HashMap, reconstructing the buckets for further use
		 */
		void Clear();

		/** Size for HashMap
		 *	Returns the current number of PairType's inserted into the HashMap
		 *	@return the unsigned number of PairType elements in the HashMap.
		 */
		std::uint32_t Size() const;

		/** ContainsKey for HashMap
		 *	Returns whether the HashMap contains a PairType with the given key
		 *	@param key the constant TKey reference to be found in the HashMap.
		 *	@return a boolean representing whether the given key was found in the HashMap.
		 */
		bool ContainsKey(const TKey& key) const;

		/** begin for HashMap
		 *	Returns an Iterator representing the first valid entry in the HashMap
		 *	@return an Iterator pointing to the first valid entry in the HashMap.
		 */
		Iterator begin() const;

		/** end for HashMap
		 *	Returns an Iterator representing the element past the end of the HashMap
		 *	@return an Iterator pointing to the element past the end of the HashMap.
		 */
		Iterator end() const;

		HashMap(HashMap&& previousHashMap);

		HashMap& operator=(HashMap&& previousHashMap);

	private:

		/** BucketType mBucket for HashMap
		 *	The Vector that stores all PairTypes inserted into the HashMap
		 */
		BucketType mBucket;

		/** THash mHashFunctor for HashMap
		 *	The instantiated hash functor for this HashMap to use when hashing given keys
		 */
		THash mHashFunctor;

		/** std::uint32_t mSize for HashMap
		 *	The unsigned number of PairType elements inserted into the HashMap
		 */
		std::uint32_t mSize;
	};
}

#include "HashMap.inl"