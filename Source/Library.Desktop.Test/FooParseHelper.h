#pragma once

#include "XmlParseMaster.h"
#include "IXmlParseHelper.h"
#include "FooSharedData.h"

namespace TestingUtilities
{
	class FooParseHelper : public FieaGameEngine::IXmlParseHelper
	{

	private:

		typedef FieaGameEngine::XmlParseMaster::SharedData SharedData;

	public:

		FooParseHelper();

		virtual ~FooParseHelper() = default;

		virtual void Initialize();

		virtual bool StartElementHandler(SharedData* sharedData, const std::string& name, FieaGameEngine::HashMap<std::string, std::string>& attributes) const;

		virtual bool EndElementHandler(SharedData* sharedData, const std::string& name) const;

		virtual bool CharDataHandler(SharedData* sharedData, const std::string& buffer, const std::uint32_t length) const;

		virtual FieaGameEngine::IXmlParseHelper* Clone() const;
	};
}