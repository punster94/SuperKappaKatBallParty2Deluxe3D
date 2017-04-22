#include "pch.h"
#include "XmlParseMaster.h"
#include "IXmlParseHelper.h"

namespace FieaGameEngine
{
#pragma region

	RTTI_DEFINITIONS(XmlParseMaster::SharedData);

	XmlParseMaster::SharedData::SharedData() :
		mXmlParseMaster(nullptr), mDepth(0)
	{

	}

	XmlParseMaster::SharedData::~SharedData()
	{

	}

	void XmlParseMaster::SharedData::Initialize()
	{

	}

	XmlParseMaster::SharedData* XmlParseMaster::SharedData::Clone() const
	{
		SharedData* clone = new SharedData();

		clone->mXmlParseMaster = mXmlParseMaster;

		return clone;
	}

	void XmlParseMaster::SharedData::SetXmlParseMaster(XmlParseMaster* xmlParseMaster)
	{
		mXmlParseMaster = xmlParseMaster;
	}

	XmlParseMaster* XmlParseMaster::SharedData::GetXmlParseMaster() const
	{
		return mXmlParseMaster;
	}

	void XmlParseMaster::SharedData::IncrementDepth()
	{
		++mDepth;
	}

	void XmlParseMaster::SharedData::DecrementDepth()
	{
		if (mDepth == 0)
		{
			throw std::exception("Cannot decrement depth past 0.");
		}

		--mDepth;
	}

	std::uint32_t XmlParseMaster::SharedData::Depth() const
	{
		return mDepth;
	}

#pragma endregion XmlParseMaster::SharedData Implementation

#pragma region

	XmlParseMaster::XmlParseMaster(SharedData* sharedData) :
		mIsClone(false), mSharedDataSetExternally(sharedData != nullptr), mParser(XML_ParserCreate(nullptr)), mFileName(""), mSharedData(sharedData)
	{
		SetParserCallbackInformation();
	}

	XmlParseMaster::~XmlParseMaster()
	{
		if (mIsClone)
		{
			for (const IXmlParseHelper* helper : mHelperList)
			{
				delete(helper);
			}

			if (mSharedData != nullptr && !mSharedDataSetExternally)
			{
				delete(mSharedData);
			}
		}

		XML_ParserFree(mParser);
	}

	XmlParseMaster* XmlParseMaster::Clone() const
	{
		XmlParseMaster* clone = new XmlParseMaster();

		clone->mSharedData = mSharedData->Clone();
		clone->mSharedData->SetXmlParseMaster(clone);

		clone->mIsClone = true;
		clone->mSharedDataSetExternally = false;

		clone->mHelperList.Reserve(mHelperList.Size());

		for (const IXmlParseHelper* helper : mHelperList)
		{
			clone->mHelperList.PushBack(helper->Clone());
		}

		return clone;
	}

	void XmlParseMaster::AddHelper(IXmlParseHelper* helper)
	{
		if (helper == nullptr)
		{
			throw std::exception("Cannot add null helper.");
		}

		if (mIsClone)
		{
			throw std::exception("Cannot add helper to cloned master");
		}

		mHelperList.PushBack(helper);
	}

	void XmlParseMaster::RemoveHelper(IXmlParseHelper* helper)
	{
		if (helper == nullptr)
		{
			throw std::exception("Cannot remove null helper.");
		}

		if (mIsClone)
		{
			throw std::exception("Cannot remove helper from cloned master");
		}

		mHelperList.Remove(helper);
	}

	void XmlParseMaster::Parse(const char* buffer, const size_t length)
	{
		InitializeDependencies();

		XML_Status result = XML_Parse(mParser, buffer, static_cast<std::int32_t>(length), true);

		ResetParser();

		if (result == XML_STATUS_ERROR)
		{
			throw std::exception("Error parsing input.");
		}
	}

	void XmlParseMaster::ParseFromFile(const std::string& fileName)
	{
		std::ifstream fileHandle;
		fileHandle.open(fileName);

		if (!fileHandle.is_open())
		{
			throw std::exception("File could not be opened.");
		}

		mFileName = fileName;

		std::string contents, line;

		while (getline(fileHandle, line))
		{
			contents += line;
		}

		Parse(contents.c_str(), contents.length());

		fileHandle.close();
	}

	const std::string& XmlParseMaster::GetFileName() const
	{
		return mFileName;
	}

	XmlParseMaster::SharedData* XmlParseMaster::GetSharedData() const
	{
		return mSharedData;
	}

	void XmlParseMaster::SetSharedData(SharedData* sharedData)
	{
		if (sharedData != nullptr)
		{
			mSharedDataSetExternally = true;
			mSharedData = sharedData;

			mSharedData->SetXmlParseMaster(this);
		}
	}

	void XmlParseMaster::StartElementHandler(void* userData, const char* name, const char** attributes)
	{
		XmlParseMaster* master = reinterpret_cast<XmlParseMaster*>(userData);

		HashMap<std::string, std::string> attributeMap;

		for (size_t i = 0; attributes[i]; i += 2)
		{
			std::pair<std::string, std::string> pair = { attributes[i], attributes[i + 1] };

			attributeMap.Insert(pair);
		}

		for (const IXmlParseHelper* helper : master->mHelperList)
		{
			if (helper->StartElementHandler(master->mSharedData, name, attributeMap))
			{
				break;
			}
		}

		if (master->mSharedData != nullptr)
		{
			master->mSharedData->IncrementDepth();
		}
	}

	void XmlParseMaster::EndElementHandler(void* userData, const char* name)
	{
		XmlParseMaster* master = reinterpret_cast<XmlParseMaster*>(userData);

		for (const IXmlParseHelper* helper : master->mHelperList)
		{
			if (helper->EndElementHandler(master->mSharedData, name))
			{
				break;
			}
		}

		if (master->mSharedData != nullptr)
		{
			master->mSharedData->DecrementDepth();
		}
	}

	void XmlParseMaster::CharDataHandler(void* userData, const char* buffer, std::int32_t length)
	{
		XmlParseMaster* master = reinterpret_cast<XmlParseMaster*>(userData);

		std::string stringBuffer = std::string(buffer, length);

		for (const IXmlParseHelper* helper : master->mHelperList)
		{
			if (helper->CharDataHandler(master->mSharedData, stringBuffer, length))
			{
				break;
			}
		}
	}

	void XmlParseMaster::ResetParser()
	{
		XML_ParserReset(mParser, nullptr);

		SetParserCallbackInformation();
	}

	void XmlParseMaster::SetParserCallbackInformation()
	{
		XML_SetElementHandler(mParser, StartElementHandler, EndElementHandler);
		XML_SetCharacterDataHandler(mParser, CharDataHandler);
		XML_SetUserData(mParser, this);
	}

	void XmlParseMaster::InitializeDependencies()
	{
		if (mSharedData != nullptr)
		{
			mSharedData->Initialize();
		}

		for (IXmlParseHelper* helper : mHelperList)
		{
			helper->Initialize();
		}
	}

#pragma endregion XmlParseMaster Implementation
}