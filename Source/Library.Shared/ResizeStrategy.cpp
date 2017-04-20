#include "pch.h"

namespace FieaGameEngine
{
	/**
	*	ResizeStrategy Implementation
	*/
	std::uint32_t ResizeStrategy::operator()(std::uint32_t, std::uint32_t capacity) const
	{
		return capacity + 1;
	}

	ResizeStrategy* ResizeStrategy::clone() const
	{
		return new ResizeStrategy(*this);
	}
}