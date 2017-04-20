#pragma once
#include <cstdint>
#include <string>

namespace FieaGameEngine
{
	template <typename TKey>
	class DefaultHash
	{

	public:

		std::uint32_t operator()(const TKey& key) const;
		std::uint32_t operator()(const TKey* key) const;
	};

	template<>
	class DefaultHash<char*>
	{

	public:

		std::uint32_t operator()(const char* key) const;
	};

	template<>
	class DefaultHash<std::string>
	{

	public:

		std::uint32_t operator()(const std::string key) const;
	};
}

#include "DefaultHash.inl"