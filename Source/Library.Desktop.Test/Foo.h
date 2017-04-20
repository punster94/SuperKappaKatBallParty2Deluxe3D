#pragma once
#include <cstdint>
#include "RTTI.h"
#include "Factory.h"

namespace TestingUtilities
{

	class Foo : public FieaGameEngine::RTTI
	{

		RTTI_DECLARATIONS(Foo, FieaGameEngine::RTTI);

	public:

		Foo();

		Foo(std::int32_t& data);

		Foo(const Foo& otherFoo);

		~Foo();

		Foo& operator=(const Foo& otherFoo);

		Foo(Foo&& previousFoo);

		Foo& operator=(Foo&& previousFoo);

		bool operator==(const Foo& otherFoo) const;

		bool operator!=(const Foo& otherFoo) const;

		void SetData(std::int32_t data);

		std::int32_t& GetData() const;

		bool Equals(const RTTI* otherFoo) const override;

	private:

		std::int32_t* mData;
	};
}