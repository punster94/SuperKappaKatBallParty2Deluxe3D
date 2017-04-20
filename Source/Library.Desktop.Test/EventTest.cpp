#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(EventTest)
	{
	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			Attributed::ClearPrescribedAttributes();
			Event<Foo>::UnsubscribeAll();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);

			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}

		TEST_METHOD(EventQueueEnqueue)
		{
			GameTime gameTime;
			std::chrono::high_resolution_clock::time_point timePoint = gameTime.CurrentTime();
			std::int32_t value = 10;
			Foo foo(value);
			EventQueue queue;
			Event<Foo>* fooEvent = new Event<Foo>(foo);
			Event<Foo>* otherFooEvent = new Event<Foo>(foo);

			queue.Enqueue(*fooEvent, gameTime);

			Assert::AreEqual(1U, queue.Size());
			Assert::IsTrue(fooEvent->TimeEnqueued() == timePoint);
			Assert::IsTrue(fooEvent->Delay() == std::chrono::milliseconds::zero());

			timePoint += std::chrono::milliseconds(value);
			gameTime.SetCurrentTime(timePoint);
			queue.Enqueue(*otherFooEvent, gameTime, std::chrono::milliseconds(value));

			Assert::AreEqual(2U, queue.Size());
			Assert::IsTrue(otherFooEvent->TimeEnqueued() == timePoint);
			Assert::IsTrue(otherFooEvent->Delay() == std::chrono::milliseconds(value));
		}

		TEST_METHOD(EventQueueSend)
		{
			std::int32_t value = 10, otherValue = 2;
			Foo foo(value), otherFoo(otherValue);
			EventQueue queue;
			FooSubscriber subscriber;

			Event<Foo>::Subscribe(subscriber);

			Event<Foo>* fooEvent = new Event<Foo>(foo);
			Event<Foo>* otherFooEvent = new Event<Foo>(otherFoo, false);

			queue.Send(*fooEvent);

			Assert::IsTrue(subscriber.mFoo == foo);

			queue.Send(*otherFooEvent);

			Assert::IsTrue(subscriber.mFoo == otherFoo);

			delete(otherFooEvent);
		}

		TEST_METHOD(EventQueueUpdate)
		{
			GameTime gameTime;
			std::chrono::high_resolution_clock::time_point timePoint = gameTime.CurrentTime();
			std::int32_t value = 2, otherValue = 5, thirdValue = 10;
			Foo foo(value), otherFoo(otherValue), thirdFoo(thirdValue);
			EventQueue queue;
			FooSubscriber subscriber;

			Event<Foo>::Subscribe(subscriber);
			Event<Foo>* fooEvent = new Event<Foo>(foo, false);
			Event<Foo>* otherFooEvent = new Event<Foo>(otherFoo);
			Event<Foo>* thirdFooEvent = new Event<Foo>(thirdFoo);

			queue.Enqueue(*fooEvent, gameTime);
			queue.Enqueue(*otherFooEvent, gameTime, std::chrono::milliseconds(otherValue));
			queue.Enqueue(*thirdFooEvent, gameTime, std::chrono::milliseconds(thirdValue));

			queue.Update(gameTime);

			delete(fooEvent);

			Assert::AreEqual(2U, queue.Size());
			Assert::IsTrue(subscriber.mFoo == foo);

			timePoint += std::chrono::milliseconds(value);
			gameTime.SetCurrentTime(timePoint);

			queue.Update(gameTime);

			Assert::AreEqual(2U, queue.Size());

			timePoint += std::chrono::milliseconds(otherValue);
			gameTime.SetCurrentTime(timePoint);

			queue.Update(gameTime);

			Assert::AreEqual(1U, queue.Size());
			Assert::IsTrue(subscriber.mFoo == otherFoo);

			timePoint += std::chrono::milliseconds(thirdValue);
			gameTime.SetCurrentTime(timePoint);

			queue.Update(gameTime);

			Assert::AreEqual(0U, queue.Size());
			Assert::IsTrue(subscriber.mFoo == thirdFoo);
		}

		TEST_METHOD(EventQueueClear)
		{
			GameTime gameTime;
			std::chrono::high_resolution_clock::time_point timePoint = gameTime.CurrentTime();
			std::int32_t value = 10, otherValue = 2;
			Foo foo(value), otherFoo(otherValue);
			EventQueue queue;
			FooSubscriber subscriber;

			Event<Foo>::Subscribe(subscriber);
			Event<Foo>* fooEvent = new Event<Foo>(foo);
			Event<Foo>* otherFooEvent = new Event<Foo>(otherFoo);

			queue.Enqueue(*fooEvent, gameTime);
			queue.Enqueue(*otherFooEvent, gameTime, std::chrono::milliseconds(value));

			timePoint += std::chrono::milliseconds(1);
			gameTime.SetCurrentTime(timePoint);

			queue.Clear();

			Assert::AreEqual(0U, queue.Size());
		}

		TEST_METHOD(EventQueueIsEmpty)
		{
			GameTime gameTime;
			std::int32_t value = 10, otherValue = 2;
			Foo foo(value), otherFoo(otherValue);
			EventQueue queue;

			Event<Foo>* fooEvent = new Event<Foo>(foo);
			Event<Foo>* otherFooEvent = new Event<Foo>(otherFoo);

			Assert::IsTrue(queue.IsEmpty());

			queue.Enqueue(*fooEvent, gameTime);

			Assert::IsFalse(queue.IsEmpty());

			queue.Enqueue(*otherFooEvent, gameTime);

			Assert::IsFalse(queue.IsEmpty());

			queue.Clear();

			Assert::IsTrue(queue.IsEmpty());
		}

		TEST_METHOD(EventQueueSize)
		{
			GameTime gameTime;
			std::int32_t value = 10, otherValue = 2;
			Foo foo(value), otherFoo(otherValue);
			EventQueue queue;

			Event<Foo>* fooEvent = new Event<Foo>(foo);
			Event<Foo>* otherFooEvent = new Event<Foo>(otherFoo);

			Assert::AreEqual(0U, queue.Size());

			queue.Enqueue(*fooEvent, gameTime);

			Assert::AreEqual(1U, queue.Size());

			queue.Enqueue(*otherFooEvent, gameTime);

			Assert::AreEqual(2U, queue.Size());

			queue.Clear();

			Assert::AreEqual(0U, queue.Size());
		}

		TEST_METHOD(EventConstructor)
		{
			std::int32_t value = 10;
			Foo foo(value);
			Event<Foo> fooEvent(foo);

			Assert::IsTrue(foo == fooEvent.Message());
			Assert::IsTrue(fooEvent.DeleteAfterPublishing());
			Assert::IsTrue(fooEvent.TimeEnqueued() == std::chrono::high_resolution_clock::time_point());
			Assert::IsTrue(fooEvent.Delay() == std::chrono::milliseconds::zero());

			Event<Foo> otherFooEvent(foo, false);

			Assert::IsTrue(foo == otherFooEvent.Message());
			Assert::IsFalse(otherFooEvent.DeleteAfterPublishing());
			Assert::IsTrue(fooEvent.TimeEnqueued() == std::chrono::high_resolution_clock::time_point());
			Assert::IsTrue(fooEvent.Delay() == std::chrono::milliseconds::zero());
		}

		TEST_METHOD(EventCopyConstructor)
		{
			GameTime gameTime;
			std::chrono::high_resolution_clock::time_point timePoint = gameTime.CurrentTime();
			std::int32_t value = 10;
			Foo foo(value);
			Event<Foo> fooEvent(foo, false);
			fooEvent.SetTime(timePoint, std::chrono::milliseconds(value));

			Event<Foo> otherFooEvent(fooEvent);

			Assert::IsTrue(fooEvent.TimeEnqueued() == otherFooEvent.TimeEnqueued());
			Assert::IsTrue(fooEvent.Delay() == otherFooEvent.Delay());
			Assert::IsTrue(fooEvent.DeleteAfterPublishing() == otherFooEvent.DeleteAfterPublishing());
			Assert::IsTrue(fooEvent.Message() == otherFooEvent.Message());
		}

		TEST_METHOD(EventAssignmentOperator)
		{
			GameTime gameTime;
			std::chrono::high_resolution_clock::time_point timePoint = gameTime.CurrentTime();
			std::int32_t value = 10, otherValue = 2;
			Foo foo(value), otherFoo(otherValue);
			Event<Foo> fooEvent(foo, false);
			fooEvent.SetTime(timePoint, std::chrono::milliseconds(value));

			Event<Foo> otherFooEvent(otherFoo, true);

			otherFooEvent = fooEvent;

			Assert::IsTrue(fooEvent.TimeEnqueued() == otherFooEvent.TimeEnqueued());
			Assert::IsTrue(fooEvent.Delay() == otherFooEvent.Delay());
			Assert::IsTrue(fooEvent.DeleteAfterPublishing() == otherFooEvent.DeleteAfterPublishing());
			Assert::IsTrue(fooEvent.Message() == otherFooEvent.Message());
		}

		TEST_METHOD(EventMoveConstructor)
		{
			GameTime gameTime;
			std::chrono::high_resolution_clock::time_point timePoint = gameTime.CurrentTime();
			std::int32_t value = 10;
			Foo foo(value);
			Event<Foo> fooEvent(foo, false);
			fooEvent.SetTime(timePoint, std::chrono::milliseconds(value));

			Event<Foo> otherFooEvent(std::move(fooEvent));

			Assert::IsTrue(otherFooEvent.TimeEnqueued() == timePoint);
			Assert::IsTrue(otherFooEvent.Delay() == std::chrono::milliseconds(value));
			Assert::IsFalse(otherFooEvent.DeleteAfterPublishing());
			Assert::IsTrue(otherFooEvent.Message() == foo);
			Assert::IsTrue(fooEvent.TimeEnqueued() == std::chrono::high_resolution_clock::time_point());
			Assert::IsTrue(fooEvent.Delay() == std::chrono::milliseconds::zero());
			Assert::IsTrue(fooEvent.DeleteAfterPublishing());
			Assert::IsTrue(fooEvent.Message() == Foo());
		}

		TEST_METHOD(EventMoveAssignmentOperator)
		{
			GameTime gameTime;
			std::chrono::high_resolution_clock::time_point timePoint = gameTime.CurrentTime();
			std::int32_t value = 10, otherValue = 2;
			Foo foo(value), otherFoo(otherValue);
			Event<Foo> fooEvent(foo, false);
			fooEvent.SetTime(timePoint, std::chrono::milliseconds(value));

			Event<Foo> otherFooEvent(otherFoo);
			otherFooEvent = std::move(fooEvent);

			Assert::IsTrue(otherFooEvent.TimeEnqueued() == timePoint);
			Assert::IsTrue(otherFooEvent.Delay() == std::chrono::milliseconds(value));
			Assert::IsFalse(otherFooEvent.DeleteAfterPublishing());
			Assert::IsTrue(otherFooEvent.Message() == foo);
			Assert::IsTrue(fooEvent.TimeEnqueued() == std::chrono::high_resolution_clock::time_point());
			Assert::IsTrue(fooEvent.Delay() == std::chrono::milliseconds::zero());
			Assert::IsTrue(fooEvent.DeleteAfterPublishing());
			Assert::IsTrue(fooEvent.Message() == Foo());
		}

		TEST_METHOD(EventSubscribe)
		{
			FooSubscriber subscriber;

			Event<Foo>::Subscribe(subscriber);
			Event<Foo>::Subscribe(subscriber);
		}

		TEST_METHOD(EventUnsubscribe)
		{
			FooSubscriber subscriber;

			Event<Foo>::Unsubscribe(subscriber);
			Event<Foo>::Unsubscribe(subscriber);
		}

		TEST_METHOD(EventUnsubscribeAll)
		{
			FooSubscriber subscriber;

			Event<Foo>::UnsubscribeAll();

			Event<Foo>::Subscribe(subscriber);

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventMessage)
		{
			std::int32_t value = 10, otherValue = 2;
			Foo foo(value), otherFoo(otherValue);
			Event<Foo> fooEvent(foo), otherFooEvent(otherFoo);

			Assert::IsTrue(fooEvent.Message() == foo);
			Assert::IsTrue(otherFooEvent.Message() == otherFoo);
		}

		TEST_METHOD(EventSetTime)
		{
			std::chrono::high_resolution_clock::time_point timePoint;
			std::int32_t value = 10, otherValue = 2;
			Foo foo(value);
			Event<Foo> fooEvent(foo, false);

			fooEvent.SetTime(timePoint, std::chrono::milliseconds(value));

			Assert::IsTrue(fooEvent.TimeEnqueued() == timePoint);
			Assert::IsTrue(fooEvent.Delay() == std::chrono::milliseconds(value));

			timePoint += std::chrono::milliseconds(value);

			fooEvent.SetTime(timePoint, std::chrono::milliseconds(otherValue));

			Assert::IsTrue(fooEvent.TimeEnqueued() == timePoint);
			Assert::IsTrue(fooEvent.Delay() == std::chrono::milliseconds(otherValue));
		}

		TEST_METHOD(EventTimeEnqueued)
		{
			std::chrono::high_resolution_clock::time_point timePoint;
			std::int32_t value = 10, otherValue = 2;
			Foo foo(value);
			Event<Foo> fooEvent(foo, false);

			Assert::IsTrue(fooEvent.TimeEnqueued() == std::chrono::high_resolution_clock::time_point());

			timePoint += std::chrono::milliseconds(value);
			fooEvent.SetTime(timePoint, std::chrono::milliseconds(value));

			Assert::IsTrue(fooEvent.TimeEnqueued() == timePoint);

			timePoint += std::chrono::milliseconds(value);
			fooEvent.SetTime(timePoint, std::chrono::milliseconds(otherValue));

			Assert::IsTrue(fooEvent.TimeEnqueued() == timePoint);
		}

		TEST_METHOD(EventDelay)
		{
			std::chrono::high_resolution_clock::time_point timePoint;
			std::int32_t value = 10, otherValue = 2;
			Foo foo(value);
			Event<Foo> fooEvent(foo, false);

			Assert::IsTrue(fooEvent.Delay() == std::chrono::milliseconds::zero());

			fooEvent.SetTime(timePoint, std::chrono::milliseconds(value));

			Assert::IsTrue(fooEvent.Delay() == std::chrono::milliseconds(value));

			fooEvent.SetTime(timePoint, std::chrono::milliseconds(otherValue));

			Assert::IsTrue(fooEvent.Delay() == std::chrono::milliseconds(otherValue));
		}

		TEST_METHOD(EventIsExpired)
		{
			std::chrono::high_resolution_clock::time_point timePoint;
			std::int32_t value = 10, otherValue = 2;
			Foo foo(value);
			Event<Foo> fooEvent(foo, false);

			Assert::IsTrue(fooEvent.IsExpired(timePoint));

			fooEvent.SetTime(timePoint, std::chrono::milliseconds(value));

			Assert::IsFalse(fooEvent.IsExpired(timePoint));

			timePoint += std::chrono::milliseconds(otherValue);

			Assert::IsFalse(fooEvent.IsExpired(timePoint));

			timePoint += std::chrono::milliseconds(value);

			Assert::IsTrue(fooEvent.IsExpired(timePoint));
		}

		TEST_METHOD(EventDeliver)
		{
			std::int32_t value = 10;
			Foo foo(value);
			Event<Foo> fooEvent(foo, false);
			FooSubscriber subscriber, otherSubscriber;

			Event<Foo>::Subscribe(subscriber);
			Event<Foo>::Subscribe(otherSubscriber);

			fooEvent.Deliver();

			Assert::IsTrue(subscriber.mFoo == foo);
			Assert::IsTrue(otherSubscriber.mFoo == foo);
		}

		TEST_METHOD(EventDeleteActerPublishing)
		{
			std::int32_t value = 10;
			Foo foo(value);
			Event<Foo> fooEvent(foo, false), otherFooEvent(foo, true);

			Assert::IsFalse(fooEvent.DeleteAfterPublishing());
			Assert::IsTrue(otherFooEvent.DeleteAfterPublishing());
		}

		TEST_METHOD(EventRTTIMethods)
		{
			const Vector<EventSubscriber*> fakeList;
			std::mutex fakeMutex;
			Foo foo;
			Event<Foo> fooEvent(foo);
			EventPublisher publisher(fakeList, fakeMutex);

			Assert::IsTrue(Event<Foo>::TypeIdClass() != EventPublisher::TypeIdClass());
			Assert::IsTrue(Event<Foo>::TypeName() != EventPublisher::TypeName());

			Assert::IsTrue(fooEvent.Is(Event<Foo>::TypeIdClass()));
			Assert::IsTrue(fooEvent.Is(Event<Foo>::TypeName()));
			Assert::AreEqual(Event<Foo>::TypeIdClass(), fooEvent.TypeIdInstance());
			Assert::AreEqual(Event<Foo>::TypeName(), fooEvent.TypeName());

			Assert::IsTrue(publisher.Is(EventPublisher::TypeIdClass()));
			Assert::IsTrue(publisher.Is(EventPublisher::TypeName()));
			Assert::AreEqual(EventPublisher::TypeIdClass(), publisher.TypeIdInstance());
			Assert::AreEqual(EventPublisher::TypeName(), publisher.TypeName());

			RTTI* asRTTI = &fooEvent;
			RTTI* otherRTTI = &publisher;

			Assert::IsFalse(nullptr == asRTTI->QueryInterface(Event<Foo>::TypeIdClass()));
			Assert::IsTrue(&fooEvent == asRTTI->As<Event<Foo>>());
			Assert::AreEqual(asRTTI->As<Event<Foo>>()->ToString(), fooEvent.ToString());

			Assert::IsFalse(nullptr == otherRTTI->QueryInterface(EventPublisher::TypeIdClass()));
			Assert::IsTrue(&publisher == otherRTTI->As<EventPublisher>());
			Assert::AreEqual(otherRTTI->As<EventPublisher>()->ToString(), publisher.ToString());
		}

		TEST_METHOD(EventExceptionTest)
		{
			ExceptionSubscriber subscriber;
			EventQueue queue;
			GameTime gameTime;
			Foo foo;

			Event<Foo>::Subscribe(subscriber);

			Event<Foo>* e = new Event<Foo>(foo);

			queue.Enqueue(*e, gameTime);

			auto updatePointer = [&queue, &gameTime] { queue.Update(gameTime); };
			
			Assert::ExpectException<std::exception>(updatePointer);
			Assert::AreEqual(0U, queue.Size());
		}

		TEST_METHOD(EventClearSubscriberTest)
		{
			Foo foo;
			ClearSubscriber subscriber;
			GameTime gameTime;
			EventQueue queue;

			subscriber.mQueue = &queue;

			Event<Foo>::Subscribe(subscriber);

			std::uint32_t eventCount = 1000;

			for (std::uint32_t i = 0; i < eventCount; ++i)
			{
				Event<Foo>* e = new Event<Foo>(foo);
				queue.Enqueue(*e, gameTime);
			}

			queue.Update(gameTime);

			Assert::AreEqual(0U, queue.Size());
		}

		TEST_METHOD(EventQueueStressTest)
		{
			EventQueue queue;
			GameTime gameTime;
			Foo foo;
			Vector<FooSubscriber*> subscribers;

			std::uint32_t eventCount = 100;

			for (std::uint32_t i = 0; i < eventCount; ++i)
			{
				Event<Foo>* e = new Event<Foo>(foo);
				queue.Enqueue(*e, gameTime);
			}

			std::uint32_t subscriberCount = 100;

			for (std::uint32_t i = 0; i < subscriberCount; ++i)
			{
				FooSubscriber* sub = new FooSubscriber();
				sub->mNotifyCount = 0;
				subscribers.PushBack(sub);
				Event<Foo>::Subscribe(*sub);
			}

			queue.Update(gameTime);

			Vector<std::uint32_t> counts;

			for (FooSubscriber* sub : subscribers)
			{
				counts.PushBack(sub->mNotifyCount);
				delete(sub);
			}

			for (std::uint32_t i : counts)
			{
				Assert::AreEqual(eventCount, i);
			}
		}

		TEST_METHOD(EventThrowSubscriberTest)
		{
			std::string subtype = "howdy";
			WorldState worldState;
			World world;
			Foo foo;
			EventThrowSubscriber subscriber;

			worldState.mWorld = &world;
			subscriber.mWorldState = &worldState;

			Event<Foo>::Subscribe(subscriber);

			std::uint32_t eventCount = 5000;

			for (std::uint32_t i = 0; i < eventCount; ++i)
			{
				Event<Foo>* e = new Event<Foo>(foo);
				world.Enqueue(*e, worldState, 0);
			}

			world.Update(worldState);

			Assert::AreEqual(eventCount, world.QueueSize());
		}

		TEST_METHOD(EventSubscribeSubscriberTest)
		{
			Foo foo;
			SubscribeSubscriber subscriber;
			GameTime gameTime;
			EventQueue queue;

			Event<Foo>::Subscribe(subscriber);

			std::uint32_t eventCount = 500;

			for (std::uint32_t i = 0; i < eventCount; ++i)
			{
				Event<Foo>* e = new Event<Foo>(foo);
				queue.Enqueue(*e, gameTime);
			}

			queue.Update(gameTime);

			Assert::AreEqual(0U, queue.Size());
			Assert::AreEqual(eventCount, subscriber.mChildSubscribers.Size());
		}

		TEST_METHOD(EventUnsubscribeSubscriberTest)
		{
			Foo foo;
			GameTime gameTime;
			EventQueue queue;

			std::uint32_t eventCount = 100;

			for (std::uint32_t i = 0; i < eventCount; ++i)
			{
				Event<Foo>* e = new Event<Foo>(foo);
				queue.Enqueue(*e, gameTime);
			}

			Vector<UnsubscribeSubscriber*> subscribers;

			std::uint32_t subCount = 100;

			for (std::uint32_t i = 0; i < subCount; ++i)
			{
				UnsubscribeSubscriber* subscriber = new UnsubscribeSubscriber();
				Event<Foo>::Subscribe(*subscriber);
				subscribers.PushBack(subscriber);
			}

			queue.Update(gameTime);
			
			Vector<bool> subscribed;

			for (UnsubscribeSubscriber* sub : subscribers)
			{
				subscribed.PushBack(sub->mUnsubscribed);
				delete(sub);
			}

			for (bool i : subscribed)
			{
				Assert::IsTrue(i);
			}
		}

	private:

		static _CrtMemState sStartMemState;

	};

	_CrtMemState EventTest::sStartMemState;
}
