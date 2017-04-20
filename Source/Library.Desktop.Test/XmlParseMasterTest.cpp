#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(XmlParseMasterTest)
	{

	public:

		TEST_CLASS_INITIALIZE(InitializeTestClass)
		{
			sMalformedXml = "<?xml version=\"1.0\"<<<<<<<F<EGWHwhw342t24>";

			sTestFileContents = "<?xml version='1.0' encoding='utf-8'?><Foo name='first foo'><Bar1 name='first bar'><Bat1 name='first bat'>First Foo Bar Bat</Bat1><Bat2 name='second bat'>Second Foo Bar Bat</Bat2></Bar1><Bar2 name='second bar'><Bat3 name='third bat'>Third Foo Bar Bat</Bat3><Bat4 name='fourth bat'>Fourth Foo Bar Bat</Bat4></Bar2><Bar3 name='third bar'><Bat5 name='fifth bat'>Fifth Foo Bar Bat</Bat5><Bat6 name='sixth bat'>Sixth Foo Bar Bat</Bat6></Bar3></Foo>";

			sTestFileCharData.PushBack("First Foo Bar Bat");
			sTestFileCharData.PushBack("Second Foo Bar Bat");
			sTestFileCharData.PushBack("Third Foo Bar Bat");
			sTestFileCharData.PushBack("Fourth Foo Bar Bat");
			sTestFileCharData.PushBack("Fifth Foo Bar Bat");
			sTestFileCharData.PushBack("Sixth Foo Bar Bat");

			std::string foo = "Foo", bar1 = "Bar1", bar2 = "Bar2", bar3 = "Bar3", bat1 = "Bat1", bat2 = "Bat2", bat3 = "Bat3", bat4 = "Bat4", bat5 = "Bat5", bat6 = "Bat6";
			std::string fFoo = "first foo", fBar = "first bar", sBar = "second bar", tBar = "third bar";
			std::string fBat = "first bat", sBat = "second bat", tBat = "third bat", oBat = "fourth bat", iBat = "fifth bat", xBat = "sixth bat";

			AddTestFileAttributeData(foo, fFoo);
			AddTestFileAttributeData(bar1, fBar);
			AddTestFileAttributeData(bat1, fBat);
			AddTestFileAttributeData(bat2, sBat);
			AddTestFileAttributeData(bar2, sBar);
			AddTestFileAttributeData(bat3, tBat);
			AddTestFileAttributeData(bat4, oBat);
			AddTestFileAttributeData(bar3, tBar);
			AddTestFileAttributeData(bat5, iBat);
			AddTestFileAttributeData(bat6, xBat);
		}

		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);

			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}

		TEST_METHOD(FooParseHelperConstructor)
		{
			FooSharedData sharedData;
			FooParseHelper helper;
		}

		TEST_METHOD(FooParseHelperDestructor)
		{
			FooSharedData sharedData;
			FooParseHelper helper;

			FooParseHelper* otherHelper = new FooParseHelper();

			delete(otherHelper);
		}

		TEST_METHOD(FooParseHelperInitialize)
		{
			FooParseHelper helper;

			helper.Initialize();
		}

		TEST_METHOD(FooParseHelperStartElementHandler)
		{
			FooParseHelper helper;
			FooSharedData sharedData;

			Assert::IsFalse(helper.StartElementHandler(nullptr, "Foo", sTestFileAttributeData["Foo"]));
			Assert::IsTrue(helper.StartElementHandler(&sharedData, "Foo", sTestFileAttributeData["Foo"]));
		}

		TEST_METHOD(FooParseHelperEndElementHandler)
		{
			FooParseHelper helper;
			FooSharedData sharedData;

			Assert::IsTrue(helper.EndElementHandler(nullptr, "Foo"));
			Assert::IsTrue(helper.EndElementHandler(&sharedData, "Foo"));
		}

		TEST_METHOD(FooParseHelperCharDataHandler)
		{
			FooParseHelper helper;
			FooSharedData sharedData;

			Assert::IsFalse(helper.CharDataHandler(nullptr, "Test1", 5));

			Assert::IsFalse(helper.CharDataHandler(&sharedData, "Test2", 0));
			Assert::IsFalse(helper.CharDataHandler(&sharedData, "", 5));
			Assert::IsTrue(helper.CharDataHandler(&sharedData, "Test4", 5));
		}

		TEST_METHOD(FooParseHelperClone)
		{
			FooSharedData sharedData;
			FooParseHelper helper, otherHelper;

			FooParseHelper* helperClone = reinterpret_cast<FooParseHelper*>(helper.Clone());
			FooParseHelper* otherHelperClone = reinterpret_cast<FooParseHelper*>(otherHelper.Clone());

			delete(helperClone);
			delete(otherHelperClone);
		}

		TEST_METHOD(FooSharedDataConstructor)
		{
			FooSharedData sharedData;

			Assert::AreEqual(0U, sharedData.Depth());
			Assert::IsTrue(sharedData.GetXmlParseMaster() == nullptr);
		}

		TEST_METHOD(FooSharedDataDestructor)
		{
			FooSharedData sharedData;
			XmlParseMaster master(&sharedData);
			FooParseHelper helper;

			master.AddHelper(&helper);
			master.Parse(sTestFileContents.c_str(), sTestFileContents.length());

			XmlParseMaster::SharedData* clone = sharedData.Clone();
			FooSharedData* otherClone = reinterpret_cast<FooSharedData*>(sharedData.Clone());

			Assert::IsTrue(sharedData.Equals(sTestFileAttributeData, sTestFileCharData));
			Assert::IsTrue(otherClone->Equals(sTestFileAttributeData, sTestFileCharData));

			delete(clone);
			delete(otherClone);
		}

		TEST_METHOD(FooSharedDataClone)
		{
			FooSharedData sharedData;
			XmlParseMaster master;
			FooParseHelper helper;

			FooSharedData* clone = reinterpret_cast<FooSharedData*>(sharedData.Clone());

			Assert::IsTrue(clone->GetXmlParseMaster() == nullptr);

			sharedData.SetXmlParseMaster(&master);
			master.SetSharedData(&sharedData);

			delete(clone);

			master.AddHelper(&helper);
			master.Parse(sTestFileContents.c_str(), sTestFileContents.length());
			clone = reinterpret_cast<FooSharedData*>(sharedData.Clone());

			Assert::IsTrue(clone->GetXmlParseMaster() == &master);
			Assert::IsTrue(sharedData.Equals(sTestFileAttributeData, sTestFileCharData));
			Assert::IsTrue(clone->Equals(sTestFileAttributeData, sTestFileCharData));

			delete(clone);
		}

		TEST_METHOD(FooSharedDataUtilities)
		{
			FooSharedData sharedData;

			sharedData.AddMap("Foo", sTestFileAttributeData["Foo"]);
			sharedData.AddMap("Bar1", sTestFileAttributeData["Bar1"]);
			sharedData.AddMap("Bat1", sTestFileAttributeData["Bat1"]);
			sharedData.AddMap("Bat2", sTestFileAttributeData["Bat2"]);
			sharedData.AddMap("Bar2", sTestFileAttributeData["Bar2"]);
			sharedData.AddMap("Bat3", sTestFileAttributeData["Bat3"]);
			sharedData.AddMap("Bat4", sTestFileAttributeData["Bat4"]);
			sharedData.AddMap("Bar3", sTestFileAttributeData["Bar3"]);
			sharedData.AddMap("Bat5", sTestFileAttributeData["Bat5"]);
			sharedData.AddMap("Bat6", sTestFileAttributeData["Bat6"]);

			sharedData.AddCharData("First Foo Bar Bat");
			sharedData.AddCharData("Second Foo Bar Bat");
			sharedData.AddCharData("Third Foo Bar Bat");
			sharedData.AddCharData("Fourth Foo Bar Bat");
			sharedData.AddCharData("Fifth Foo Bar Bat");
			sharedData.AddCharData("Sixth Foo Bar Bat");

			Assert::IsTrue(sharedData.Equals(sTestFileAttributeData, sTestFileCharData));
		}

		TEST_METHOD(FooSharedDataRTTIMethods)
		{
			FooSharedData sharedData;

			Assert::IsTrue(sharedData.Is(FooSharedData::TypeIdClass()));
			Assert::IsTrue(sharedData.Is(FooSharedData::TypeName()));
			Assert::AreEqual(FooSharedData::TypeIdClass(), sharedData.TypeIdInstance());
			Assert::AreEqual(FooSharedData::TypeName(), sharedData.TypeName());

			RTTI* asRTTI = &sharedData;

			Assert::IsFalse(nullptr == asRTTI->QueryInterface(FooSharedData::TypeIdClass()));
			Assert::AreEqual(asRTTI->As<FooSharedData>()->ToString(), sharedData.ToString());
		}

		TEST_METHOD(SharedDataConstructor)
		{
			XmlParseMaster::SharedData sharedData;

			Assert::AreEqual(0U, sharedData.Depth());
			Assert::IsTrue(sharedData.GetXmlParseMaster() == nullptr);
		}

		TEST_METHOD(SharedDataDestructor)
		{
			XmlParseMaster master;
			XmlParseMaster::SharedData sharedData;

			sharedData.SetXmlParseMaster(&master);

			XmlParseMaster::SharedData* clone = sharedData.Clone();

			delete(clone);
		}

		TEST_METHOD(SharedDataClone)
		{
			XmlParseMaster master;
			XmlParseMaster::SharedData sharedData;

			XmlParseMaster::SharedData* clone = sharedData.Clone();

			Assert::IsTrue(clone->GetXmlParseMaster() == nullptr);

			sharedData.SetXmlParseMaster(&master);
			delete(clone);
			clone = sharedData.Clone();

			Assert::IsTrue(clone->GetXmlParseMaster() == &master);

			delete(clone);
		}

		TEST_METHOD(SharedDataSetXmlParseMaster)
		{
			XmlParseMaster master;
			XmlParseMaster::SharedData sharedData;

			Assert::IsTrue(sharedData.GetXmlParseMaster() == nullptr);

			sharedData.SetXmlParseMaster(&master);

			Assert::IsTrue(sharedData.GetXmlParseMaster() == &master);
		}

		TEST_METHOD(SharedDataGetXmlParseMaster)
		{
			XmlParseMaster master;
			XmlParseMaster::SharedData sharedData;

			Assert::IsTrue(sharedData.GetXmlParseMaster() == nullptr);

			sharedData.SetXmlParseMaster(&master);

			Assert::IsTrue(sharedData.GetXmlParseMaster() == &master);
		}

		TEST_METHOD(SharedDataIncrementDepth)
		{
			XmlParseMaster::SharedData sharedData;

			sharedData.IncrementDepth();

			Assert::AreEqual(1U, sharedData.Depth());

			sharedData.IncrementDepth();

			Assert::AreEqual(2U, sharedData.Depth());
		}

		TEST_METHOD(SharedDataDecrementDepth)
		{
			XmlParseMaster::SharedData sharedData;

			sharedData.IncrementDepth();
			sharedData.IncrementDepth();
			sharedData.DecrementDepth();

			Assert::AreEqual(1U, sharedData.Depth());

			sharedData.DecrementDepth();

			Assert::AreEqual(0U, sharedData.Depth());

			auto decrementDepthPointer = [&sharedData] { sharedData.DecrementDepth(); };

			Assert::ExpectException<std::exception>(decrementDepthPointer);
		}

		TEST_METHOD(SharedDataDepth)
		{
			XmlParseMaster::SharedData sharedData;

			Assert::AreEqual(0U, sharedData.Depth());

			sharedData.IncrementDepth();

			Assert::AreEqual(1U, sharedData.Depth());

			sharedData.IncrementDepth();

			Assert::AreEqual(2U, sharedData.Depth());

			sharedData.DecrementDepth();

			Assert::AreEqual(1U, sharedData.Depth());

			sharedData.DecrementDepth();

			Assert::AreEqual(0U, sharedData.Depth());
		}

		TEST_METHOD(SharedDataRTTIMethods)
		{
			XmlParseMaster::SharedData sharedData;

			Assert::IsTrue(sharedData.Is(XmlParseMaster::SharedData::TypeIdClass()));
			Assert::IsTrue(sharedData.Is(XmlParseMaster::SharedData::TypeName()));
			Assert::AreEqual(XmlParseMaster::SharedData::TypeIdClass(), sharedData.TypeIdInstance());
			Assert::AreEqual(XmlParseMaster::SharedData::TypeName(), sharedData.TypeName());

			RTTI* asRTTI = &sharedData;

			Assert::IsFalse(nullptr == asRTTI->QueryInterface(XmlParseMaster::SharedData::TypeIdClass()));
			Assert::AreEqual(asRTTI->As<XmlParseMaster::SharedData>()->ToString(), sharedData.ToString());
		}


		TEST_METHOD(XmlParseMasterConstructor)
		{
			XmlParseMaster master;
			
			Assert::IsTrue(master.GetFileName() == "");
		}

		TEST_METHOD(XmlParseMasterDestructor)
		{
			FooSharedData sharedData;
			XmlParseMaster master(&sharedData);

			FooParseHelper firstHelper, secondHelper, thirdHelper;
			master.AddHelper(&firstHelper);
			master.AddHelper(&secondHelper);
			master.AddHelper(&thirdHelper);

			const XmlParseMaster* otherClone = master.Clone();
			delete(otherClone);
		}

		TEST_METHOD(XmlParseMasterClone)
		{
			FooSharedData sharedData;
			XmlParseMaster master(&sharedData);
			FooParseHelper helper;

			master.AddHelper(&helper);

			XmlParseMaster* clone = master.Clone();
			
			Assert::IsTrue(&sharedData != clone->GetSharedData());
			Assert::IsTrue(clone->GetSharedData()->GetXmlParseMaster() == clone);

			clone->Parse(sTestFileContents.c_str(), sTestFileContents.length());
			
			FooSharedData* sharedDataClone = clone->GetSharedData()->As<FooSharedData>();

			Assert::IsTrue(sharedDataClone->Equals(sTestFileAttributeData, sTestFileCharData));

			delete(clone);
		}

		TEST_METHOD(XmlParseMasterAddHelper)
		{
			FooSharedData sharedData;
			XmlParseMaster master(&sharedData);
			FooParseHelper helper;

			master.Parse(sTestFileContents.c_str(), sTestFileContents.length());

			Assert::IsFalse(sharedData.Equals(sTestFileAttributeData, sTestFileCharData));

			master.AddHelper(&helper);
			master.Parse(sTestFileContents.c_str(), sTestFileContents.length());

			Assert::IsTrue(sharedData.Equals(sTestFileAttributeData, sTestFileCharData));

			auto addHelperPointer = [&master] { master.AddHelper(nullptr); };

			Assert::ExpectException<std::exception>(addHelperPointer);

			XmlParseMaster* clone = master.Clone();
			auto addHelperToClonePointer = [clone, &helper] { clone->AddHelper(&helper); };

			Assert::ExpectException<std::exception>(addHelperToClonePointer);

			delete(clone);
		}

		TEST_METHOD(XmlParseMasterRemoveHelper)
		{
			FooSharedData sharedData;
			XmlParseMaster master(&sharedData);
			FooParseHelper helper;

			master.AddHelper(&helper);
			master.Parse(sTestFileContents.c_str(), sTestFileContents.length());

			Assert::IsTrue(sharedData.Equals(sTestFileAttributeData, sTestFileCharData));

			FooSharedData otherSharedData;
			master.SetSharedData(reinterpret_cast<XmlParseMaster::SharedData*>(&otherSharedData));
			master.RemoveHelper(&helper);
			master.Parse(sTestFileContents.c_str(), sTestFileContents.length());

			Assert::IsFalse(otherSharedData.Equals(sTestFileAttributeData, sTestFileCharData));

			auto removeHelperPointer = [&master] { master.RemoveHelper(nullptr); };

			Assert::ExpectException<std::exception>(removeHelperPointer);

			XmlParseMaster* clone = master.Clone();
			auto removeHelperToClonePointer = [clone, &helper] { clone->RemoveHelper(&helper); };

			Assert::ExpectException<std::exception>(removeHelperToClonePointer);

			delete(clone);
		}

		TEST_METHOD(XmlParseMasterParse)
		{
			FooSharedData sharedData;
			XmlParseMaster master(&sharedData);

			auto parsePointer = [&master] { master.Parse(sMalformedXml.c_str(), sMalformedXml.length()); };

			Assert::ExpectException<std::exception>(parsePointer);

			master.Parse(sTestFileContents.c_str(), sTestFileContents.length());

			Assert::IsTrue(sharedData.Equals(sEmptyAttributeData, sEmptyCharData));
			Assert::IsTrue(master.GetFileName() == "");

			FooParseHelper helper;
			master.AddHelper(&helper);

			master.Parse(sTestFileContents.c_str(), sTestFileContents.size());

			Assert::IsTrue(sharedData.Equals(sTestFileAttributeData, sTestFileCharData));
			Assert::IsTrue(master.GetFileName() == "");
		}

		TEST_METHOD(XmlParseMasterParseFromFile)
		{
			FooSharedData sharedData;
			XmlParseMaster master(&sharedData);

			auto parseFromFilePointer = [&master] { master.ParseFromFile("FileNotFound.xml"); };

			Assert::ExpectException<std::exception>(parseFromFilePointer);

			master.ParseFromFile("FooParseHelperTest.xml");

			Assert::IsTrue(sharedData.Equals(sEmptyAttributeData, sEmptyCharData));
			Assert::IsTrue(master.GetFileName() == "FooParseHelperTest.xml");

			FooParseHelper helper;
			master.AddHelper(&helper);

			master.ParseFromFile("FooParseHelperTest.xml");

			Assert::IsTrue(sharedData.Equals(sTestFileAttributeData, sTestFileCharData));
			Assert::IsTrue(master.GetFileName() == "FooParseHelperTest.xml");
		}

		TEST_METHOD(XmlParseMasterGetFileName)
		{
			XmlParseMaster master;

			Assert::IsTrue(master.GetFileName() == "");

			master.Parse(sTestFileContents.c_str(), sTestFileContents.length());

			Assert::IsTrue(master.GetFileName() == "");

			master.ParseFromFile("FooParseHelperTest.xml");

			Assert::IsTrue(master.GetFileName() == "FooParseHelperTest.xml");

			master.Parse(sTestFileContents.c_str(), sTestFileContents.length());

			Assert::IsTrue(master.GetFileName() == "FooParseHelperTest.xml");
		}

		TEST_METHOD(XmlParseMasterGetSharedData)
		{
			XmlParseMaster master;

			Assert::IsTrue(master.GetSharedData() == nullptr);

			XmlParseMaster::SharedData* sharedData = new FooSharedData();
			master.SetSharedData(sharedData);

			Assert::IsTrue(master.GetSharedData() == sharedData);
			Assert::IsTrue(sharedData->GetXmlParseMaster() == &master);

			delete(sharedData);
		}

	private:

		static void AddTestFileAttributeData(std::string& elementName, std::string& nameValue);

		static _CrtMemState sStartMemState;

		static HashMap<std::string, HashMap<std::string, std::string>> sTestFileAttributeData;

		static Vector<std::string> sTestFileCharData;

		static HashMap<std::string, HashMap<std::string, std::string>> sEmptyAttributeData;

		static Vector<std::string> sEmptyCharData;

		static std::string sTestFileContents;

		static std::string sMalformedXml;
	};

	void XmlParseMasterTest::AddTestFileAttributeData(std::string& elementName, std::string& nameValue)
	{
		HashMap<std::string, std::string>& map = sTestFileAttributeData[elementName];
		map["name"] = nameValue;
	}

	_CrtMemState XmlParseMasterTest::sStartMemState;

	HashMap<std::string, HashMap<std::string, std::string>> XmlParseMasterTest::sTestFileAttributeData;

	Vector<std::string> XmlParseMasterTest::sTestFileCharData;

	HashMap<std::string, HashMap<std::string, std::string>> XmlParseMasterTest::sEmptyAttributeData;

	Vector<std::string> XmlParseMasterTest::sEmptyCharData;

	std::string XmlParseMasterTest::sTestFileContents;

	std::string XmlParseMasterTest::sMalformedXml;
}
