#include "pch.h"

namespace TestingUtilities
{
	FooParseHelper::FooParseHelper()
	{

	}

	void FooParseHelper::Initialize()
	{

	}

	bool FooParseHelper::StartElementHandler(SharedData* sharedData, const std::string& name, FieaGameEngine::HashMap<std::string, std::string>& attributes) const
	{
		if (sharedData != nullptr)
		{
			FooSharedData* fooSharedData = sharedData->As<FooSharedData>();

			if (fooSharedData != nullptr)
			{
				fooSharedData->AddMap(name, attributes);

				return true;
			}
		}

		return false;
	}

	bool FooParseHelper::EndElementHandler(SharedData*, const std::string&) const
	{
		return true;
	}

	bool FooParseHelper::CharDataHandler(SharedData* sharedData, const std::string& buffer, const std::uint32_t length) const
	{
		if (sharedData != nullptr)
		{
			FooSharedData* fooSharedData = sharedData->As<FooSharedData>();

			if (fooSharedData != nullptr && length > 0)
			{
				if (buffer.length() > 0)
				{
					fooSharedData->AddCharData(buffer);

					return true;
				}
			}
		}

		return false;
	}

	FieaGameEngine::IXmlParseHelper* FooParseHelper::Clone() const
	{
		return new FooParseHelper();
	}
}