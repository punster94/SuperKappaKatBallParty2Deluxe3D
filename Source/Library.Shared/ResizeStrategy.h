#pragma once
#include <cstdint>

namespace FieaGameEngine
{
	/** Resize strategy functor struct
	*	Structure for holding the operation to calculate how much the container's capacity gets incremented
	*/
	class ResizeStrategy
	{

	public:

		/** Function operator for ResizeStrategy
		*	Operation to be overridden by child classes inheriting from the ResizeStrategy
		*	@param size the size of the container.
		*	@param capacity the capacity of the container.
		*	@return the amount to increase the capacity of the container by on resize.
		*/
		virtual std::uint32_t operator()(std::uint32_t size, std::uint32_t capacity) const;

		/** Clone operator for ResizeStrategy
		*	Returns a ResizeStrategy pointer that points to a clone of this resize strategy
		*	@return a pointer to the clone of this ResizeStrategy.
		*/
		virtual ResizeStrategy* clone() const;
	};
}