#pragma once

#include "IXmlParseHelper.h"
#include "XmlParseMaster.h"
#include "Scope.h"
#include "SList.h"

namespace FieaGameEngine
{
	/** ScopeParseHelper Class
	 *	Concrete implementation of IXmlParseHelper used to parse Scope instances from files.
	 */
	class ScopeParseHelper : public IXmlParseHelper
	{

	public:

		/** ScopeParseHelper State Enumeration mState
		 *	Specifies the states the parse helper can be in while parsing a Scope.
		 */
		enum class State
		{
			NotParsing,
			ParsingScope,
			ParsingInteger,
			ParsingFloat,
			ParsingVector,
			ParsingMatrix,
			ParsingString,
			ParsingWorld,
			ParsingSector,
			ParsingEntity,
			ParsingAction,
			ParsingIf,
			ParsingExpression,
			ParsingReaction,
		};

		/** ScopeParseHelper::ScopeSharedData Class
		 *	Implementation of the SharedData class that publicly stores a pointer to a Scope.
		 */
		class ScopeSharedData : public XmlParseMaster::SharedData
		{

			RTTI_DECLARATIONS(ScopeSharedData, RTTI);

		public:

			/** ScopeSharedData Constructor
			 *	A constructor that initializes the Scope pointer to nullptr.
			 */
			ScopeSharedData();

			/** ScopeSharedData Destructor
			 *	Virtual destructor that deletes the Scope pointer within if it has been set.
			 */
			virtual ~ScopeSharedData();

			/** ScopeSharedData Initialize
			 *	Virtual function used to initialize the members of this class for usage in parsing.
			 */
			virtual void Initialize();

			/** ScopeSharedData Clone
			 *	Virtual constructor that returns a pointer to a new ScopeSharedData.
			 *	@return an XmlParseMaster::SharedData pointer that contains the new ScopeSharedData.
			 */
			virtual XmlParseMaster::SharedData* Clone() const;

			/** ScopeSharedData PushState
			 *	Pushes the given State onto the SharedData stack.
			 *	@param state the State of the parsing to be pushed.
			 */
			void PushState(State state);

			/** ScopeSharedData PeekState
			 *	Peeks the current top of the SharedData stack.
			 *	@return the State that is on top of the SharedData stack.
			 */
			State PeekState();

			/** ScopeSharedData PopState
			 *	Pops the current top off of the SharedData stack.
			 */
			void PopState();

			/** ScopeSharedData mScope
			 *	A public Scope pointer that contains the scope currently being parsed from XML.
			 */
			Scope* mScope;

		private:

			SList<State> mStateStack;
		};

		/** ScopeParseHelper Constructor
		 *	Constructs a ScopeParseHelper in the NotParsing state.
		 */
		ScopeParseHelper();

		/** ScopeParseHelper destructor
		 *	Virtual destructor that calls the parent destructor.
		 */
		virtual ~ScopeParseHelper();

		/** ScopeParseHelper Initialize
		 *	Initializes the state of the ScopeParseHelper to the NotParsing state so it can be used to parse Scope information from XML.
		 */
		virtual void Initialize();

		/** ScopeParseHelper StartElementHandler
		 *	Handles the attributes and names of an XML element being parsed, or returns false.
		 *	@exception will fail if the entered element does not conform to the current state of parsing and expected grammar.
		 *	@param sharedData an XmlParseMaster::SharedData pointer pointing to the SharedData to append information into.
		 *	@param name a constant string reference containing the name of the XML element being parsed.
		 *	@param attributes a reference to a HashMap of string-string pairs representing the attributes of the XML element being parsed.
		 *	@return a boolean representing whether the element was started successfully.
		 */
		virtual bool StartElementHandler(XmlParseMaster::SharedData* sharedData, const std::string& name, HashMap<std::string, std::string>& attributes) const;

		/** ScopeParseHelper EndElementHandler
		 *	Handles the closing of an XML element being parsed, or returns false.
		 *	@exception will fail if the entered element does not conform to the current state of parsing and expected grammar.
		 *	@param sharedData an XmlParseMaster::SharedData pointer pointing to the SharedData to append information into.
		 *	@param name a constant string reference containing the name of the XML element being parsed.
		 *	@return a boolean representing whether the element was closed successfully.
		 */
		virtual bool EndElementHandler(XmlParseMaster::SharedData* sharedData, const std::string& name) const;

		/** ScopeParseHelper CharDataHandler
		 *	Handles the character data within an XML element being parsed, or returns false.
		 *	@param sharedData an XmlParseMaster::SharedData pointer pointing to the SharedData to append information into.
		 *	@param buffer a constant string reference containing the character data of the XML element being parsed.
		 *	@param length a constant unsigned integer representing the length of the character buffer provided.
		 *	@return a boolean representing whether the element's character data was handled successfully.
		 */
		virtual bool CharDataHandler(XmlParseMaster::SharedData* sharedData, const std::string& buffer, const std::uint32_t length) const;

		/** ScopeParseHelper Clone
		 *	Virtual constructor for ScopeParseHelpers that returns a new ScopeParseHelper as an IXmlParseHelper pointer.
		 *	@return an IXmlParseHelper pointer to a new ScopeParseHelper.
		 */
		virtual IXmlParseHelper* Clone() const;

	private:

		/** ScopeParseHelper PrimitiveStartElementAction
		 *	Parses the given information as primitive element.
		 *	@exception will fail if the entered element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData the Scope element will be appended to.
		 *	@param attributes a HashMap<std::string, std::string> reference containing the attribute values of the element being parsed.
		 */
		void PrimitiveStartElementAction(const std::string& element, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes);

		/** ScopeParseHelper ScopeStartElementAction
		 *	Parses the given information as a scope element.
		 *	@exception will fail if the entered element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData the Scope element will be appended to.
		 *	@param attributes a HashMap<std::string, std::string> reference containing the attribute values of the element being parsed.
		 */
		void ScopeStartElementAction(const std::string& element, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes);

		/** ScopeParseHelper WorldStartElementAction
		 *	Parses the given information as a world element.
		 *	@exception will fail if the entered element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData the Scope element will be appended to.
		 *	@param attributes a HashMap<std::string, std::string> reference containing the attribute values of the element being parsed.
		 */
		void WorldStartElementAction(const std::string& element, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes);

		/** ScopeParseHelper SectorStartElementAction
		 *	Parses the given information as a sector element.
		 *	@exception will fail if the entered element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData the Scope element will be appended to.
		 *	@param attributes a HashMap<std::string, std::string> reference containing the attribute values of the element being parsed.
		 */
		void SectorStartElementAction(const std::string& element, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes);

		/** ScopeParseHelper EntityStartElementAction
		 *	Parses the given information as an entity element.
		 *	@exception will fail if the entered element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData the Scope element will be appended to.
		 *	@param attributes a HashMap<std::string, std::string> reference containing the attribute values of the element being parsed.
		 */
		void EntityStartElementAction(const std::string& element, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes);

		/** ScopeParseHelper ActionStartElementAction
		 *	Parses the given information as an action element.
		 *	@exception will fail if the entered element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData the Scope element will be appended to.
		 *	@param attributes a HashMap<std::string, std::string> reference containing the attribute values of the element being parsed.
		 */
		void ActionStartElementAction(const std::string& element,ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes);

		/** ScopeParseHelper IfStartElementAction
		 *	Parses the given information as an if element.
		 *	@exception will fail if the entered element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData the Scope element will be appended to.
		 *	@param attributes a HashMap<std::string, std::string> reference containing the attribute values of the element being parsed.
		 */
		void IfStartElementAction(const std::string& element, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes);

		/** ScopeParseHelper ThenStartElementAction
		 *	Parses the given information as a then element.
		 *	@exception will fail if the entered element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData the Scope element will be appended to.
		 *	@param attributes a HashMap<std::string, std::string> reference containing the attribute values of the element being parsed.
		 */
		void ThenStartElementAction(const std::string& element, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes);

		/** ScopeParseHelper ElseStartElementAction
		 *	Parses the given information as an else element.
		 *	@exception will fail if the entered element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData the Scope element will be appended to.
		 *	@param attributes a HashMap<std::string, std::string> reference containing the attribute values of the element being parsed.
		 */
		void ElseStartElementAction(const std::string& element, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes);

		/** ScopeParseHelper ExpressionStartElementAction
		 *	Parses the given information as an expression element.
		 *	@exception will fail if the entered element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData the Scope element will be appended to.
		 *	@param attributes a HashMap<std::string, std::string> reference containing the attribute values of the element being parsed.
		 */
		void ExpressionStartElementAction(const std::string& element, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes);

		/** ScopeParseHelper ReactionStartElementAction
		 *	Parses the given information as a reaction element.
		 *	@exception will fail if the entered element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData the Scope element will be appended to.
		 *	@param attributes a HashMap<std::string, std::string> reference containing the attribute values of the element being parsed.
		 */
		void ReactionStartElementAction(const std::string& element, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes);

		/** ScopeParseHelper PrimitiveEndElementAction
		 *	Changes the state of this ScopeParseHelper from a primitive to the parent parsing type of the completed element.
		 *	@exception will fail if the closing element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData that contains the constructed Scope.
		 */
		void PrimitiveEndElementAction(const std::string& element, ScopeSharedData* sharedData);

		/** ScopeParseHelper ScopeEndElementAction
		 *	Changes the state of this ScopeParseHelper from scope to the parent parsing type of the completed element.
		 *	If the scope being closed was the base scope, sets the parsing state to NotParsing.
		 *	@exception will fail if the closing element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData that contains the constructed Scope.
		 */
		void ScopeEndElementAction(const std::string& element, ScopeSharedData* sharedData);

		/** ScopeParseHelper WorldEndElementAction
		 *	Changes the state of this ScopeParseHelper from world to the parent parsing type of the completed element.
		 *	@exception will fail if the closing element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData that contains the constructed Scope.
		 */
		void WorldEndElementAction(const std::string& element, ScopeSharedData* sharedData);

		/** ScopeParseHelper SectorEndElementAction
		 *	Changes the state of this ScopeParseHelper from sector to the parent parsing type of the completed element.
		 *	@exception will fail if the closing element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData that contains the constructed Scope.
		 */
		void SectorEndElementAction(const std::string& element, ScopeSharedData* sharedData);

		/** ScopeParseHelper EntityEndElementAction
		 *	Changes the state of this ScopeParseHelper from entity to the parent parsing type of the completed element.
		 *	@exception will fail if the closing element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData that contains the constructed Scope.
		 */
		void EntityEndElementAction(const std::string& element, ScopeSharedData* sharedData);

		/** ScopeParseHelper ActionEndElementAction
		 *	Changes the state of this ScopeParseHelper from action to the parent parsing type of the completed element.
		 *	@exception will fail if the closing element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData that contains the constructed Scope.
		 */
		void ActionEndElementAction(const std::string& element, ScopeSharedData* sharedData);

		/** ScopeParseHelper IfEndElementAction
		 *	Changes the state of this ScopeParseHelper from if to the parent parsing type of the completed element.
		 *	@exception will fail if the closing element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData that contains the constructed Scope.
		 */
		void IfEndElementAction(const std::string& element, ScopeSharedData* sharedData);

		/** ScopeParseHelper ExpressionEndElementAction
		 *	Changes the state of this ScopeParseHelper from expression to the parent parsing type of the completed element.
		 *	@exception will fail if the closing element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData that contains the constructed Scope.
		 */
		void ExpressionEndElementAction(const std::string& element, ScopeSharedData* sharedData);

		/** ScopeParseHelper ReactionEndElementAction
		 *	Changes the state of this ScopeParseHelper from reaction to the parent parsing type of the completed element.
		 *	@exception will fail if the closing element does not conform to the current state of parsing and expected grammar.
		 *	@param element a constant string reference representing the name of the element tag being parsed.
		 *	@param sharedData a ScopeSharedData pointer pointing to the SharedData that contains the constructed Scope.
		 */
		void ReactionEndElementAction(const std::string& element, ScopeSharedData* sharedData);

		/** ScopeParseHelper AllowsForDataMembers
		 *	Determines whether the current state allows for primitive data to be appended to the current scope.
		 *	@return a boolean representing whether the current state allows for primitive data to be appended.
		 */
		bool AllowsForDataMembers(ScopeSharedData* sharedData);

		/** ScopeParseHelper StartElementAction Type Definition
		 *	Defines StartElementAction function signatures for function pointer mappings.
		 */
		typedef void(ScopeParseHelper::*StartElementAction)(const std::string& element, ScopeSharedData* sharedData, HashMap<std::string, std::string>& attributes);

		/** ScopeParseHelper sStartElementActions
		 *	A static constant HashMap of State to StartElementAction function mappings.
		 */
		static const HashMap<std::string, StartElementAction> sStartElementActions;

		/** ScopeParseHelper EndElementAction Type Definition
		 *	Defines EndElementAction function signatures for function pointer mappings.
		 */
		typedef void(ScopeParseHelper::*EndElementAction)(const std::string& element, ScopeSharedData* sharedData);

		/** ScopeParseHelper sEndElementActions
		 *	A static constant HashMap of State to EndElementAction function mappings.
		 */
		static const HashMap<std::string, EndElementAction> sEndElementActions;

		/** ScopeParseHelper sPrimitiveTypeMap
		 *	A static constant HashMap of string to pairs of Datum::DatumType States.
		 */
		static const HashMap<std::string, std::pair<Datum::DatumType, State>> sPrimitiveTypeMap;

		/** ScopeParseHelper sNameAttribute
		 *	A static constant string representing the value expected for name attributes.
		 */
		static const std::string sNameAttribute;

		/** ScopeParseHelper sValueAttribute
		 *	A static constant string representing the value expected for value attributes.
		 */
		static const std::string sValueAttribute;

		/** ScopeParseHelper sClassAttribute
		 *	A static constant string representing the value expected for class attributes.
		 */
		static const std::string sClassAttribute;

		/** ScopeParseHelper sInstanceAttribute
		 *	A static constant string representing the value expected for instance attributes.
		 */
		static const std::string sInstanceAttribute;

		/** ScopeParseHelper sIndexAttribute
		 * A static constant string representing the value expected for index attributes.
		 */
		static const std::string sIndexAttribute;
	};
}