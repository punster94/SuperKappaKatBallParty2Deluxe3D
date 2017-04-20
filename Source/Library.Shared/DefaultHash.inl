namespace FieaGameEngine
{

  	template <typename TKey>
	std::uint32_t DefaultHash<TKey>::operator()(const TKey& key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(&key);

		std::uint32_t returnValue = 0;

		for (std::uint32_t i = 0; i < sizeof(TKey); ++i)
		{
			returnValue += data[i] + 11;
		}

		return returnValue;
	}

	template <typename TKey>
	std::uint32_t DefaultHash<TKey>::operator()(const TKey* key) const
	{
		const std::uint8_t data = reinterpret_cast<const std::uint8_t*>(key);

		std::uint32_t returnValue = 0;

		for (std::uint32_t i = 0; i < sizeof(TKey); ++i)
		{
			returnValue += data[i] + 11;
		}

		return returnValue;
	}

  	inline std::uint32_t DefaultHash<char*>::operator()(const char* string) const
	{
		std::uint32_t returnValue = 0;
		std::uint32_t length = static_cast<std::uint32_t>(strlen(string));

		for (std::uint32_t i = 0; i < length; ++i)
		{
			returnValue += string[i] + 11;
		}

		return returnValue;
	}

	inline std::uint32_t DefaultHash<std::string>::operator()(const std::string string) const
	{
		std::uint32_t returnValue = 0;

		for (char character : string)
		{
			returnValue += character + 11;
		}

		return returnValue;
	}
}