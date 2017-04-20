namespace FieaGameEngine
{
	template <typename T>
	RTTI_DEFINITIONS(Event<T>)

	template <typename T>
	Event<T>::Event(const T& message, bool deleteAfterPublishing) :
		EventPublisher(sSubscribers, sMutex, deleteAfterPublishing), mMessage(message)
	{

	}

	template <typename T>
	Event<T>::Event(const Event& otherEvent) :
		EventPublisher(otherEvent), mMessage(otherEvent.mMessage)
	{

	}

	template <typename T>
	Event<T>& Event<T>::operator=(const Event& otherEvent)
	{
		if (this != &otherEvent)
		{
			EventPublisher::operator=(otherEvent);

			mMessage = otherEvent.mMessage;
		}

		return *this;
	}

	template <typename T>
	Event<T>::Event(Event&& previousEvent) :
		EventPublisher(std::move(previousEvent)), mMessage(std::move(previousEvent.mMessage))
	{
		
	}

	template <typename T>
	Event<T>& Event<T>::operator=(Event&& previousEvent)
	{
		if (this != &previousEvent)
		{
			EventPublisher::operator=(std::move(previousEvent));

			mMessage = std::move(previousEvent.mMessage);
		}

		return *this;
	}

	template <typename T>
	void Event<T>::Subscribe(EventSubscriber& subscriber)
	{
		std::lock_guard<std::mutex> lock(sMutex);

		if (sSubscribers.Find(&subscriber) == sSubscribers.end())
		{
			sSubscribers.PushBack(&subscriber);
		}
	}

	template <typename T>
	void Event<T>::Unsubscribe(EventSubscriber& subscriber)
	{
		std::lock_guard<std::mutex> lock(sMutex);

		sSubscribers.Remove(&subscriber);
	}

	template <typename T>
	void Event<T>::UnsubscribeAll()
	{
		std::lock_guard<std::mutex> lock(sMutex);

		sSubscribers.Clear();
		sSubscribers.ShrinkToFit();
		sSubscribers.Reserve(sSubscribers.defaultCapacity);
	}

	template <typename T>
	const T& Event<T>::Message() const
	{
		return mMessage;
	}

	template <typename T>
	Vector<EventSubscriber*> Event<T>::sSubscribers;

	template <typename T>
	std::mutex Event<T>::sMutex;
}