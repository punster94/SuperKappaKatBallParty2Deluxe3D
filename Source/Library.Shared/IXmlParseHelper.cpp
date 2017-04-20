#include "pch.h"

namespace FieaGameEngine
{
	IXmlParseHelper::IXmlParseHelper()
	{

	}

	IXmlParseHelper::~IXmlParseHelper()
	{

	}

	void IXmlParseHelper::Initialize()
	{
		
	}

	bool IXmlParseHelper::StartElementHandler(XmlParseMaster::SharedData*, const std::string&, HashMap<std::string, std::string>&) const
	{
		return false;
	}

	bool IXmlParseHelper::EndElementHandler(XmlParseMaster::SharedData*, const std::string&) const
	{
		return false;
	}

	bool IXmlParseHelper::CharDataHandler(XmlParseMaster::SharedData*, const std::string&, const std::uint32_t) const
	{
		return false;
	}
}