#pragma once

#include "XmlParseMaster.h"
#include "Vector.h"
#include "HashMap.h"

namespace TestingUtilities
{
	class FooSharedData : public FieaGameEngine::XmlParseMaster::SharedData
	{

	public:

		FooSharedData();

		virtual ~FooSharedData();

		virtual void Initialize();

		virtual FieaGameEngine::XmlParseMaster::SharedData* Clone() const;

		void AddMap(const std::string& name, FieaGameEngine::HashMap<std::string, std::string>& map);

		void AddCharData(const std::string& data);

		bool Equals(FieaGameEngine::HashMap<std::string, FieaGameEngine::HashMap<std::string, std::string>>& maps, FieaGameEngine::Vector<std::string>& charData);

	private:

		FieaGameEngine::HashMap<std::string, FieaGameEngine::HashMap<std::string, std::string>> mMaps;
		
		FieaGameEngine::Vector<std::string> mCharData;
	};
}