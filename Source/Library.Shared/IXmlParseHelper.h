#pragma once

#include <cstdint>
#include <string>
#include "XmlParseMaster.h"
#include "HashMap.h"

namespace FieaGameEngine
{
	/** IXmlParseHelper class
	 *	An abstract class intended to provide an interface for helpers in a chain-of-command parsing system.
	 */
	class IXmlParseHelper
	{

	public:

		/** IXmlParseHelper Constructor
		 *	Initializes members of the IXmlParseMaster class.
		 */
		IXmlParseHelper();

		/** IXmlParseHelper Destructor
		 *	Polymorphic destructor for helpers to clean up their data members if necessary.
		 */
		virtual ~IXmlParseHelper();

		/** IXmlParseHelper Copy Constructor
		 *	Explicitly deleted.
		 */
		IXmlParseHelper(const IXmlParseHelper& otherHelper) = delete;

		/** IXmlParseHelper Assignment Operator
		 *	Explicitly deleted.
		 */
		IXmlParseHelper& operator=(const IXmlParseHelper& otherHelper) = delete;

		/** IXmlParseHelper Initialize
		 *	Initializes the helper's members before being used to parse.
		 */
		virtual void Initialize();

		/** IXmlParseHelper StartElementHandler
		 *	Handles the attribute and name information of an element during parsing.
		 *	@param sharedData a reference to the SharedData object of the master calling this helper.
		 *	@param name a constant string reference containing the name of the element being parsed.
		 *	@param attributes a HashMap<std::string, std::string> reference containing information about the attributes of the element being parsed.
		 *	@return a boolean representing whether the element was handled correctly.
		 */
		virtual bool StartElementHandler(XmlParseMaster::SharedData* sharedData, const std::string& name, HashMap<std::string, std::string>& attributes) const;

		/** IXmlParseHelper EndElementHandler
		 *	Handles the completion of the element being parsed.
		 *	@param sharedData a reference to the SharedData object of the master calling this helper.
		 *	@param name a constant string reference containing the name of the element being completed.
		 *	@return a boolean representing whether the element completion was handled correctly.
		 */
		virtual bool EndElementHandler(XmlParseMaster::SharedData* sharedData, const std::string& name) const;

		/** IXmlParseHelper CharDataHandler
		 *	Handles the character data inside of an element during parsing.
		 *	@param sharedData a reference to the SharedData object of the master calling this helper.
		 *	@param buffer a constant string reference representing the character data inside the element being parsed.
		 *	@param length a constant unsigned integer representing the length of the character data inside the element being parsed.
		 *	@return a boolean representing whether the element was handled correctly.
		 */
		virtual bool CharDataHandler(XmlParseMaster::SharedData* sharedData, const std::string& buffer, const std::uint32_t length) const;

		/** IXmlParseHelper Clone
		 *	Clones this helper, including any necessary data members to behave like this helper in its current state.
		 *	Explicitly deleted to ensure instances of this class cannot be created.
		 *	@return a pointer to a new IXmlParseHelper that is a duplicate of this helper.
		 */
		virtual IXmlParseHelper* Clone() const = 0;
	};
}