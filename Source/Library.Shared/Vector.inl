namespace FieaGameEngine
{
	/**
	 *	Vector<T> Implementation
	 */

	template <typename T>
	Vector<T>::Vector(std::uint32_t initialCapacity) :
		mArray(nullptr), mResizeStrategy(nullptr), mSize(0), mCapacity(0)
	{
		mResizeStrategy = new ResizeStrategy();

		Reserve(initialCapacity);
	}

	template <typename T>
	Vector<T>::Vector(std::initializer_list<std::string> list) :
		Vector(list.size())
	{
		for (std::string& pair : list)
		{
			Insert(pair);
		}
	}

	template <typename T>
	Vector<T>::Vector(const Vector& otherVector) :
		mArray(nullptr), mResizeStrategy(nullptr), mSize(otherVector.mSize), mCapacity(otherVector.mCapacity)
	{
		DeepCopy(otherVector);
	}

	template <typename T>
	Vector<T>::~Vector()
	{
		Clear();
		
		free(mArray);
		delete(mResizeStrategy);
	}

	template <typename T>
	T& Vector<T>::operator[](std::uint32_t index)
	{
		return const_cast<T&> (const_cast<const Vector*> (this)->operator[](index));
	}

	template <typename T>
	const T& Vector<T>::operator[](std::uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::exception("Index out of bounds.");
		}

		return mArray[index];
	}

	template <typename T>
	void Vector<T>::PopBack()
	{
		if (!IsEmpty())
		{
			mArray[--mSize].~T();
		}
	}

	template <typename T>
	bool Vector<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template <typename T>
	T& Vector<T>::Front()
	{
		return const_cast<T&> (const_cast<const Vector*> (this)->Front());
	}

	template <typename T>
	const T& Vector<T>::Front() const
	{
		if (IsEmpty())
		{
			throw std::exception("Empty vector.");
		}

		return *mArray;
	}

	template <typename T>
	T& Vector<T>::Back()
	{
		return const_cast<T&> (const_cast<const Vector*> (this)->Back());
	}

	template <typename T>
	const T& Vector<T>::Back() const
	{
		if (IsEmpty())
		{
			throw std::exception("Empty vector.");
		}

		return mArray[mSize - 1];
	}

	template <typename T>
	std::uint32_t Vector<T>::Size() const
	{
		return mSize;
	}

	template <typename T>
	std::uint32_t Vector<T>::Capacity() const
	{
		return mCapacity;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::begin() const
	{
		return Iterator(*this, mArray);
	}

	template <typename T>
	typename const Vector<T>::Iterator Vector<T>::end() const
	{
		return Iterator(*this, &mArray[mSize]);
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::PushBack(const T& value)
	{
		if (mSize == mCapacity)
		{
			std::uint32_t sizeToIncrement = mResizeStrategy->operator()(mSize, mCapacity);

			if (sizeToIncrement <= 0)
			{
				throw std::exception("Incompatible resizing strategy.");
			}

			Reserve(mCapacity + sizeToIncrement);
		}
		
		new (&mArray[mSize]) T(value);

		return Iterator(*this, &mArray[mSize++]);
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(const Vector& otherVector)
	{
		if (this != &otherVector)
		{
			Clear();

			free(mArray);
			delete(mResizeStrategy);

			mSize = otherVector.mSize;
			mCapacity = otherVector.mCapacity;

			DeepCopy(otherVector);
		}

		return *this;
	}

	template <typename T>
	T& Vector<T>::At(std::uint32_t index)
	{
		return (*this)[index];
	}

	template <typename T>
	const T& Vector<T>::At(std::uint32_t index) const
	{
		return (*this)[index];
	}

	template <typename T>
	void Vector<T>::ShrinkToFit()
	{
		if (mSize != mCapacity)
		{
			mCapacity = 0;
			Reserve(mSize);
		}
	}

	template <typename T>
	void Vector<T>::Reserve(std::uint32_t amount)
	{
		if (amount > mCapacity)
		{
			mCapacity = amount;

			mArray = static_cast<T*>(realloc(mArray, mCapacity * sizeof(T)));
		}
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Find(const T& value) const
	{
		Iterator it;

		for (it = begin(); it != end(); ++it)
		{
			if (*it == value)
			{
				break;
			}
		}

		return it;
	}

	template <typename T>
	void Vector<T>::Clear()
	{
		for (T& value : *this)
		{
			value.~T();
		}

		mSize = 0;
	}

	template <typename T>
	void Vector<T>::Remove(const T& value)
	{
		if (!IsEmpty())
		{
			Iterator it = Find(value);

			if (it != end())
			{
				Iterator workIt = it;

				while (++workIt != end())
				{
					*it.mData = *workIt;

					++it;
				}

				mArray[--mSize].~T();
			}
		}
	}

	template <typename T>
	void Vector<T>::Remove(Iterator first, Iterator last)
	{
		if (first.mData <= last.mData && first.mOwner == this && last.mOwner == this && !IsEmpty())
		{
			Iterator workBackIt;
			std::uint32_t sizeReduction = 0;

			for (workBackIt = last++; first != last; ++first)
			{
				if (++workBackIt != end())
				{
					*first = *workBackIt;

					(*workBackIt).~T();
				}
				else
				{
					(*first).~T();
				}

				++sizeReduction;
			}

			while (++workBackIt != end())
			{
				new ((last++).mData) T(*workBackIt);

				(*workBackIt).~T();
			}

			mSize -= sizeReduction;
		}
	}

	template <typename T>
	void Vector<T>::SetResizeStrategy(const ResizeStrategy* resizeStrategy)
	{
		delete(mResizeStrategy);

		mResizeStrategy = resizeStrategy->clone();
	}

	template <typename T>
	Vector<T>::Vector(Vector&& previousVector) :
		mSize(previousVector.mSize), mCapacity(previousVector.mCapacity),
		mArray(previousVector.mArray), mResizeStrategy(previousVector.mResizeStrategy)
	{
		previousVector.mSize = 0;
		previousVector.mCapacity = 0;
		previousVector.mArray = nullptr;
		previousVector.mResizeStrategy = nullptr;
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(Vector&& previousVector)
	{
		if (this != &previousVector)
		{
			Clear();

			delete(mResizeStrategy);
			free(mArray);

			mSize = previousVector.mSize;
			mCapacity = previousVector.mCapacity;
			mArray = previousVector.mArray;
			mResizeStrategy = previousVector.mResizeStrategy;

			previousVector.mSize = 0;
			previousVector.mCapacity = 0;
			previousVector.mArray = nullptr;
			previousVector.mResizeStrategy = nullptr;
		}

		return *this;
	}

	template <typename T>
	void Vector<T>::DeepCopy(const Vector& otherVector)
	{
		mArray = static_cast<T*>(malloc(mCapacity * sizeof(T)));
		T* workPointer = mArray;

		for (T value : otherVector)
		{
			new (workPointer++) T(value);
		}

		mResizeStrategy = otherVector.mResizeStrategy->clone();
	}

	/**
	 *	Vector<T>::Iterator Implementation
	 */

	template <typename T>
	Vector<T>::Iterator::Iterator() :
		mOwner(nullptr), mData(nullptr)
	{

	}

	template <typename T>
	Vector<T>::Iterator::Iterator(const Iterator& otherIt) :
		mOwner(otherIt.mOwner), mData(otherIt.mData)
	{

	}

	template <typename T>
	Vector<T>::Iterator::Iterator(const Vector& owner, T* data) :
		mOwner(&owner), mData(data)
	{

	}

	template <typename T>
	Vector<T>::Iterator::~Iterator()
	{
		mOwner = nullptr;
		mData = nullptr;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator it(*this);
		++(*this);

		return it;
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Invalid owner.");
		}

		if (*this != mOwner->end())
		{
			++mData;
		}

		return *this;
	}

	template <typename T>
	T& Vector<T>::Iterator::operator*()
	{
		return const_cast<T&> (const_cast<const Iterator*> (this)->operator*());
	}

	template <typename T>
	const T& Vector<T>::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("No owner.");
		}

		if (*this == mOwner->end())
		{
			throw std::exception("End of vector reached.");
		}

		return *mData;
	}

	template <typename T>
	bool Vector<T>::Iterator::operator==(const Iterator& otherIt) const
	{
		if (mOwner == otherIt.mOwner)
		{
			return mData == otherIt.mData;
		}

		return false;
	}

	template <typename T>
	bool Vector<T>::Iterator::operator!=(const Iterator& otherIt) const
	{
		return !(*this == otherIt);
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator=(const Iterator& otherIt)
	{
		if (*this != otherIt)
		{
			mOwner = otherIt.mOwner;
			mData = otherIt.mData;
		}

		return *this;
	}
}