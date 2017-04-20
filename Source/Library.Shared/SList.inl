namespace FieaGameEngine
{
	/**
	*	SList<T>::Iterator Implementation
	*/
	template <typename T>
	SList<T>::Node::Node(const T& data, Node* next = nullptr) :
		Data(data), Next(next)
	{
		
	}

	/**
	 *	SList<T> Implementation
	 */

	template <typename T>
	SList<T>::SList() :
		mSize(0), mFront(nullptr), mBack(nullptr)
	{

	}

	template <typename T>
	void SList<T>::DeepCopy(const SList& list)
	{
		for (T& value : list)
		{
			PushBack(value);
		}
	}

	template <typename T>
	SList<T>::SList(const SList& list) :
		SList()
	{
		if (!list.IsEmpty())
		{
			DeepCopy(list);
		}
	}

	template <typename T>
	SList<T>& SList<T>::operator=(const SList& list)
	{
		if (this != &list)
		{
			Clear();
			DeepCopy(list);
		}

		return *this;
	}

	template <typename T>
	SList<T>::~SList()
	{
		Clear();
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushFront(const T& data)
	{
		SList::Node* front = new SList::Node(data, mFront);

		mFront = front;

		if (IsEmpty())
		{
			mBack = mFront;
		}

		++mSize;

		return Iterator(*this, front);
	}

	template <typename T>
	void SList<T>::PopFront()
	{
		if (!IsEmpty())
		{
			SList::Node* next = mFront->Next;
			delete(mFront);
			mFront = next;

			--mSize;
		}
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& data)
	{
		SList::Node* back = new SList::Node(data);

		if (IsEmpty())
		{
			mBack = back;
			mFront = mBack;
		}
		else
		{
			mBack->Next = back;
			mBack = back;
		}

		++mSize;

		return Iterator(*this, back);
	}

	template <typename T>
	void SList<T>::Clear()
	{
		while (!IsEmpty())
		{
			PopFront();
		}
	}

	template <typename T>
	bool SList<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template <typename T>
	std::uint32_t SList<T>::Size() const
	{
		return mSize;
	}

	template <typename T>
	T& SList<T>::Front()
	{
		return const_cast<T&> (const_cast<const SList*> (this)->Front());
	}

	template <typename T>
	const T& SList<T>::Front() const
	{
		if (IsEmpty())
		{
			throw std::exception("Empty list.");
		}

		return mFront->Data;
	}

	template <typename T>
	T& SList<T>::Back()
	{
		return const_cast<T&> (const_cast<const SList*> (this)->Back());
	}

	template <typename T>
	const T& SList<T>::Back() const
	{
		if (IsEmpty())
		{
			throw std::exception("Empty list.");
		}

		return mBack->Data;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::begin() const
	{
		return Iterator(*this, mFront);
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::end() const
	{
		return Iterator(*this, nullptr);
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::InsertAfter(const T& data, typename const SList<T>::Iterator& it)
	{
		if (this != it.mOwner)
		{
			throw std::exception("Iterator not owned by this SList.");
		}

		Iterator result;

		if (it != end())
		{
			SList::Node* newNode = new SList::Node(data, it.mNode->Next);

			it.mNode->Next = newNode;

			if (it.mNode == mBack)
			{
				mBack = newNode;
			}

			++mSize;

			result = Iterator(*this, newNode);
		}
		else
		{
			result = PushBack(data);
		}

		return result;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Find(const T& value)
	{
		SList<T>::Iterator it = begin();

		while (it != end() && *it != value)
		{
			++it;
		}

		return it;
	}

	template <typename T>
	void SList<T>::Remove(const T& value)
	{
		SList<T>::Iterator foundIt = Find(value);

		if (foundIt != end())
		{
			SList<T>::Iterator it = begin();

			if (foundIt == it)
			{
				mFront = it.mNode->Next;

				delete(it.mNode);
			}
			else if (foundIt.mNode == mBack)
			{
				while (it != end())
				{
					if (it.mNode->Next == foundIt.mNode)
					{
						it.mNode->Next = nullptr;
						mBack = it.mNode;

						delete(foundIt.mNode);

						break;
					}

					++it;
				}
			}
			else
			{
				Node* nodeToDelete = foundIt.mNode->Next;
				foundIt.mNode->Data = nodeToDelete->Data;
				foundIt.mNode->Next = nodeToDelete->Next;

				if (nodeToDelete == mBack)
				{
					mBack = foundIt.mNode;
				}

				delete(nodeToDelete);
			}

			--mSize;
		}
	}

	template <typename T>
	SList<T>::SList(SList&& previousSList) :
		mSize(previousSList.mSize), mFront(previousSList.mFront), mBack(previousSList.mBack)
	{
		previousSList.mFront = nullptr;
		previousSList.mBack = nullptr;
		previousSList.mSize = 0;
	}

	template <typename T>
	SList<T>& SList<T>::operator=(SList&& previousSList)
	{
		if (this != &previousSList)
		{
			Clear();

			mSize = previousSList.mSize;
			mFront = previousSList.mFront;
			mBack = previousSList.mBack;

			previousSList.mFront = nullptr;
			previousSList.mBack = nullptr;
			previousSList.mSize = 0;
		}

		return *this;
	}

	/**
	 *	SList<T>::Iterator Implementation
	 */

	template <typename T>
	SList<T>::Iterator::Iterator() :
		mOwner(nullptr), mNode(nullptr)
	{

	}

	template <typename T>
	SList<T>::Iterator::Iterator(const SList& owner, Node* node) :
		mOwner(&owner), mNode(node)
	{

	}

	template <typename T>
	SList<T>::Iterator::~Iterator()
	{
		mNode = nullptr;
		mOwner = nullptr;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator it(*this);
		++(*this);

		return it;
	}

	template <typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Invalid owner.");
		}

		if (mNode != nullptr)
		{
			mNode = mNode->Next;
		}

		return *this;
	}

	template <typename T>
	T& SList<T>::Iterator::operator*()
	{
		return const_cast<T&> (const_cast<const Iterator*> (this)->operator*());
	}

	template <typename T>
	const T& SList<T>::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Invalid owner.");
		}

		if (*this == mOwner->end())
		{
			throw std::exception("End of list reached.");
		}

		return mNode->Data;
	}

	template <typename T>
	bool SList<T>::Iterator::operator==(const Iterator& otherIt) const
	{
		if (mOwner == otherIt.mOwner)
		{
			return mNode == otherIt.mNode;
		}

		return false;
	}

	template <typename T>
	bool SList<T>::Iterator::operator!=(const Iterator& otherIt) const
	{
		return !(*this == otherIt);
	}

	template <typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator=(const Iterator& otherIt)
	{
		if (*this != otherIt)
		{
			mOwner = otherIt.mOwner;
			mNode = otherIt.mNode;
		}

		return *this;
	}
}