#include "pch.h"

namespace TestingUtilities
{
	std::uint32_t FooHashFunctor::operator()(const Foo& key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(&key.GetData());

		std::uint32_t returnValue = 0;

		for (std::uint32_t i = 0; i < sizeof(std::int32_t); ++i)
		{
			returnValue += data[i] + 11;
		}

		return returnValue;
	}
}