namespace FieaGameEngine
{
  #pragma region
	template <typename TKey, typename TValue, typename THash>
	HashMap<TKey, TValue, THash>::HashMap(std::uint32_t bucketCount) :
		mSize(0)
	{
		mBucket.Reserve(bucketCount);

		for (std::uint32_t i = 0; i < bucketCount; ++i)
		{
			mBucket.PushBack(ChainType());
		}
	}

	template <typename TKey, typename TValue, typename THash>
	HashMap<TKey, TValue, THash>::HashMap(std::initializer_list<PairType> list) :
		HashMap()
	{
		for (PairType pair : list)
		{
			Insert(pair);
		}
	}

	template <typename TKey, typename TValue, typename THash>
	bool HashMap<TKey, TValue, THash>::operator==(const HashMap& otherHash) const
	{
		bool equal = Size() == otherHash.Size();

		if (equal)
		{
			for (PairType pair : otherHash)
			{
				Iterator it = Find(pair.first);

				if (it == end() || it->second != pair.second)
				{
					equal = false;
					break;
				}
			}
		}

		return equal;
	}

	template <typename TKey, typename TValue, typename THash>
	bool HashMap<TKey, TValue, THash>::operator!=(const HashMap& otherHash) const
	{
		return !(*this == otherHash);
	}

	template <typename TKey, typename TValue, typename THash>
	typename HashMap<TKey, TValue, THash>::Iterator HashMap<TKey, TValue, THash>::Find(const TKey& key) const
	{
		std::uint32_t index;
		return Find(key, index);
	}

	template <typename TKey, typename TValue, typename THash>
	typename HashMap<TKey, TValue, THash>::Iterator HashMap<TKey, TValue, THash>::Find(const TKey& key, std::uint32_t& index) const
	{
		index = mHashFunctor(key) % mBucket.Size();

		const ChainType& chain = mBucket[index];
		Iterator it = end();

		for (ChainType::Iterator chainIt = chain.begin(); chainIt != chain.end(); ++chainIt)
		{
			if ((*chainIt).first == key)
			{
				it = Iterator(*this, index, chainIt);
				break;
			}
		}

		return it;
	}

	template <typename TKey, typename TValue, typename THash>
	typename HashMap<TKey, TValue, THash>::Iterator HashMap<TKey, TValue, THash>::Insert(const PairType& pair)
	{
		bool inserted = false;
		return Insert(pair, inserted);
	}

	template <typename TKey, typename TValue, typename THash>
	typename HashMap<TKey, TValue, THash>::Iterator HashMap<TKey, TValue, THash>::Insert(const PairType& pair, bool& inserted)
	{
		Iterator it = Find(pair.first);

		inserted = false;

		if (it == end())
		{
			std::uint32_t chainIndex = mHashFunctor(pair.first) % mBucket.Size();
			ChainType::Iterator newPosition = mBucket[chainIndex].PushBack(pair);

			it = Iterator(*this, chainIndex, newPosition);
			inserted = true;

			++mSize;
		}

		return it;
	}

	template <typename TKey, typename TValue, typename THash>
	TValue& HashMap<TKey, TValue, THash>::operator[](const TKey& key)
	{
		return Insert(PairType(key, TValue()))->second;
	}

	template <typename TKey, typename TValue, typename THash>
	const TValue& HashMap<TKey, TValue, THash>::operator[](const TKey& key) const
	{
		Iterator it = Find(key);

		if (it == end())
		{
			throw std::exception("Cannot find given key");
		}

		return (*it).second;
	}

	template <typename TKey, typename TValue, typename THash>
	void HashMap<TKey, TValue, THash>::Remove(const TKey& key)
	{
		Iterator it = Find(key);

		if (it != end())
		{
			mBucket[it.mChainIndex].Remove(*it);

			--mSize;
		}
	}

	template <typename TKey, typename TValue, typename THash>
	void HashMap<TKey, TValue, THash>::Clear()
	{
		for (ChainType& chain : mBucket)
		{
			chain.Clear();
		}

		mSize = 0;
	}

	template <typename TKey, typename TValue, typename THash>
	std::uint32_t HashMap<TKey, TValue, THash>::Size() const
	{
		return mSize;
	}

	template <typename TKey, typename TValue, typename THash>
	bool HashMap<TKey, TValue, THash>::ContainsKey(const TKey& key) const
	{
		return Find(key) != end();
	}

	template <typename TKey, typename TValue, typename THash>
	typename HashMap<TKey, TValue, THash>::Iterator HashMap<TKey, TValue, THash>::begin() const
	{
		Iterator it = end();

		if (Size() > 0)
		{
			std::uint32_t chainIndex = 0;

			while (mBucket[chainIndex].IsEmpty())
			{
				++chainIndex;
			}

			it = Iterator(*this, chainIndex, mBucket[chainIndex].begin());
		}

		return it;
	}

	template <typename TKey, typename TValue, typename THash>
	typename HashMap<TKey, TValue, THash>::Iterator HashMap<TKey, TValue, THash>::end() const
	{
		return Iterator(*this, mBucket.Size(), ChainType::Iterator());
	}

	template <typename TKey, typename TValue, typename THash>
	HashMap<TKey, TValue, THash>::HashMap(HashMap&& previousHashMap)
	{
		*this = std::move(previousHashMap);
	}

	template <typename TKey, typename TValue, typename THash>
	HashMap<TKey, TValue, THash>& HashMap<TKey, TValue, THash>::operator=(HashMap&& previousHashMap)
	{
		if (this != &previousHashMap)
		{
			Clear();

			mSize = previousHashMap.mSize;
			mBucket = std::move(previousHashMap.mBucket);

			previousHashMap.mSize = 0;
		}

		return *this;
	}

  #pragma endregion HashMap<TKey, TValue, THash> Implementation

  #pragma region
	template <typename TKey, typename TValue, typename THash>
	HashMap<TKey, TValue, THash>::Iterator::Iterator() :
		mOwner(nullptr), mChainIndex(0)
	{

	}

	template <typename TKey, typename TValue, typename THash>
	typename HashMap<TKey, TValue, THash>::Iterator HashMap<TKey, TValue, THash>::Iterator::operator++(int)
	{
		Iterator it(*this);
		++(*this);

		return it;
	}

	template <typename TKey, typename TValue, typename THash>
	typename HashMap<TKey, TValue, THash>::Iterator& HashMap<TKey, TValue, THash>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Invalid owner.");
		}

		if (++mPositionInChain == mOwner->mBucket[mChainIndex].end())
		{
			while (++mChainIndex < mOwner->mBucket.Size() && (*mOwner).mBucket[mChainIndex].IsEmpty())
			{

			}

			mPositionInChain = mOwner->end().mPositionInChain;

			if (mChainIndex != mOwner->mBucket.Size())
			{
				mPositionInChain = (*mOwner).mBucket[mChainIndex].begin();
			}
		}

		return *this;
	}

	template <typename TKey, typename TValue, typename THash>
	typename HashMap<TKey, TValue, THash>::PairType& HashMap<TKey, TValue, THash>::Iterator::operator*()
	{
		return const_cast<PairType&> (const_cast<const Iterator*> (this)->operator*());
	}

	template <typename TKey, typename TValue, typename THash>
	const typename HashMap<TKey, TValue, THash>::PairType& HashMap<TKey, TValue, THash>::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Invalid owner");
		}

		if (*this == mOwner->end())
		{
			throw std::exception("Cannot dereference end of HashMap");
		}

		return *mPositionInChain;
	}

	template <typename TKey, typename TValue, typename THash>
	typename HashMap<TKey, TValue, THash>::PairType* HashMap<TKey, TValue, THash>::Iterator::operator->()
	{
		return const_cast<PairType*> (const_cast<const Iterator*> (this)->operator->());
	}

	template <typename TKey, typename TValue, typename THash>
	const typename HashMap<TKey, TValue, THash>::PairType* HashMap<TKey, TValue, THash>::Iterator::operator->() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Invalid owner");
		}

		if (*this == mOwner->end())
		{
			throw std::exception("Cannot dereference end of HashMap");
		}

		return &(*mPositionInChain);
	}

	template <typename TKey, typename TValue, typename THash>
	bool HashMap<TKey, TValue, THash>::Iterator::operator==(const Iterator& otherIt) const
	{
		if (mOwner == otherIt.mOwner && mChainIndex == otherIt.mChainIndex)
		{
			return mPositionInChain == otherIt.mPositionInChain;
		}

		return false;
	}

	template <typename TKey, typename TValue, typename THash>
	bool HashMap<TKey, TValue, THash>::Iterator::operator!=(const Iterator& otherIt) const
	{
		return !(*this == otherIt);
	}

	template <typename TKey, typename TValue, typename THash>
	HashMap<TKey, TValue, THash>::Iterator::Iterator(const HashMap& owner, std::uint32_t index, typename ChainType::Iterator position) :
		mOwner(&owner), mChainIndex(index), mPositionInChain(position)
	{
		
	}
  #pragma endregion HashMap<TKey, TValue, THash>::Iterator Implementation
}