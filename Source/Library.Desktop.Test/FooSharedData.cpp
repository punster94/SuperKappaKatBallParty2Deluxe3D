#include "pch.h"

namespace TestingUtilities
{
	FooSharedData::FooSharedData() :
		FieaGameEngine::XmlParseMaster::SharedData()
	{

	}

	FooSharedData::~FooSharedData()
	{

	}

	void FooSharedData::Initialize()
	{
		mMaps.Clear();
		mCharData.Clear();
	}

	FieaGameEngine::XmlParseMaster::SharedData* FooSharedData::Clone() const
	{
		FooSharedData* clone = new FooSharedData();

		clone->mMaps = mMaps;
		clone->mCharData = mCharData;
		clone->SetXmlParseMaster(GetXmlParseMaster());

		return clone;
	}

	void FooSharedData::AddMap(const std::string& name, FieaGameEngine::HashMap<std::string, std::string>& map)
	{
		mMaps[name] = map;
	}

	void FooSharedData::AddCharData(const std::string& data)
	{
		mCharData.PushBack(data);
	}

	bool FooSharedData::Equals(FieaGameEngine::HashMap<std::string, FieaGameEngine::HashMap<std::string, std::string>>& maps, FieaGameEngine::Vector<std::string>& charData)
	{
		charData;
		bool result = maps.Size() == mMaps.Size()
			&& charData.Size() == mCharData.Size()
			&& maps == mMaps;

		if (result)
		{
			for (std::uint32_t i = 0; i < charData.Size(); ++i)
			{
				if (charData[i] != mCharData[i])
				{
					result = false;
					break;
				}
			}
		}

		return result;
	}
}