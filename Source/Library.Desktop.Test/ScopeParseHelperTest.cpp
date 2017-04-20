#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace TestingUtilities;

namespace LibraryDesktopTest
{
	TEST_CLASS(ScopeParseHelperTest)
	{

	public:

		TEST_CLASS_INITIALIZE(InitializeTestClass)
		{
			sTestFileScope.Append("Health") = 10;
			sTestFileScope.Append("Speed") = 2.5f;
			sTestFileScope.Append("Aliases") = "Steals McGee";
			sTestFileScope.Append("Aliases").PushBack("Sanjay");

			Scope& secondScope = sTestFileScope.AppendScope("Powers");
			secondScope.Append("Power Count") = 1;

			Scope& thirdScope = secondScope.AppendScope("Shape Shift");
			thirdScope.Append("Form Count") = 2;
			thirdScope.Append("Forms") = "Spider";
			thirdScope.Append("Forms").PushBack("Sneaky Snake");

			Scope& fourthScope = sTestFileScope.AppendScope("Box");
			fourthScope.Append("Side Length") = 1.0f;

			sTestFileScope.Append("Position") = glm::vec4();
			sTestFileScope.Append("Corner Positions") = glm::mat4(10.0f);
		}

		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			Attributed::ClearPrescribedAttributes();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);

			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}

		TEST_METHOD(ScopeSharedDataConstructor)
		{
			ScopeParseHelper::ScopeSharedData sharedData;

			Assert::IsTrue(sharedData.mScope == nullptr);
		}

		TEST_METHOD(ScopeSharedDataDestructor)
		{
			ScopeParseHelper::ScopeSharedData sharedData;
			sharedData.mScope = new Scope();
			*sharedData.mScope = sTestFileScope;
		}

		TEST_METHOD(ScopeSharedDataInitialize)
		{
			ScopeParseHelper::ScopeSharedData sharedData;

			sharedData.Initialize();

			Assert::IsTrue(sharedData.mScope == nullptr);

			sharedData.mScope = new Scope();
			*sharedData.mScope = sTestFileScope;

			sharedData.Initialize();

			Assert::IsTrue(sharedData.mScope == nullptr);
		}

		TEST_METHOD(ScopeSharedDataClone)
		{
			ScopeParseHelper::ScopeSharedData sharedData;

			XmlParseMaster::SharedData* clone = sharedData.Clone();

			Assert::IsTrue(clone->Is(ScopeParseHelper::ScopeSharedData::TypeIdClass()));

			delete(clone);
		}

		TEST_METHOD(ScopeSharedDataRTTIMethods)
		{
			ScopeParseHelper::ScopeSharedData sharedData;

			Assert::IsTrue(sharedData.Is(ScopeParseHelper::ScopeSharedData::TypeIdClass()));
			Assert::IsTrue(sharedData.Is(ScopeParseHelper::ScopeSharedData::TypeName()));
			Assert::AreEqual(ScopeParseHelper::ScopeSharedData::TypeIdClass(), sharedData.TypeIdInstance());
			Assert::AreEqual(ScopeParseHelper::ScopeSharedData::TypeName(), sharedData.TypeName());

			RTTI* asRTTI = &sharedData;

			Assert::IsFalse(nullptr == asRTTI->QueryInterface(ScopeParseHelper::ScopeSharedData::TypeIdClass()));
			Assert::AreEqual(asRTTI->As<ScopeParseHelper::ScopeSharedData>()->ToString(), sharedData.ToString());
		}

		TEST_METHOD(ScopeParseHelperConstructor)
		{
			ScopeParseHelper helper;
		}

		TEST_METHOD(ScopeParseHelperDestructor)
		{
			ScopeParseHelper helper;
			ScopeParseHelper* helperPointer = new ScopeParseHelper();

			delete(helperPointer);
		}

		TEST_METHOD(ScopeParseHelperInitialize)
		{
			ScopeParseHelper helper;

			helper.Initialize();
		}

		TEST_METHOD(ScopeParseHelperStartElementHandler)
		{
			ScopeParseHelper helper;
			ScopeParseHelper::ScopeSharedData sharedData;
			XmlParseMaster::SharedData invalidSharedData;

			HashMap<std::string, std::string> baseScopeAttributes = { { "name", "Scope 1" }, };
			HashMap<std::string, std::string> integerAttributes = { { "name", "integer" },{ "value", "0" }, };
			HashMap<std::string, std::string> floatAttributes = { { "name", "float" },{ "value", "0.0" }, };
			HashMap<std::string, std::string> vectorAttributes = { { "name", "vector" },{ "value", "0.0,0.0,0.0,0.0," }, };
			HashMap<std::string, std::string> matrixAttributes = { { "name", "matrix" },{ "value", "0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0," }, };
			HashMap<std::string, std::string> stringAttributes = { { "name", "string" },{ "value", "test" }, };
			HashMap<std::string, std::string> worldAttributes = { { "name", "World 1" }, };
			HashMap<std::string, std::string> sectorAttributes = { { "name", "Sector 1" }, };
			HashMap<std::string, std::string> entityAttributes = { { "name", "Entity 1" },{ "class", "Entity" }, };
			HashMap<std::string, std::string> actionAttributes = { { "name", "Action 1" },{ "class", "ActionList" }, };
			HashMap<std::string, std::string> reactionAttributes = { { "name", "Reaction 1" },{ "class", "ReactionAttributed" }, };

			auto integerStartElementPointer = [&helper, &sharedData, &integerAttributes] { return helper.StartElementHandler(&sharedData, "integer", integerAttributes); };
			auto floatStartElementPointer = [&helper, &sharedData, &floatAttributes] { return helper.StartElementHandler(&sharedData, "float", floatAttributes); };
			auto vectorStartElementPointer = [&helper, &sharedData, &vectorAttributes] { return helper.StartElementHandler(&sharedData, "vector", vectorAttributes); };
			auto matrixStartElementPointer = [&helper, &sharedData, &matrixAttributes] { return helper.StartElementHandler(&sharedData, "matrix", matrixAttributes); };
			auto stringStartElementPointer = [&helper, &sharedData, &stringAttributes] { return helper.StartElementHandler(&sharedData, "string", stringAttributes); };
			auto scopeStartElementPointer = [&helper, &sharedData, &baseScopeAttributes] { return helper.StartElementHandler(&sharedData, "scope", baseScopeAttributes); };
			auto worldStartElementPointer = [&helper, &sharedData, &worldAttributes] { return helper.StartElementHandler(&sharedData, "world", worldAttributes); };
			auto sectorStartElementPointer = [&helper, &sharedData, &sectorAttributes] { return helper.StartElementHandler(&sharedData, "sector", sectorAttributes); };
			auto entityStartElementPointer = [&helper, &sharedData, &entityAttributes] { return helper.StartElementHandler(&sharedData, "entity", entityAttributes); };
			auto actionStartElementPointer = [&helper, &sharedData, &actionAttributes] { return helper.StartElementHandler(&sharedData, "action", actionAttributes); };
			auto ifStartElementPointer = [&helper, &sharedData, &actionAttributes] { return helper.StartElementHandler(&sharedData, "if", actionAttributes); };
			auto thenStartElementPointer = [&helper, &sharedData, &actionAttributes] { return helper.StartElementHandler(&sharedData, "then", actionAttributes); };
			auto elseStartElementPointer = [&helper, &sharedData, &actionAttributes] { return helper.StartElementHandler(&sharedData, "else", actionAttributes); };
			auto expressionStartElementPointer = [&helper, &sharedData, &actionAttributes] { return helper.StartElementHandler(&sharedData, "expression", actionAttributes); };
			auto reactionStartElementPointer = [&helper, &sharedData, &reactionAttributes] { return helper.StartElementHandler(&sharedData, "reaction", reactionAttributes); };

			Assert::IsFalse(helper.StartElementHandler(nullptr, "", baseScopeAttributes));
			Assert::IsFalse(helper.StartElementHandler(&invalidSharedData, "", baseScopeAttributes));
			Assert::IsFalse(helper.StartElementHandler(&sharedData, "", baseScopeAttributes));
			Assert::ExpectException<std::exception>(integerStartElementPointer);
			Assert::ExpectException<std::exception>(floatStartElementPointer);
			Assert::ExpectException<std::exception>(vectorStartElementPointer);
			Assert::ExpectException<std::exception>(matrixStartElementPointer);
			Assert::ExpectException<std::exception>(stringStartElementPointer);
			Assert::ExpectException<std::exception>(sectorStartElementPointer);
			Assert::ExpectException<std::exception>(entityStartElementPointer);
			Assert::ExpectException<std::exception>(actionStartElementPointer);
			Assert::ExpectException<std::exception>(ifStartElementPointer);
			Assert::ExpectException<std::exception>(thenStartElementPointer);
			Assert::ExpectException<std::exception>(elseStartElementPointer);
			Assert::ExpectException<std::exception>(expressionStartElementPointer);
			Assert::ExpectException<std::exception>(reactionStartElementPointer);
			Assert::IsTrue(scopeStartElementPointer());
			Assert::ExpectException<std::exception>(worldStartElementPointer);
			Assert::IsTrue(integerStartElementPointer());
			Assert::ExpectException<std::exception>(scopeStartElementPointer);

			helper.EndElementHandler(&sharedData, "integer");

			Assert::IsTrue(floatStartElementPointer());

			helper.EndElementHandler(&sharedData, "float");

			Assert::IsTrue(vectorStartElementPointer());

			helper.EndElementHandler(&sharedData, "vector");

			Assert::IsTrue(matrixStartElementPointer());

			helper.EndElementHandler(&sharedData, "matrix");

			Assert::IsTrue(stringStartElementPointer());

			helper.EndElementHandler(&sharedData, "string");

			Assert::IsTrue(scopeStartElementPointer());

			helper.EndElementHandler(&sharedData, "scope");
			helper.EndElementHandler(&sharedData, "scope");

			Assert::AreEqual(0U, sharedData.Depth());
		}

		TEST_METHOD(ScopeParseHelplerEndElementHandler)
		{
			ScopeParseHelper helper;
			ScopeParseHelper::ScopeSharedData sharedData;
			XmlParseMaster::SharedData invalidSharedData;

			HashMap<std::string, std::string> baseScopeAttributes = { { "name", "Scope 1" }, };
			HashMap<std::string, std::string> integerAttributes = { { "name", "integer" },{ "value", "0" }, };
			HashMap<std::string, std::string> floatAttributes = { { "name", "float" },{ "value", "0.0" }, };
			HashMap<std::string, std::string> vectorAttributes = { { "name", "vector" },{ "value", "0.0,0.0,0.0,0.0," }, };
			HashMap<std::string, std::string> matrixAttributes = { { "name", "matrix" },{ "value", "0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0," }, };
			HashMap<std::string, std::string> stringAttributes = { { "name", "string" },{ "value", "test" }, };
			HashMap<std::string, std::string> worldAttributes = { { "name", "World 1" }, };
			HashMap<std::string, std::string> sectorAttributes = { { "name", "Sector 1" }, };
			HashMap<std::string, std::string> entityAttributes = { { "name", "Entity 1" },{ "class", "Entity" }, };
			HashMap<std::string, std::string> actionAttributes = { { "name", "Action 1" },{ "class", "ActionList" }, };
			HashMap<std::string, std::string> reactionAttributes = { { "name", "Reaction 1" },{ "class", "ReactionAttributed" }, };

			auto integerEndElementPointer = [&helper, &sharedData] { return helper.EndElementHandler(&sharedData, "integer"); };
			auto floatEndElementPointer = [&helper, &sharedData] { return helper.EndElementHandler(&sharedData, "float"); };
			auto vectorEndElementPointer = [&helper, &sharedData] { return helper.EndElementHandler(&sharedData, "vector"); };
			auto matrixEndElementPointer = [&helper, &sharedData] { return helper.EndElementHandler(&sharedData, "matrix"); };
			auto stringEndElementPointer = [&helper, &sharedData] { return helper.EndElementHandler(&sharedData, "string"); };
			auto worldEndElementPointer = [&helper, &sharedData] { return helper.EndElementHandler(&sharedData, "world"); };
			auto sectorEndElementPointer = [&helper, &sharedData] { return helper.EndElementHandler(&sharedData, "sector"); };
			auto entityEndElementPointer = [&helper, &sharedData] { return helper.EndElementHandler(&sharedData, "entity"); };
			auto actionEndElementPointer = [&helper, &sharedData] { return helper.EndElementHandler(&sharedData, "action"); };
			auto ifEndElementPointer = [&helper, &sharedData] { return helper.EndElementHandler(&sharedData, "if"); };
			auto expressionEndElementPointer = [&helper, &sharedData] { return helper.EndElementHandler(&sharedData, "expression"); };
			auto reactionEndElementPointer = [&helper, &sharedData] { return helper.EndElementHandler(&sharedData, "reaction"); };

			Assert::IsFalse(helper.EndElementHandler(nullptr, ""));
			Assert::IsFalse(helper.EndElementHandler(&invalidSharedData, ""));
			Assert::IsFalse(helper.EndElementHandler(&sharedData, ""));

			helper.StartElementHandler(&sharedData, "scope", baseScopeAttributes);

			Assert::ExpectException<std::exception>(integerEndElementPointer);
			Assert::ExpectException<std::exception>(floatEndElementPointer);
			Assert::ExpectException<std::exception>(vectorEndElementPointer);
			Assert::ExpectException<std::exception>(matrixEndElementPointer);
			Assert::ExpectException<std::exception>(stringEndElementPointer);
			Assert::ExpectException<std::exception>(worldEndElementPointer);
			Assert::ExpectException<std::exception>(sectorEndElementPointer);
			Assert::ExpectException<std::exception>(entityEndElementPointer);
			Assert::ExpectException<std::exception>(actionEndElementPointer);
			Assert::ExpectException<std::exception>(ifEndElementPointer);
			Assert::ExpectException<std::exception>(expressionEndElementPointer);
			Assert::ExpectException<std::exception>(reactionEndElementPointer);

			helper.StartElementHandler(&sharedData, "integer", integerAttributes);

			auto scopeEndElementPointer = [&helper, &sharedData] { return helper.EndElementHandler(&sharedData, "scope"); };

			Assert::ExpectException<std::exception>(scopeEndElementPointer);
			Assert::IsTrue(integerEndElementPointer());

			helper.StartElementHandler(&sharedData, "float", floatAttributes);

			Assert::IsTrue(floatEndElementPointer());

			helper.StartElementHandler(&sharedData, "vector", vectorAttributes);

			Assert::IsTrue(vectorEndElementPointer());

			helper.StartElementHandler(&sharedData, "matrix", matrixAttributes);

			Assert::IsTrue(matrixEndElementPointer());

			helper.StartElementHandler(&sharedData, "string", stringAttributes);

			Assert::IsTrue(stringEndElementPointer());
			Assert::IsTrue(scopeEndElementPointer());
		}

		TEST_METHOD(ScopeParseHandlerCharDataHandler)
		{
			ScopeParseHelper helper;
			ScopeParseHelper::ScopeSharedData sharedData;
			XmlParseMaster::SharedData invalidSharedData;

			Assert::IsFalse(helper.CharDataHandler(nullptr, "", 0));
			Assert::IsFalse(helper.CharDataHandler(&invalidSharedData, "", 0));
			Assert::IsTrue(helper.CharDataHandler(&sharedData, "", 0));
		}

		TEST_METHOD(ScopeParseHelperClone)
		{
			ScopeParseHelper helper;

			IXmlParseHelper* clone = helper.Clone();

			ScopeParseHelper* asScopeParseHelper = dynamic_cast<ScopeParseHelper*>(clone);

			Assert::IsTrue(asScopeParseHelper != nullptr);

			delete(clone);
		}

		TEST_METHOD(ScopeParseHelperFunctionalTest)
		{
			ScopeParseHelper::ScopeSharedData sharedData;
			XmlParseMaster master(&sharedData);
			ScopeParseHelper helper;

			master.AddHelper(&helper);

			master.ParseFromFile(sTestFilePath);

			Assert::IsTrue(*sharedData.mScope == sTestFileScope);

			Assert::IsTrue(*sTestFileScope.Find("Health") == 10);
			Assert::IsTrue(*sTestFileScope.Find("Speed") == 2.5f);
			Assert::IsTrue(sTestFileScope.Find("Aliases")->Get<std::string&>(0U) == "Steals McGee");
			Assert::IsTrue(sTestFileScope.Find("Aliases")->Get<std::string&>(1U) == "Sanjay");

			Scope& testNestedScope = sTestFileScope.Find("Powers")->Get<Scope&>();
			Assert::IsTrue(testNestedScope.GetParent() == &sTestFileScope);
			Assert::IsTrue(*testNestedScope.Find("Power Count") == 1);

			Scope& thirdTestScope = testNestedScope.Find("Shape Shift")->Get<Scope&>();
			Assert::IsTrue(thirdTestScope.GetParent() == &testNestedScope);
			Assert::IsTrue(*thirdTestScope.Find("Form Count") == 2);
			Assert::IsTrue(thirdTestScope.Find("Forms")->Get<std::string&>(0U) == "Spider");
			Assert::IsTrue(thirdTestScope.Find("Forms")->Get<std::string&>(1U) == "Sneaky Snake");

			Scope& fourthTestScope = sTestFileScope.Find("Box")->Get<Scope&>();
			Assert::IsTrue(*fourthTestScope.Find("Side Length") == 1.0f);

			Assert::IsTrue(*sTestFileScope.Find("Position") == glm::vec4());
			Assert::IsTrue(*sTestFileScope.Find("Corner Positions") == glm::mat4(10.0f));

			Assert::AreEqual(0U, sharedData.Depth());

			Assert::IsTrue(*sharedData.mScope->Find("Health") == 10);
			Assert::IsTrue(*sharedData.mScope->Find("Speed") == 2.5f);
			Assert::IsTrue(sharedData.mScope->Find("Aliases")->Get<std::string&>(0U) == "Steals McGee");
			Assert::IsTrue(sharedData.mScope->Find("Aliases")->Get<std::string&>(1U) == "Sanjay");

			Scope& nestedScope = sharedData.mScope->Find("Powers")->Get<Scope&>();
			Assert::IsTrue(nestedScope.GetParent() == sharedData.mScope);
			Assert::IsTrue(*nestedScope.Find("Power Count") == 1);

			Scope& thirdScope = nestedScope.Find("Shape Shift")->Get<Scope&>();
			Assert::IsTrue(thirdScope.GetParent() == &nestedScope);
			Assert::IsTrue(*thirdScope.Find("Form Count") == 2);
			Assert::IsTrue(thirdScope.Find("Forms")->Get<std::string&>(0U) == "Spider");
			Assert::IsTrue(thirdScope.Find("Forms")->Get<std::string&>(1U) == "Sneaky Snake");

			Scope& fourthScope = sharedData.mScope->Find("Box")->Get<Scope&>();
			Assert::IsTrue(*fourthScope.Find("Side Length") == 1.0f);

			Assert::IsTrue(*sharedData.mScope->Find("Position") == glm::vec4());
			Assert::IsTrue(*sharedData.mScope->Find("Corner Positions") == glm::mat4(10.0f));
		}

		TEST_METHOD(ScopeParseHelperWorldFunctionalTest)
		{
			ScopeParseHelper::ScopeSharedData sharedData;
			XmlParseMaster master(&sharedData);
			ScopeParseHelper helper;

			master.AddHelper(&helper);

			master.ParseFromFile(sWorldTestFilePath);

			World& world = *sharedData.mScope->As<World>();

			World otherWorld;
			otherWorld.SetName("World 1");
			otherWorld.Append("Sector Count") = 2;

			Sector* sectorOne = otherWorld.CreateSector("Sector 1");
			sectorOne->Append("Entity Count") = 2;

			Entity* entityOne = sectorOne->CreateEntity("Entity", "Entity 1");
			entityOne->AppendScope("Character 1") = sTestFileScope;
			sectorOne->CreateEntity("Entity", "Entity 2");
			sectorOne->AppendScope("Character 1") = sTestFileScope;
			otherWorld.CreateSector("Sector 2");
			otherWorld.AppendScope("Character 1") = sTestFileScope;

			Assert::IsTrue(world == otherWorld);
			Assert::IsTrue(world.Name() == "World 1");
			Assert::IsTrue(*world.Find("Sector Count") == 2);

			Sector& sector = *world.Sectors().Get<Scope&>().As<Sector>();
			Assert::IsTrue(sector.Name() == "Sector 1");
			Assert::IsTrue(*sector.Find("Entity Count") == 2);

			Entity& entity = *sector.Entities().Get<Scope&>().As<Entity>();
			Assert::IsTrue(entity.Name() == "Entity 1");
			Assert::IsTrue(entity.Find("Character 1")->Get<Scope&>() == sTestFileScope);

			Entity& otherEntity = *sector.Entities().Get<Scope&>(1).As<Entity>();
			Assert::IsTrue(otherEntity.Name() == "Entity 2");
			Assert::IsTrue(sector.Find("Character 1")->Get<Scope&>() == sTestFileScope);

			Sector& otherSector = *world.Sectors().Get<Scope&>(1).As<Sector>();
			Assert::IsTrue(otherSector.Name() == "Sector 2");
			Assert::IsTrue(world.Find("Character 1")->Get<Scope&>() == sTestFileScope);

			Action* actionOne = entityOne->CreateAction("ActionList", "Action 1");
			actionOne->AppendScope("Character 1") = sTestFileScope;
			actionOne->Append("Power Word") = "Wizzle";
			actionOne->Append("Duration") = 2.0f;
			entityOne->CreateAction("ActionList", "Action 2");

			master.ParseFromFile(sActionTestFilePath);

			World& actionWorld = *sharedData.mScope->As<World>();

			Assert::IsTrue(actionWorld == otherWorld);
		}

		TEST_METHOD(ScopeParseHelperIfFunctionalTest)
		{
			ScopeParseHelper::ScopeSharedData sharedData;
			XmlParseMaster master(&sharedData);
			ScopeParseHelper helper;
			WorldState worldState;

			master.AddHelper(&helper);

			master.ParseFromFile(sIfTestFilePath);

			World& world = *sharedData.mScope->As<World>();

			Sector* sectorOne = world.Sectors().Get<Scope&>(0).As<Sector>();
			ActionListIf* sectorOneIf = sectorOne->Find(Action::sActionsKey)->Get<Scope&>().As<ActionListIf>();

			sectorOneIf->Update(worldState);

			Assert::IsTrue(worldState.mAction == sectorOneIf);

			Sector* sectorTwo = world.Sectors().Get<Scope&>(1).As<Sector>();
			ActionListIf* sectorTwoIf = sectorTwo->Find(Action::sActionsKey)->Get<Scope&>().As<ActionListIf>();
			ActionList* sectorTwoThenContents = sectorTwoIf->Actions().Get<Scope&>(1).As<ActionList>()->Actions().Get<Scope&>().As<ActionList>();

			sectorTwoIf->Update(worldState);

			Assert::IsTrue(worldState.mAction == sectorTwoThenContents);

			Sector* sectorThree = world.Sectors().Get<Scope&>(2).As<Sector>();
			ActionListIf* sectorThreeIf = sectorThree->Find(Action::sActionsKey)->Get<Scope&>().As<ActionListIf>();
			ActionList* sectorThreeElseContents = sectorThreeIf->Actions().Get<Scope&>(2).As<ActionList>()->Actions().Get<Scope&>().As<ActionList>();

			sectorThreeIf->Update(worldState);

			Assert::IsTrue(worldState.mAction == sectorThreeElseContents);

			Sector* sectorFour = world.Sectors().Get<Scope&>(3).As<Sector>();
			ActionListIf* sectorFourIf = sectorFour->Find(Action::sActionsKey)->Get<Scope&>().As<ActionListIf>();
			ActionList* sectorFourThenContents = sectorFourIf->Actions().Get<Scope&>(1).As<ActionList>()->Actions().Get<Scope&>().As<ActionList>();

			sectorFourIf->Update(worldState);

			Assert::IsTrue(worldState.mAction == sectorFourThenContents);

			Sector* sectorFive = world.Sectors().Get<Scope&>(4).As<Sector>();
			ActionListIf* sectorFiveIf = sectorFive->Find(Action::sActionsKey)->Get<Scope&>().As<ActionListIf>();
			ActionList* sectorFiveElseContents = sectorFiveIf->Actions().Get<Scope&>(1).As<ActionList>()->Actions().Get<Scope&>().As<ActionList>();

			sectorFiveIf->Update(worldState);

			Assert::IsTrue(worldState.mAction == sectorFiveElseContents);

			Sector* sectorSix = world.Sectors().Get<Scope&>(5).As<Sector>();
			ActionListIf* sectorSixIf = sectorSix->Find(Action::sActionsKey)->Get<Scope&>().As<ActionListIf>();

			sectorSixIf->Update(worldState);

			Assert::IsTrue(worldState.mAction == sectorSixIf);
		}

		TEST_METHOD(ScopeParseHelperCreateDestroyFunctionalTest)
		{
			ScopeParseHelper::ScopeSharedData sharedData;
			XmlParseMaster master(&sharedData);
			ScopeParseHelper helper;
			WorldState worldState;

			master.AddHelper(&helper);
			master.ParseFromFile(sCreateDestroyTestFilePath);

			World& world = *sharedData.mScope->As<World>();
			ActionListIf* ifAction = world.Actions().Get<Scope&>().As<ActionListIf>();
			ActionList* thenAction = ifAction->ThenAction()->As<ActionList>();
			ActionList* elseAction = ifAction->ElseAction()->As<ActionList>();

			ifAction->Update(worldState);

			Assert::AreEqual(2U, thenAction->Actions().Size());
			Assert::AreEqual(2U, elseAction->Actions().Size());

			ActionExpression* expression = ifAction->FindAction(ActionExpression::sExpressionKey)->As<ActionExpression>();
			expression->SetExpression("false");

			ifAction->Update(worldState);

			Assert::AreEqual(2U, thenAction->Actions().Size());
			Assert::AreEqual(1U, elseAction->Actions().Size());
		}

		TEST_METHOD(ScopeParseHelperExpressionFunctionalTest)
		{
			ScopeParseHelper::ScopeSharedData sharedData;
			XmlParseMaster master(&sharedData);
			ScopeParseHelper helper;
			WorldState worldState;

			master.AddHelper(&helper);
			master.ParseFromFile(sExpressionFilePath);

			World& world = *sharedData.mScope->As<World>();
			ActionListIf* ifAction = world.Actions().Get<Scope&>().As<ActionListIf>();
			ActionList* thenAction = ifAction->ThenAction()->As<ActionList>();
			ActionList* elseAction = ifAction->ElseAction()->As<ActionList>();

			ifAction->Update(worldState);

			Assert::AreEqual(2U, thenAction->Actions().Size());
			Assert::AreEqual(2U, elseAction->Actions().Size());

			world["number"] = 2.0f;

			ifAction->Update(worldState);

			Assert::AreEqual(2U, thenAction->Actions().Size());
			Assert::AreEqual(1U, elseAction->Actions().Size());
		}

		TEST_METHOD(ScopeParseHelperReactionFunctionalTest)
		{
			ScopeParseHelper::ScopeSharedData sharedData;
			XmlParseMaster master(&sharedData);
			ScopeParseHelper helper;
			WorldState worldState;

			master.AddHelper(&helper);
			master.ParseFromFile(sReactionFilePath);

			World& world = *sharedData.mScope->As<World>();

			world.Update(worldState);

			Assert::AreEqual(1U, world.QueueSize());
			Assert::AreEqual(1U, world.Actions().Size());

			worldState.SetGameTime(worldState.GetGameTime() + std::chrono::milliseconds(10));
			world.Update(worldState);

			Assert::AreEqual(0U, world.QueueSize());
			Assert::AreEqual(12, world.Reactions().Get<Scope&>()["argument"].Get<std::int32_t&>());
		}

	private:

		static _CrtMemState sStartMemState;

		static const std::string sTestFilePath;

		static const std::string sWorldTestFilePath;

		static const std::string sActionTestFilePath;

		static const std::string sIfTestFilePath;

		static const std::string sCreateDestroyTestFilePath;

		static const std::string sExpressionFilePath;

		static const std::string sReactionFilePath;

		static Scope sTestFileScope;

		EntityFactory mEntityFactory;

		ActionListFactory mActionListFactory;

		ActionListIfFactory mActionListIfFactory;

		ActionCreateActionFactory mActionCreateActionFactory;

		ActionDestroyActionFactory mActionDestroyActionFactory;

		ActionExpressionFactory mActionExpressionFactory;

		ActionEventFactory mActionEventFactory;

		ReactionAttributedFactory mReactionAttributedFactory;
	};

	_CrtMemState ScopeParseHelperTest::sStartMemState;

	const std::string ScopeParseHelperTest::sTestFilePath = "ScopeParseHelperTest.xml";

	const std::string ScopeParseHelperTest::sWorldTestFilePath = "WorldParseHelperTest.xml";

	const std::string ScopeParseHelperTest::sActionTestFilePath = "ActionParseHelperTest.xml";

	const std::string ScopeParseHelperTest::sIfTestFilePath = "IfParseHelperTest.xml";

	const std::string ScopeParseHelperTest::sCreateDestroyTestFilePath = "CreateDestroyParseTest.xml";

	const std::string ScopeParseHelperTest::sExpressionFilePath = "ExpressionParseTest.xml";

	const std::string ScopeParseHelperTest::sReactionFilePath = "ReactionParseHelperTest.xml";

	Scope ScopeParseHelperTest::sTestFileScope;
}
