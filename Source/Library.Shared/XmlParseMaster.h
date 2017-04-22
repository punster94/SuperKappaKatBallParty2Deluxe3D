#pragma once

#include <cstdint>
#include <string>
#include "Vector.h"
#include "HashMap.h"
#include "RTTI.h"
#include "expat.h"

namespace FieaGameEngine
{
	/** IXmlParseMaster Forward Declaration
	 *
	 */
	class IXmlParseHelper;

	/** XmlParseMaster class
	 *	A class intended to contain a list of helpers used for parsing XML.
	 */
	class XmlParseMaster final
	{

	public:

		/** XmlParseMaster::SharedData class
		 *	A contained class used to store data across multiple XmlParseMaster instances.
		 */
		class SharedData : public RTTI
		{
			RTTI_DECLARATIONS(SharedData, RTTI);

		public:

			/** SharedData Constructor
			 *	A constructor that initializes the depth of the shared data to 0 and its XmlParseMaster reference to nullptr.
			 */
			SharedData();

			/** SharedData Destructor
			 *	Polymorphic destructor that cleans up any data members or dependencies of this class.
			 */
			virtual ~SharedData();

			/** SharedDataInitialize
			 *	Implements the process a SharedData must go through before usage in parsing.
			 */
			virtual void Initialize();

			/** SharedData Clone
			 *	A virtual constructor that creates a clone of this SharedData and returns a pointer to it.
			 *	@return a SharedData pointer that points to a new SharedData that will behave like this SharedData.
			 */
			virtual SharedData* Clone() const;

			/** SharedData SetXmlParseMaster
			 *	Sets the XmlParseMaster reference of this SharedData to the one passed in.
			 *	@param xmlParseMaster the XmlParsePaster pointer to set this SharedData's XmlParseMaster reference to.
			 */
			void SetXmlParseMaster(XmlParseMaster* xmlParseMaster);

			/** SharedData GetXmlParseMaster
			 *	Gets the current XmlParseMaster reference of this SharedData.
			 *	@return an XmlParseMaster pointer that points to this SharedData's XmlParseMaster reference.
			 */
			XmlParseMaster* GetXmlParseMaster() const;

			/** SharedData IncrementDepth
			 *	Increments the current depth of this SharedData.
			 */
			void IncrementDepth();

			/** SharedData DecrementDepth
			 *	Decrements the current depth of this SharedData.
			 *	@exception will fail if the current depth of this SharedData is 0.
			 */
			void DecrementDepth();

			/** SharedData Depth
			 *	Returns the current depth of this SharedData.
			 *	@return an unsigned integer representing the current depth of this SharedData.
			 */
			std::uint32_t Depth() const;

		private:

			/** SharedData mXmlParseMaster
			 *	This SharedData's current XmlParseMaster reference.
			 */
			XmlParseMaster* mXmlParseMaster;

			/** SharedData mDepth
			 *	An unsigned integer representing this SharedData's current depth.
			 */
			std::uint32_t mDepth;
		};

		/** XmlParseMaster Constructor
		 *	Constructs an XmlParseMaster as default or using a provided SharedData pointer.
		 *	Initializes parser structure.
		 *	@param sharedData a SharedData pointer to set; defaulted to nullptr.
		 */
		XmlParseMaster(SharedData* sharedData = nullptr);

		/** XmlParseMaster Destructor
		 *	Destructs created members if this XmlParseMaster was a clone and frees the parsing structure.
		 */
		~XmlParseMaster();

		/** XmlParseMaster Clone
		 *	Clones this XmlParseMaster and all of its helpers and SharedData.
		 *	@return an XmlParseMaster pointer that points to the new clone.
		 */
		XmlParseMaster* Clone() const;

		/** XmlParseMaster AddHelper
		 *	Adds the given helper to a list of helpers that can be delegated to.
		 *	@exception will fail if given helper is nullptr.
		 *	@exception will fail if this XmlParseMaster is a clone.
		 *	@param helper an IXmlParseHelper pointer to be added to the list of helpers.
		 */
		void AddHelper(IXmlParseHelper* helper);

		/** XmlParseMaster RemoveHelper
		 *	Removes the given helper from the list of helpers that can be delegated to.
		 *	@exception will fail if given helper is nullptr.
		 *	@exception will fail if this XmlParseMaster is a clone.
		 *	@param helper an IXmlParseHelper pointer to be removed from the list of helpers.
		 */
		void RemoveHelper(IXmlParseHelper* helper);

		/** XmlParseMaster Parse
		 *	Parses the given character buffer of XML data.
		 *	@exception will fail if malformed XML is provided.
		 *	@param buffer a constant character pointer of XML data to be parsed.
		 *	@param length a constant unsigned integer representing the length of the character data to be parsed.
		 */
		void Parse(const char* buffer, const size_t length);

		/** XmlParseMaster ParseFromFile
		 *	Parses the file at the given path as XML data.
		 *	@exception will fail if the file could not be opened successfully.
		 *	@param fileName a constant string reference containing the path to the file to be parsed.
		 */
		void ParseFromFile(const std::string& fileName);

		/** XmlParseMaster GetFileName
		 *	Returns the path to the last file that began being parsed by this XmlParseMaster.
		 *	@return a constant string reference representing the path to the file.
		 */
		const std::string& GetFileName() const;

		/** XmlParseMaster GetSharedData
		 *	Returns the current SharedData reference of this XmlParseMaster.
		 *	@return a SharedData pointer pointing to the current SharedData reference of this XmlParseMaster.
		 */
		SharedData* GetSharedData() const;

		/** XmlParseMaster SetSharedData
		 *	Sets the current SharedData reference of this XmlParseMaster and its XmlParseMaster reference to each other.
		 *	@param sharedData a SharedData pointer set this XmlParseMaster's SharedData reference to.
		 */
		void SetSharedData(SharedData* sharedData);

	private:

		/** XmlParseMaster StartElementHandler
		 *	A static method that is called back during parsing to handle the entry of an XML element.
		 *	@param userData a void pointer containing the location of the XmlParseMaster that is parsing the XML.
		 *	@param name a constant character pointer containing the name of the XML element being entered.
		 *	@param attributes a constant character double pointer containing pairs of strings representing the attributes of the element being entered.
		 */
		static void StartElementHandler(void* userData, const char* name, const char** attributes);

		/** XmlParseMaster EndElementHandler
		 *	A static method that is called back during parsing to handle the completion of an XML element.
		 *	@param userData a void pointer containing the location of the XmlParseMaster that is parsing the XML.
		 *	@param name a constant character pointer containing the name of the XML element being completed.
		 */
		static void EndElementHandler(void* userData, const char* name);

		/** XmlParseMaster CharDataHandler
		 *	A static method that is called back during parsing to handle the character data inside of an XML element.
		 *	@param userData a void pointer containing the location of the XmlParseMaster that is parsing the XML.
		 *	@param buffer a constant character pointer containing the character data inside of the XML element being handled.
		 *	@param length an integer representing the length of the character data inside of the XML element being handled.
		 */
		static void CharDataHandler(void* userData, const char* buffer, std::int32_t length);

		/** XmlParseMaster ResetParser
		 *	Frees the current parser, reallocating a new one and setting its callbacks.
		 */
		void ResetParser();

		/** XmlParseMaster SetParserCallbackInformation
		 *	Sets the callback information of this XmlParseMaster's current parser.
		 */
		void SetParserCallbackInformation();

		/** XmlParseMaster InitializeDependencies
		 *	Initializes the list of helpers and the current SharedData reference.
		 */
		void InitializeDependencies();

		/** XmlParseMaster mIsClone
		 *	A boolean representing whether this XmlParseMaster is a clone.
		 */
		bool mIsClone;

		/** XmlParseMaster mSharedDataSetExternally
		 *	A boolean representing whether the SharedData reference of this XmlParseMaster was set externally with a call to SetSharedData.
		 */
		bool mSharedDataSetExternally;

		/** XmlParseMaster mParser
		 *	A structure that represents the expat parser wrapped by this master class.
		 */
		XML_Parser mParser;

		/** XmlParseMaster mFileName
		 *	A string containing the last file path that has begun parsing by this XmlParseMaster.
		 */
		std::string mFileName;

		/** XmlParseMaster mSharedData
		 *	A SharedData pointer containing the current SharedData reference of this XmlParseMaster.
		 */
		SharedData* mSharedData;

		/** XmlParseMaster mHelperList
		 *	A Vector of constant IXmlParseHelper pointers that can be used by this XmlParseMaster to handle incoming XML elements.
		 */
		Vector<IXmlParseHelper*> mHelperList;
	};
}