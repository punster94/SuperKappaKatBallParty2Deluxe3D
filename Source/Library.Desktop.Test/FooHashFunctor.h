#pragma once
#include <cstdint>
#include "Foo.h"

namespace TestingUtilities
{
	class FooHashFunctor
	{

	public:

		std::uint32_t operator()(const Foo& key) const;
	};
}