#include "pch.h"
#include "Foo.h"

namespace TestingUtilities
{

	RTTI_DEFINITIONS(Foo);

	Foo::Foo() :
		mData(nullptr)
	{

	}

	Foo::Foo(std::int32_t& data) :
		mData(new std::int32_t(data))
	{
		
	}

	Foo::Foo(const Foo& otherFoo) :
		mData(otherFoo.mData)
	{
		if (otherFoo.mData != nullptr)
		{
			mData = new std::int32_t(*otherFoo.mData);
		}
	}

	Foo::~Foo()
	{
		if (mData != nullptr)
		{
			delete(mData);
		}
	}

	Foo& Foo::operator=(const Foo& otherFoo)
	{
		if (this != &otherFoo)
		{
			if (mData != nullptr)
			{
				delete(mData);
			}

			mData = otherFoo.mData;

			if (otherFoo.mData != nullptr)
			{
				mData = new std::int32_t(*otherFoo.mData);
			}
		}

		return *this;
	}

	Foo::Foo(Foo&& previousFoo) :
		mData(previousFoo.mData)
	{
		previousFoo.mData = nullptr;
	}

	Foo& Foo::operator=(Foo&& previousFoo)
	{
		if (this != &previousFoo)
		{
			if (mData != nullptr)
			{
				delete(mData);
			}

			mData = previousFoo.mData;

			previousFoo.mData = nullptr;
		}

		return *this;
	}

	bool Foo::operator==(const Foo& otherFoo) const
	{
		if (mData == nullptr)
		{
			return otherFoo.mData == nullptr;
		}

		return *mData == *(otherFoo.mData);
	}

	bool Foo::operator!=(const Foo& otherFoo) const
	{
		return !(operator==(otherFoo));
	}

	void Foo::SetData(std::int32_t data)
	{
		*mData = data;
	}

	std::int32_t& Foo::GetData() const
	{
		return *mData;
	}

	bool Foo::Equals(const RTTI* otherFoo) const
	{
		if (this == otherFoo)
		{
			return true;
		}

		Foo* fooPointer = otherFoo->As<Foo>();

		return fooPointer == nullptr ? false : *this == *fooPointer;
	}
}